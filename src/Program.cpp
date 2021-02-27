#include "Program.hpp"

#include "config.hpp"

#include <iostream>
#include <optional>
#include <unordered_map>
#include <string>

#include "DataEmpty.hpp"
#include "DataFunction.hpp"
#include "DataInt.hpp"
#include "DataString.hpp"

#include "TokenAssignment.hpp"
#include "TokenBlock.hpp"
#include "TokenConcat.hpp"
#include "TokenDivide.hpp"
#include "TokenFunction.hpp"
#include "TokenInt.hpp"
#include "TokenLiteral.hpp"
#include "TokenMinus.hpp"
#include "TokenMultiply.hpp"
#include "TokenPlus.hpp"
#include "TokenString.hpp"

#include "is_whitespace.hpp"
#include "ParseError.hpp"
#if PERF_PROFILING
#include "Profiling.hpp"
#endif
#include "opcodes.hpp"
#include "read_file.hpp"

namespace Utopia
{
	void Program::echo_impl_stdout(Program* p, void* arg, const char* str)
	{
		std::cout << str;
	}

	void Program::printVariables()
	{
		std::cout << "Variables:" << std::endl;
		size_t i = 0;
		for (const auto& var : variables)
		{
			std::cout << std::to_string(i++) << ": " << var->toCPP() << std::endl;
		}
		std::cout << std::endl;
	}

	static void profilingStartSection(const char* section)
	{
#if PERF_PROFILING
		Profiling::startSection(section);
#endif
	}

	static void profilingEndSection(const char* section)
	{
#if PERF_PROFILING
		Profiling::endSection(section);
#endif
	}

	struct LiteralBuffer
	{
		SourceLocation loc;
		std::string data;
		
		LiteralBuffer(const SourceLocation& loc, char c)
			: loc(loc), data(1, c)
		{
		}
	};

	struct InternalExceptionEndParsing
	{
	};

	static void finishLiteralToken(std::vector<std::unique_ptr<Token>>& tokens, std::optional<LiteralBuffer>& literal_buffer)
	{
		if (!literal_buffer.has_value())
		{
			return;
		}
		try
		{
			long long value = std::stoll(literal_buffer.value().data);
			tokens.emplace_back(std::make_unique<TokenInt>(literal_buffer.value().loc, value));
		}
		catch (const std::exception&)
		{
			if (literal_buffer.value().data == "_end_parsing")
			{
				throw InternalExceptionEndParsing();
			}
			tokens.emplace_back(std::make_unique<TokenLiteral>(literal_buffer.value().loc, std::move(literal_buffer.value().data)));
		}
		literal_buffer.reset();
	}

	static void squashIntoContainer(std::vector<std::unique_ptr<Token>>& tokens, std::vector<std::unique_ptr<Token>>::iterator& i)
	{
		Token* const token = i->get();
		if (i == tokens.begin() || i + 1 == tokens.end())
		{
			token->throwUnexpected();
		}
		i--;
		((TokenContainer*)token)->left = std::move(*i);
		i = tokens.erase(i);
		i++;
		((TokenContainer*)token)->right = std::move(*i);
		i = tokens.erase(i);
	}

#if DEBUG_TOKENS
	static void _printToken(const std::string& prefix, const Token* token, const char* name = nullptr)
	{
		if (token == nullptr)
		{
			return;
		}
		std::cout << prefix;
		if (name != nullptr)
		{
			std::cout << name << ": ";
		}
		std::cout << token->getName() << std::endl;
		if (token->isContainer())
		{
			const std::string child_prefix = std::string(prefix).append("    ");
			_printToken(child_prefix, ((const TokenContainer*)token)->left.get(), "left");
			_printToken(child_prefix, ((const TokenContainer*)token)->right.get(), "right");
		}
	}

	static void printTokens(const char* scope, const std::vector<std::unique_ptr<Token>>& tokens)
	{
		std::cout << scope << ":" << std::endl;
		const std::string prefix = "    ";
		for (const auto& token : tokens)
		{
			_printToken(prefix, token.get());
		}
	}
#endif

	struct VariableData
	{
		size_t index;
		SourceLocation loc;
	};

	static void emplaceOp(Scope& scope, const uint8_t op, const SourceLocation& loc)
	{
		scope.ops.emplace_back(op);
		scope.op_locs.emplace_back(loc);
	}

