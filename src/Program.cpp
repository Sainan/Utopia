#include "Program.hpp"

#define DEBUG_TOKENS false

#include <fstream>
#if DEBUG_TOKENS
#include <iostream>
#endif
#include <optional>
#include <string>

#include "DataString.hpp"
#include "ParseError.hpp"
#include "OpCode.hpp"
#include "OpEcho.hpp"
#include "TokenDivide.hpp"
#include "TokenInt.hpp"
#include "TokenLiteral.hpp"
#include "TokenMinus.hpp"
#include "TokenMultiply.hpp"
#include "TokenPlus.hpp"
#include "TokenString.hpp"

namespace Utopia
{
	static void processWord(std::vector<std::unique_ptr<Token>>& tokens, SourceLocation& loc, std::optional<std::unique_ptr<TokenString>>& str, std::string&& word)
	{
		if(str.has_value())
		{
			str.value()->value.append(1, ' ');
			if (!word.empty())
			{
				if (word.at(word.length() - 1) == '\"')
				{
					word.pop_back();
					str.value()->value.append(std::move(word));
					tokens.emplace_back(std::move(str.value()));
					str = std::nullopt;
				}
				else
				{
					str.value()->value.append(std::move(word));
				}
			}
		}
		else if (!word.empty())
		{
			if (word.at(0) == '\"')
			{
				word.erase(0, 1);
				if (!word.empty() && word.at(word.length() - 1) == '\"')
				{
					word.pop_back();
					tokens.emplace_back(std::make_unique<TokenString>(loc, std::move(word)));
				}
				else
				{
					str = std::make_unique<TokenString>(loc, std::move(word));
				}
			}
			else if (word == "+")
			{
				tokens.emplace_back(std::make_unique<TokenPlus>(loc));
			}
			else if (word == "-")
			{
				tokens.emplace_back(std::make_unique<TokenMinus>(loc));
			}
			else if (word == "*")
			{
				tokens.emplace_back(std::make_unique<TokenMultiply>(loc));
			}
			else if (word == "/")
			{
				tokens.emplace_back(std::make_unique<TokenDivide>(loc));
			}
			else
			{
				try
				{
					long long value = std::stoll(word);
					tokens.emplace_back(std::make_unique<TokenInt>(loc, value));
				}
				catch (...)
				{
					tokens.emplace_back(std::make_unique<TokenLiteral>(loc, std::move(word)));
				}
			}
		}
	}

	static void processLine(std::vector<std::unique_ptr<Token>>& tokens, SourceLocation& loc, std::optional<std::unique_ptr<TokenString>>& str, std::string&& line)
	{
		loc.line++;
		if (line.empty())
		{
			return;
		}
		if (line.at(line.length() - 1) == '\r')
		{
			line.pop_back();
		}
		loc.colon = 1;
		size_t start = 0;
		while (true)
		{
			size_t delim = line.find(" ", start);
			if (delim == std::string::npos)
			{
				break;
			}
			processWord(tokens, loc, str, line.substr(start, delim - start));
			start = delim + 1;
			loc.colon = delim + 2;
		}
		{
			auto remainder = line.substr(start);
			if (!remainder.empty())
			{
				size_t remainder_size = remainder.size();
				processWord(tokens, loc, str, std::move(remainder));
				loc.colon += remainder_size;
			}
		}

		if (str.has_value())
		{
			throw ParseError(std::string("Unterminated string in ").append(loc.toString()));
		}
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
	Program Program::fromString(std::string&& name, const std::string& code)
	{
		std::vector<std::unique_ptr<Token>> tokens{};

		// Tokenize
		{
			std::optional<std::unique_ptr<TokenString>> str = std::nullopt;
			SourceLocation loc(std::move(name));
			size_t start = 0;
			while (true)
			{
				size_t delim = code.find("\n", start);
				if (delim == std::string::npos)
				{
					break;
				}
				processLine(tokens, loc, str, code.substr(start, delim - start));
				start = delim + 1;
			}
			processLine(tokens, loc, str, code.substr(start));
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

		// Assemble
		Program p{};
		{
			for (size_t i = 0; i < tokens.size(); i++)
			{
				Token* const token = tokens.at(i).get();
				switch (token->type)
				{
				default:
					token->throwUnexpected();
					break;

				case TOKEN_LITERAL:
				{
					auto& literal = ((TokenLiteral*)token)->literal;
					if (literal == "echo")
					{
						p.ops.emplace_back(OP_ECHO);
						p.ops.emplace_back(p.data.size());
						if (i + 1 == tokens.size())
						{
							token->throwUnexpected();
						}
						Token* const next_token = tokens.at(++i).get();
						if (next_token->type == TOKEN_INT)
						{
							p.data.emplace_back(std::make_unique<DataString>(std::to_string(((TokenInt*)next_token)->value)));
						}
						else
						{
							p.data.emplace_back(std::make_unique<DataString>(std::move(((TokenString*)next_token)->value)));
						}
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
