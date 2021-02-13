#include "Program.hpp"

#define DEBUG_TOKENS false

#include <fstream>
#if DEBUG_TOKENS
#include <iostream>
#endif
#include <optional>
#include <string>
#include <unordered_map>

#include "DataString.hpp"
#include "ParseError.hpp"
#include "OpCode.hpp"
#include "OpEcho.hpp"
#include "TokenAssignment.hpp"
#include "TokenDivide.hpp"
#include "TokenInt.hpp"
#include "TokenLiteral.hpp"
#include "TokenMinus.hpp"
#include "TokenMultiply.hpp"
#include "TokenPlus.hpp"
#include "TokenString.hpp"

namespace Utopia
{
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

#if DEBUG_TOKENS
	static void printTokens(const char* scope, std::vector<std::unique_ptr<Token>>& tokens)
	{
		std::cout << scope  << ": ";
		for (size_t i = 0; i < tokens.size(); i++)
		{
			if (i != 0)
			{
				std::cout << ", ";
			}
			std::cout << tokens.at(i)->getName();
		}
		std::cout << std::endl;
	}
#endif

	static std::unique_ptr<DataString> tokenToData(Token* token)
	{
		if (token->type == TOKEN_INT)
		{
			return std::make_unique<DataString>(std::to_string(((TokenInt*)token)->value));
		}
		else
		{
			token->expectType(TOKEN_STRING);
			return std::make_unique<DataString>(std::move(((TokenString*)token)->value));
		}
	}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
	Program Program::fromString(std::string&& name, const std::string& code)
	{
		std::vector<std::unique_ptr<Token>> tokens{};

		// Tokenize
		{
			SourceLocation loc(std::move(name));
			std::optional<std::unique_ptr<TokenString>> string_buffer = std::nullopt;
			std::optional<LiteralBuffer> literal_buffer = std::nullopt;
			try
			{
				for (auto c : code)
				{
					loc.colon++;
					if (c == '\r')
					{
						continue;
					}
					if (c == '\n')
					{
						if (string_buffer.has_value())
						{
							loc.throwHere("Unexpected new line while reading string");
						}
						finishLiteralToken(tokens, literal_buffer);
						loc.line++;
						loc.colon = 0;
						continue;
					}
					if (string_buffer.has_value())
					{
						if (c == '"')
						{
							tokens.emplace_back(std::move(string_buffer.value()));
							string_buffer.reset();
						}
						else
						{
							string_buffer.value()->value.append(1, c);
						}
					}
					else switch (c)
					{
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

					case ' ':
					case ';':
					case '.':
						finishLiteralToken(tokens, literal_buffer);
						break;

					case '"':
						finishLiteralToken(tokens, literal_buffer);
						string_buffer = std::make_unique<TokenString>(loc);
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

#if DEBUG_TOKENS
		printTokens("Tokens from source", tokens);
#endif

		// Squash Round 1
		{
			size_t pre_squash_size;
			do
			{
				pre_squash_size = tokens.size();
				Token* prev_token = nullptr;
				for (size_t i = 0; i < tokens.size(); i++)
				{
					Token* const token = tokens.at(i).get();
					switch (token->type)
					{
					case TOKEN_MULTIPLY:
						if (prev_token == nullptr || i + 1 == tokens.size())
						{
							token->throwUnexpected();
						}
						prev_token->expectType(TOKEN_INT);
						{
							Token* const next_token = tokens.at(i + 1).get();
							next_token->expectType(TOKEN_INT);
							((TokenInt*)prev_token)->value *= ((TokenInt*)next_token)->value;
						}
						tokens.erase(tokens.cbegin() + i, tokens.cbegin() + (i + 2));
						break;

					case TOKEN_DIVIDE:
						if (prev_token == nullptr || i + 1 == tokens.size())
						{
							token->throwUnexpected();
						}
						prev_token->expectType(TOKEN_INT);
						{
							Token* const next_token = tokens.at(i + 1).get();
							next_token->expectType(TOKEN_INT);
							((TokenInt*)prev_token)->value /= ((TokenInt*)next_token)->value;
						}
						tokens.erase(tokens.cbegin() + i, tokens.cbegin() + (i + 2));
						break;
					}
					prev_token = token;
				}

#if DEBUG_TOKENS
				if (tokens.size() != pre_squash_size)
				{
					printTokens("Tokens after squashing round 1", tokens);
				}
#endif
			} while (tokens.size() != pre_squash_size);
		}
		// Squash Round 2
		{
			size_t pre_squash_size;
			do
			{
				pre_squash_size = tokens.size();
				Token* prev_token = nullptr;
				for (size_t i = 0; i < tokens.size(); i++)
				{
					Token* const token = tokens.at(i).get();
					switch (token->type)
					{
					case TOKEN_PLUS:
						if (prev_token == nullptr || i + 1 == tokens.size())
						{
							token->throwUnexpected();
						}
						prev_token->expectType(TOKEN_INT);
						{
							Token* const next_token = tokens.at(i + 1).get();
							next_token->expectType(TOKEN_INT);
							((TokenInt*)prev_token)->value += ((TokenInt*)next_token)->value;
						}
						tokens.erase(tokens.cbegin() + i, tokens.cbegin() + (i + 2));
						break;

					case TOKEN_MINUS:
						if (prev_token == nullptr || i + 1 == tokens.size())
						{
							token->throwUnexpected();
						}
						prev_token->expectType(TOKEN_INT);
						{
							Token* const next_token = tokens.at(i + 1).get();
							next_token->expectType(TOKEN_INT);
							((TokenInt*)prev_token)->value -= ((TokenInt*)next_token)->value;
						}
						tokens.erase(tokens.cbegin() + i, tokens.cbegin() + (i + 2));
						break;
					}
					prev_token = token;
				}

#if DEBUG_TOKENS
				if (tokens.size() != pre_squash_size)
				{
					printTokens("Tokens after squashing round 2", tokens);
				}
#endif
			} while (tokens.size() != pre_squash_size);
		}
		// Squash Round 3
		{
			size_t pre_squash_size;
			do
			{
				pre_squash_size = tokens.size();
				for (size_t i = 0; i < tokens.size(); i++)
				{
					Token* const token = tokens.at(i).get();
					switch (token->type)
					{
					case TOKEN_STRING:
						if (i + 1 != tokens.size())
						{
							Token* const next_token = tokens.at(i + 1).get();
							if (next_token->type == TOKEN_INT)
							{
								((TokenString*)token)->value.append(std::to_string(((TokenInt*)next_token)->value));
								tokens.erase(tokens.cbegin() + (i + 1));
							}
							else if (next_token->type == TOKEN_STRING)
							{
								((TokenString*)token)->value.append(((TokenString*)next_token)->value);
								tokens.erase(tokens.cbegin() + (i + 1));
							}
						}
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

		// Assemble
		Program p{};
		std::unordered_map<std::string, size_t> variable_map{};

		// Assemble Round 1: Variables
		{
			Token* prev_token = nullptr;
			for (auto i = tokens.cbegin(); i != tokens.cend(); i++)
			{
				Token* const token = i->get();
				switch (token->type)
				{
				case TOKEN_ASSIGNMENT:
					if (prev_token == nullptr || i + 1 == tokens.cend())
					{
						token->throwUnexpected();
					}
					prev_token->expectType(TOKEN_LITERAL);
					{
						auto& var_name = ((TokenLiteral*)prev_token)->literal;
						auto var_map_entry = variable_map.find(var_name);
						if (var_map_entry == variable_map.end())
						{
							variable_map.emplace(std::move(var_name), p.variables.size());
							p.variables.emplace_back(tokenToData((++i)->get()));
						}
						else
						{
							p.variables.at(var_map_entry->second) = tokenToData((++i)->get());
						}
					}
					break;
				}
				prev_token = token;
			}
		}
		// Assemble Round 2: Echo
		for (auto i = tokens.cbegin(); i != tokens.cend(); i++)
		{
			Token* const token = i->get();
			switch (token->type)
			{
			case TOKEN_LITERAL:
			{
				auto& literal = ((TokenLiteral*)token)->literal;
				if (literal == "echo")
				{
					if (i + 1 == tokens.cend())
					{
						token->throwUnexpected();
					}
					Token* const next_token = (++i)->get();
					p.ops.emplace_back(OP_ECHO);
					if (next_token->type == TOKEN_LITERAL)
					{
						auto var_map_entry = variable_map.find(((TokenLiteral*)next_token)->literal);
						if (var_map_entry == variable_map.end())
						{
							next_token->loc.throwHere(std::move(std::string("Unknown variable '").append(((TokenLiteral*)next_token)->literal).append(1, '\'')));
						}
						p.ops.emplace_back(var_map_entry->second);
					}
					else
					{
						p.ops.emplace_back(p.variables.size());
						p.variables.emplace_back(tokenToData(next_token));
					}
				}
				break;
			}
			}
		}

		return p;
	}
#pragma clang diagnostic pop

	Program Program::fromFile(std::string&& path)
	{
		std::ifstream file(path);
		file.seekg(0, std::ios::end);
		auto size = (size_t)file.tellg();
		std::string buffer(size, ' ');
		file.seekg(0);
		file.read(&buffer[0], size);

		return Program::fromString(std::move(path), buffer);
	}

	void Program::execute() const
	{
		size_t ip = 0;
		while (ip < ops.size())
		{
			switch (ops.at(ip++))
			{
			case OP_ECHO:
				OpEcho op{};
				op.execute(*this, { ops.at(ip++) });
				break;
			}
		}
	}
}