	static void emplaceOp(Scope& scope, const size_t op, const SourceLocation& loc)
	{
		// TODO: Properly deal with size_t
		return emplaceOp(scope, uint8_t(op), loc);
	}

	template <typename T>
	static void emplaceOp(Scope& scope, const T op, const Token* token)
	{
		return emplaceOp(scope, op, token->loc);
	}

	static void emplaceOp(Scope& scope, const VariableData& var)
	{
		return emplaceOp(scope, var.index, var.loc);
	}

	[[nodiscard]] static VariableData expandVariable(Program& p, Scope& scope, const std::unordered_map<std::string, size_t>& var_map, Token* token);

	[[nodiscard]] static VariableData emplaceContainer(Program& p, Scope& scope, const std::unordered_map<std::string, size_t>& var_map, TokenContainer* token, const OpCode opcode)
	{
		VariableData var{ p.variables.size(), token->getLeftmostSourceLocation() };
		if (std::unique_ptr<Data> data = token->attemptToEvaluate())
		{
			p.variables.emplace_back(std::move(data));
		}
		else
		{
			p.variables.emplace_back(std::make_unique<DataEmpty>());
			auto l_var = expandVariable(p, scope, var_map, ((TokenContainer*)token)->left.get());
			auto r_var = expandVariable(p, scope, var_map, ((TokenContainer*)token)->right.get());
			emplaceOp(scope, opcode, token);
			emplaceOp(scope, var);
			emplaceOp(scope, l_var);
			emplaceOp(scope, r_var);
		}
		return var;
	}

	[[nodiscard]] static VariableData emplaceContainer(Program& p, Scope& scope, const std::unordered_map<std::string, size_t>& var_map, Token* token, const OpCode opcode)
	{
		return emplaceContainer(p, scope, var_map, (TokenContainer*)token, opcode);
	}

	static void scopeFromString(Program& p, Scope& scope, SourceLocation loc, const std::string& code, std::unordered_map<std::string, size_t> var_map);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
	[[nodiscard]] static VariableData expandVariable(Program& p, Scope& scope, const std::unordered_map<std::string, size_t>& var_map, Token* token)
	{
		switch (token->type)
		{
		case TOKEN_STRING:
		{
			VariableData var{ p.variables.size(), token->loc };
			p.variables.emplace_back(std::make_unique<DataString>(std::move(((TokenString*)token)->value)));
			return var;
		}

		case TOKEN_INT:
		{
			VariableData var{ p.variables.size(), token->loc };
			p.variables.emplace_back(std::make_unique<DataInt>(((TokenInt*)token)->value));
			return var;
		}

		case TOKEN_FUNC:
		{
			Scope s{};
			scopeFromString(p, s, ((TokenFunction*)token)->contents_start_loc, ((TokenFunction*)token)->contents, var_map);
			VariableData var{ p.variables.size(), token->loc };
			p.variables.emplace_back(std::make_unique<DataFunction>(std::move(s)));
			return var;
		}
		break;

		case TOKEN_LITERAL:
		{
			auto var_entry = var_map.find(((TokenLiteral*)token)->literal);
			if (var_entry == var_map.end())
			{
				token->loc.throwHere<ParseError>(std::move(std::string("Use of undefined variable '").append(((TokenLiteral*)token)->literal).append(1, '\'')));
			}
			return { var_entry->second, token->loc };
		}

		case TOKEN_PLUS:
			return emplaceContainer(p, scope, var_map, token, OP_PLUS);

		case TOKEN_MINUS:
			return emplaceContainer(p, scope, var_map, token, OP_MINUS);

		case TOKEN_MULTIPLY:
			return emplaceContainer(p, scope, var_map, token, OP_MULTIPLY);

		case TOKEN_DIVIDE:
			return emplaceContainer(p, scope, var_map, token, OP_DIVIDE);

		case TOKEN_CONCAT:
			return emplaceContainer(p, scope, var_map, token, OP_CONCAT);
		}
		token->throwUnexpected();
	}

