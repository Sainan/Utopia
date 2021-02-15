#include "Program.hpp"

#include "config.hpp"

#if PERF_PROFILING
#include <chrono>
#endif
#if DEBUG_TOKENS || DEBUG_VM
#include <iostream>
#endif
#include <optional>
#include <unordered_map>
#include <string>

#include "DataEmpty.hpp"
#include "DataInt.hpp"
#include "DataString.hpp"

#include "ParseError.hpp"
#include "VmError.hpp"
#include "TypeError.hpp"

#include "TokenAssignment.hpp"
#include "TokenConcat.hpp"
#include "TokenDivide.hpp"
#include "TokenInt.hpp"
#include "TokenLiteral.hpp"
#include "TokenMinus.hpp"
#include "TokenMultiply.hpp"
#include "TokenPlus.hpp"
#include "TokenString.hpp"

#if PERF_PROFILING
#include "Profiling.hpp"
#endif
#include "read_file.hpp"
#include "opcodes.hpp"

namespace Utopia
{
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

	static void emplaceOp(Program& p, const uint8_t op, const SourceLocation& loc)
	{
		p.ops.emplace_back(op);
		p.op_locs.emplace_back(loc);
	}

	static void emplaceOp(Program& p, const size_t op, const SourceLocation& loc)
	{
		// TODO: Properly deal with size_t
		return emplaceOp(p, uint8_t(op), loc);
	}

	template <typename T>
	static void emplaceOp(Program& p, const T op, const Token* token)
	{
		return emplaceOp(p, op, token->loc);
	}

	static void emplaceOp(Program& p, const VariableData& var)
	{
		return emplaceOp(p, var.index, var.loc);
	}

	[[nodiscard]] static VariableData expandVariable(Program& p, const std::unordered_map<std::string, size_t>& var_map, Token* token);

	[[nodiscard]] static VariableData emplaceContainer(Program& p, const std::unordered_map<std::string, size_t>& var_map, TokenContainer* token, const OpCode opcode)
	{
		VariableData var{ p.variables.size(), token->getLeftmostSourceLocation() };
		if (std::unique_ptr<Data> data = token->attemptToEvaluate())
		{
			p.variables.emplace_back(std::move(data));
		}
		else
		{
			p.variables.emplace_back(std::make_unique<DataEmpty>());
			auto l_var = expandVariable(p, var_map, ((TokenContainer*)token)->left.get());
			auto r_var = expandVariable(p, var_map, ((TokenContainer*)token)->right.get());
			emplaceOp(p, opcode, token);
			emplaceOp(p, var);
			emplaceOp(p, l_var);
			emplaceOp(p, r_var);
		}
		return var;
	}

	[[nodiscard]] static VariableData emplaceContainer(Program& p, const std::unordered_map<std::string, size_t>& var_map, Token* token, const OpCode opcode)
	{
		return emplaceContainer(p, var_map, (TokenContainer*)token, opcode);
	}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
	[[nodiscard]] static VariableData expandVariable(Program& p, const std::unordered_map<std::string, size_t>& var_map, Token* token)
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
			return emplaceContainer(p, var_map, token, OP_PLUS);

		case TOKEN_MINUS:
			return emplaceContainer(p, var_map, token, OP_MINUS);

		case TOKEN_MULTIPLY:
			return emplaceContainer(p, var_map, token, OP_MULTIPLY);

		case TOKEN_DIVIDE:
			return emplaceContainer(p, var_map, token, OP_DIVIDE);

		case TOKEN_CONCAT:
			return emplaceContainer(p, var_map, token, OP_CONCAT);
		}
		token->throwUnexpected();
	}

	Program Program::fromString(std::string&& name, const std::string& code)
	{
		Program p{};
		std::unordered_map<std::string, size_t> var_map{};

		std::vector<std::unique_ptr<Token>> tokens{};

		// Tokenize
		profilingStartSection("Tokenization");
		{
			SourceLocation loc(std::move(name));
			std::optional<LiteralBuffer> literal_buffer = std::nullopt;
			try
			{
				auto i = code.begin();
				while (i != code.end())
				{
					loc.character++;
					char c = *i++;
					switch (c)
					{
					case '\r':
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
						break;

					case '\n':
						finishLiteralToken(tokens, literal_buffer);
						loc.line++;
						loc.character = 0;
						break;

					case ' ':
					case ';':
					case '.':
						finishLiteralToken(tokens, literal_buffer);
						break;

					case '"':
						finishLiteralToken(tokens, literal_buffer);
						{
							auto str = std::make_unique<TokenString>(loc);
							while (i != code.end())
							{
								loc.character++;
								char c = *i++;
								if (c == '\n')
								{
									loc.throwHere<ParseError>("Unexpected new line while reading string");
								}
								if (c == '"')
								{
									tokens.emplace_back(std::move(str));
									break;
								}
								str->value.append(1, c);
							}
						}
						break;

					case '+':
						finishLiteralToken(tokens, literal_buffer);
						tokens.emplace_back(std::make_unique<TokenPlus>(loc));
						break;

					case '-':
						finishLiteralToken(tokens, literal_buffer);
						tokens.emplace_back(std::make_unique<TokenMinus>(loc));
						break;

					case '*':
						finishLiteralToken(tokens, literal_buffer);
						tokens.emplace_back(std::make_unique<TokenMultiply>(loc));
						break;

					case '/':
						finishLiteralToken(tokens, literal_buffer);
						tokens.emplace_back(std::make_unique<TokenDivide>(loc));
						break;

					case '=':
						finishLiteralToken(tokens, literal_buffer);
						tokens.emplace_back(std::make_unique<TokenAssignment>(loc));
						break;
					}
				}
			}
			catch (const InternalExceptionEndParsing&)
			{
			}
		}
		profilingEndSection("Tokenization");

#if DEBUG_TOKENS
		printTokens("Tokens from source", tokens);
#endif

		// Squash Round 1
		profilingStartSection("Squash Round 1");
		{
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
				printTokens("Tokens after squashing round 1", tokens);
			}
#endif
		}
		profilingEndSection("Squash Round 1");
		// Squash Round 2
		profilingStartSection("Squash Round 2");
		{
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
				printTokens("Tokens after squashing round 2", tokens);
			}