	static void scopeFromString(Program& p, Scope& scope, SourceLocation loc, const std::string& code, std::unordered_map<std::string, size_t> var_map)
	{
		std::vector<std::unique_ptr<Token>> tokens{};

		profilingStartSection("Tokenization");
		{
			std::optional<LiteralBuffer> literal_buffer = std::nullopt;
			try
			{
				auto i = code.begin();
				while (i != code.end())
				{
					char c = *i++;
					switch (c)
					{
					case '\r':
						loc.character++;
						break;

					default:
						if (literal_buffer.has_value())
						{
							literal_buffer.value().data.append(1, c);
						}
						else
						{
							literal_buffer.emplace(loc, c);
						}
						loc.character++;
						break;

					case '\n':
						finishLiteralToken(tokens, literal_buffer);
						loc.newline();
						break;

					case ' ':
					case '\t':
					case ';':
					case '.':
						finishLiteralToken(tokens, literal_buffer);
						loc.character++;
						break;

					case '"':
						finishLiteralToken(tokens, literal_buffer);
						{
							auto str = std::make_unique<TokenString>(loc);
							while (i != code.end())
							{
								loc.character++;
								c = *i++;
								if (c == '\n')
								{
									loc.throwHere<ParseError>("Unexpected new line while reading double-quoted string");
								}
								if (c == '"')
								{
									break;
								}
								str->value.append(1, c);
							}
							tokens.emplace_back(std::move(str));
						}
						loc.character++;
						break;

					case '+':
						finishLiteralToken(tokens, literal_buffer);
						tokens.emplace_back(std::make_unique<TokenPlus>(loc));
						loc.character++;
						break;

					case '-':
						finishLiteralToken(tokens, literal_buffer);
						tokens.emplace_back(std::make_unique<TokenMinus>(loc));
						loc.character++;
						break;

					case '*':
						finishLiteralToken(tokens, literal_buffer);
						tokens.emplace_back(std::make_unique<TokenMultiply>(loc));
						loc.character++;
						break;

					case '/':
						switch (i == code.end() ? 0 : *i)
						{
						case '/':
							i++;
							while (i != code.end() && *i++ != '\n');
							loc.newline();
							break;

						case '*':
							i++;
							loc.character += 2;
							{
								bool star = true;
								while (i != code.end())
								{
									loc.character++;
									c = *i++;
									if (c == '*')
									{
										star = true;
										continue;
									}
									if (star && c == '/')
									{
										break;
									}
									star = false;
									if (c == '\n')
									{
										loc.newline();
										continue;
									}
								}
							}
							break;

						default:
							finishLiteralToken(tokens, literal_buffer);
							tokens.emplace_back(std::make_unique<TokenDivide>(loc));
							loc.character++;
						}
						break;

					case '=':
						finishLiteralToken(tokens, literal_buffer);
						tokens.emplace_back(std::make_unique<TokenAssignment>(loc));
						loc.character++;
						break;

					case '#':
						while (i != code.end() && *i++ != '\n');
						loc.newline();
						break;

					case '|':
						finishLiteralToken(tokens, literal_buffer);
						if (tokens.empty())
						{
						_unexpected_pipe:
							loc.throwHere<ParseError>("Unexpected |");
						}
						{
							auto prev_token_i = tokens.end() - 1;
							Token* const prev_token = prev_token_i->get();
							if (prev_token->type != TOKEN_LITERAL)
							{
								goto _unexpected_pipe;
							}
							const char* line_ending = nullptr;
							if (((TokenLiteral*)prev_token)->literal == "CRLF")
							{
								line_ending = "\r\n";
							}
							else if (((TokenLiteral*)prev_token)->literal == "LF")
							{
								line_ending = "\n";
							}
							else if (((TokenLiteral*)prev_token)->literal == "CR")
							{
								line_ending = "\r";
							}
							if (line_ending == nullptr)
							{
								goto _unexpected_pipe;
							}
							auto str = std::make_unique<TokenString>(loc);
							while (i != code.end())
							{
								loc.character++;
								c = *i++;
								if (c == '\r')
								{
									continue;
								}
								if (c == '\n')
								{
									loc.newline();
									auto j = i;
									while (j != code.end())
									{
										loc.character++;
										c = *j++;
										if (c == '|')
										{
											i = j;
											str->value.append(line_ending);
											break;
										}
										if (!is_whitespace(c))
										{
											loc.character = 1;
											goto _pipe_string_ends;
										}
									}
									continue;
								}
								str->value.append(1, c);
							}
						_pipe_string_ends:
							*prev_token_i = std::move(str);
						}
						break;

					case '{':
					{
						SourceLocation start_loc(loc);
						auto block = std::make_unique<TokenBlock>(loc);
						int depth = 1;
						while (i != code.end())
						{
							loc.character++;
							c = *i++;
							if (c == '}')
							{
								if (--depth == 0)
								{
									goto _finish_block;
								}
							}
							else if (c == '{')
							{
								depth++;
							}
							else if (c == '\n')
							{
								loc.newline();
							}
							block->contents.append(1, c);
						}
						start_loc.throwHere<ParseError>("Found end of file before closing } for the block");
					_finish_block:
						tokens.emplace_back(std::move(block));
					}
					break;
					}
				}
				finishLiteralToken(tokens, literal_buffer);
			}
			catch (const InternalExceptionEndParsing&)
			{
			}
		}
		profilingEndSection("Tokenization");
#if DEBUG_TOKENS
		printTokens("Tokenization", tokens);
#endif

		{
			profilingStartSection("Squash Function");
#if DEBUG_TOKENS
			auto pre_squash_size = tokens.size();
#endif
			for (auto i = tokens.begin(); i != tokens.end(); i++)
			{
				Token* const token = i->get();
				if (token->type == TOKEN_BLOCK)
				{
					if (i == tokens.begin())
					{
						token->throwUnexpected();
					}
					i--;
					Token* const prev_token = i->get();
					prev_token->expectType(TOKEN_LITERAL);
					if (((TokenLiteral*)prev_token)->literal != "fn")
					{
						prev_token->throwUnexpected();
					}
					auto func = std::make_unique<TokenFunction>(prev_token->loc, token->loc, std::move(((TokenBlock*)token)->contents));
					func->contents_start_loc.character += 1;
					i = tokens.erase(i);
					*i = std::move(func);
				}
			}
#if DEBUG_TOKENS
			if (tokens.size() != pre_squash_size)
			{
				printTokens("Squash Function", tokens);
			}
#endif
			profilingEndSection("Squash Function");
		}
		{
			profilingStartSection("Squash Multiplication/Division");
#if DEBUG_TOKENS
			auto pre_squash_size = tokens.size();
#endif
			auto i = tokens.begin();
			while (i != tokens.end())
			{
				Token* const token = i->get();
				switch (token->type)
				{
				default:
					i++;
					break;

				case TOKEN_MULTIPLY:
				case TOKEN_DIVIDE:
					squashIntoContainer(tokens, i);
					break;
				}
			}
#if DEBUG_TOKENS
			if (tokens.size() != pre_squash_size)
			{
				printTokens("Squash Multiplication/Division", tokens);
			}
#endif
			profilingEndSection("Squash Multiplication/Division");
		}
		{
			profilingStartSection("Squash Addition/Subtraction");
#if DEBUG_TOKENS
			auto pre_squash_size = tokens.size();
#endif
			auto i = tokens.begin();
			while (i != tokens.end())
			{
				Token* const token = i->get();
				switch (token->type)
				{
				default:
					i++;
					break;

				case TOKEN_PLUS:
				case TOKEN_MINUS:
					squashIntoContainer(tokens, i);
					break;
				}
			}
#if DEBUG_TOKENS
			if (tokens.size() != pre_squash_size)
			{
				printTokens("Squash Addition/Subtraction", tokens);
			}
#endif
			profilingEndSection("Squash Addition/Subtraction");
		}
		{
			profilingStartSection("Squash Concat");
			size_t pre_squash_size;
			do
			{
				pre_squash_size = tokens.size();
				auto i = tokens.begin();
				while (i != tokens.end())
				{
					Token* const token = i->get();
					switch (token->type)
					{
					case TOKEN_STRING:
					case TOKEN_CONCAT:
						if (i == tokens.begin())
						{
							token->throwUnexpected();
						}
						if (i + 1 != tokens.end())
						{
							if ((*(i + 1))->isRValue())
							{
								auto concat = std::make_unique<TokenConcat>(token->loc);
								concat->left = std::move(*i);
								i++;
								concat->right = std::move(*i);
								i = tokens.erase(i);
								i--;
								*i = std::move(concat);
								i++;
								break;
							}
						}
						[[fallthrough]];
					default:
						i++;
						break;
					}
				}
#if DEBUG_TOKENS
				if (tokens.size() != pre_squash_size)
				{
					printTokens("Squash Concat", tokens);
				}
#endif
			} while (tokens.size() != pre_squash_size);
			profilingEndSection("Squash Concat");
		}
		{
			profilingStartSection("Squash Assignment");
#if DEBUG_TOKENS
			auto pre_squash_size = tokens.size();
#endif
			auto i = tokens.begin();
			while (i != tokens.end())
			{
				Token* const token = i->get();
				switch (token->type)
				{
				default:
					i++;
					break;

				case TOKEN_ASSIGNMENT:
					squashIntoContainer(tokens, i);
					break;
				}
			}
#if DEBUG_TOKENS
			if (tokens.size() != pre_squash_size)
			{
				printTokens("Squash Assignment", tokens);
			}
#endif
			profilingEndSection("Squash Assignment");
		}

		profilingStartSection("Assemble Variables");
		for (const auto& i : tokens)
		{
			Token* const token = i.get();
			switch (token->type)
			{
			case TOKEN_ASSIGNMENT:
			{
				((TokenAssignment*)token)->left->expectType(TOKEN_LITERAL);
				std::string& var_name = ((TokenLiteral*)((TokenAssignment*)token)->left.get())->literal;
				auto r_val = expandVariable(p, scope, var_map, ((TokenAssignment*)token)->right.get());
				if (((TokenAssignment*)token)->right->isRValue())
				{
					auto var_map_entry = var_map.find(var_name);
					if (var_map_entry != var_map.end())
					{
						// TODO: Maybe just allow it and throw a warning instead?
						throw ParseError(std::string("Reassignment of constant variable '").append(var_name).append(1, '\''));
					}
					var_map.emplace(std::move(var_name), r_val.index);
				}
				else
				{
					emplaceOp(scope, OP_ASSIGNMENT, token);
					auto var_map_entry = var_map.find(var_name);
					if (var_map_entry == var_map.end())
					{
						var_map.emplace(std::move(var_name), p.variables.size());
						emplaceOp(scope, p.variables.size(), ((TokenAssignment*)token)->left.get());
						p.variables.emplace_back(std::make_unique<DataEmpty>());
					}
					else
					{
						emplaceOp(scope, var_map_entry->second, ((TokenAssignment*)token)->left.get());
					}
					emplaceOp(scope, r_val);
				}
			}
			break;
			}
		}
		profilingEndSection("Assemble Variables");

		profilingStartSection("Assemble Calls");
		{
			auto i = tokens.cbegin();
			while (i != tokens.cend())
			{
				Token* const token = i->get();
				switch (token->type)
				{
				case TOKEN_LITERAL:
				{
					auto& literal = ((TokenLiteral*)token)->literal;
					if (literal == "echo" || literal == "print")
					{
						if (++i == tokens.cend())
						{
							token->throwUnexpected();
						}
						auto arg_var = expandVariable(p, scope, var_map, i->get());
						emplaceOp(scope, OP_ECHO, token);
						emplaceOp(scope, arg_var);
					}
					else
					{
						auto var_entry = var_map.find(literal);
						if (var_entry == var_map.end())
						{
							token->throwUnexpected();
						}
						Data* const var = p.variables.at(var_entry->second).get();
						var->expectType(DATA_FUNC, token->loc);
						emplaceOp(scope, OP_CALL, token);
						emplaceOp(scope, var_entry->second, token);
					}
				}
				[[fallthrough]];
				default:
					i++;
					break;
				}
			}
		}
		profilingEndSection("Assemble Calls");
	}
#pragma clang diagnostic pop

	void Program::fromString(std::string&& name, const std::string& code)
	{
		SourceLocation loc(std::move(name));
		std::unordered_map<std::string, size_t> var_map{};
		scopeFromString(*this, *this, loc, code, var_map);
	}

	void Program::fromFile(std::string&& path)
	{
		std::string buffer = read_file(path);
		return fromString(std::move(path), buffer);
	}

	void Program::eraseDebugInformation()
	{
		this->op_locs.clear();
		for (auto& var : variables)
		{
			Data* const data = var.get();
			if (data->type == DATA_FUNC)
			{
				((DataFunction*)data)->scope.op_locs.clear();
			}
		}
	}

	void Program::execute()
	{
		return static_cast<Scope*>(this)->execute(this);
	}
}