#endif
		}
		profilingEndSection("Squash Round 2");
		// Squash Round 3
		profilingStartSection("Squash Round 3");
		{
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
					printTokens("Tokens after squashing round 3", tokens);
				}
#endif
			} while (tokens.size() != pre_squash_size);
		}
		profilingEndSection("Squash Round 3");
		// Squash Round 4
		profilingStartSection("Squash Round 4");
		{
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
				printTokens("Tokens after squashing round 4", tokens);
			}
#endif
		}
		profilingEndSection("Squash Round 4");

		// Assemble Round 1: Variables
		profilingStartSection("Assemble Round 1");
		for (const auto& i : tokens)
		{
			Token* const token = i.get();
			switch (token->type)
			{
			case TOKEN_ASSIGNMENT:
				{
					((TokenAssignment*)token)->left->expectType(TOKEN_LITERAL);
					std::string& var_name = ((TokenLiteral*)((TokenAssignment*)token)->left.get())->literal;
					auto r_val = expandVariable(p, var_map, ((TokenAssignment*)token)->right.get());
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
						emplaceOp(p, OP_ASSIGNMENT, token);
						auto var_map_entry = var_map.find(var_name);
						if (var_map_entry == var_map.end())
						{
							var_map.emplace(std::move(var_name), p.variables.size());
							emplaceOp(p, p.variables.size(), ((TokenAssignment*)token)->left.get());
							p.variables.emplace_back(std::make_unique<DataEmpty>());
						}
						else
						{
							emplaceOp(p, var_map_entry->second, ((TokenAssignment*)token)->left.get());
						}
						emplaceOp(p, r_val);
					}
				}
				break;
			}
		}
		profilingEndSection("Assemble Round 1");
		// Assemble Round 2: Echo
		profilingStartSection("Assemble Round 2");
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
							auto arg_var = expandVariable(p, var_map, i->get());
							emplaceOp(p, OP_ECHO, token);
							emplaceOp(p, arg_var);
						}
						else
						{
							token->throwUnexpected();
						}
					}
					[[fallthrough]];
				default:
					i++;
					break;
				}
			}
		}
		profilingEndSection("Assemble Round 2");

		return p;
	}
#pragma clang diagnostic pop

	Program Program::fromFile(std::string&& path)
	{
		std::string buffer = read_file(path);
		return Program::fromString(std::move(path), buffer);
	}

	static void vm_debug_printArguments(std::vector<uint8_t>::iterator& opcode_i, std::vector<uint8_t>::iterator& i)
	{
#if DEBUG_VM
		while (++opcode_i != i)
		{
			std::cout << "op_arg: " << std::to_string(*opcode_i) << std::endl;
		}
#endif
	}

	template <typename E>
	__declspec(noreturn) static void vm_catch(Program& p, std::vector<uint8_t>::iterator& opcode_i, std::vector<uint8_t>::iterator& i, const E& e)
	{
		vm_debug_printArguments(opcode_i, i);
		if (p.op_locs.empty())
		{
			throw e;
		}
		p.op_locs.at(i - 1 - p.ops.begin()).throwHere<E>(e.what());
	}

	void Program::execute()
	{
#if DEBUG_VM
		std::cout << "Variables:" << std::endl;
		for (const auto& var : variables)
		{
			std::cout << var->toCPP() << std::endl;
		}
		std::cout << std::endl;
#endif

		auto i = ops.begin();
		while (i != ops.end())
		{
			auto opcode_i = i++;
			try
			{
				auto opcode = *opcode_i;
#if DEBUG_VM
				if (opcode >= _OP_SIZE)
				{
					throw VmError(std::string("Unknown opcode ").append(std::to_string(opcode)));
				}
				std::cout << "opcode: " << std::to_string(opcode) << std::endl;
#endif
				opcodes[opcode].execute(*this, i);
				vm_debug_printArguments(opcode_i, i);
			}
			catch (const TypeError& e)
			{
				vm_catch<>(*this, opcode_i, i, e);
			}
			catch (const VmError& e)
			{
				vm_catch<>(*this, opcode_i, i, e);
			}
		}
	}
}
