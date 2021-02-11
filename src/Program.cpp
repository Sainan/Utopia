#include "Program.hpp"

#include <fstream>
#include <optional>
#include <stdexcept>
#include <string>

#include "DataString.hpp"
#include "OpCode.hpp"
#include "OpEcho.hpp"
#include "Token.hpp"
#include "TokenLiteral.hpp"
#include "TokenString.hpp"

namespace Utopia
{
	static size_t line_num;
	static std::optional<std::string> str;

	static void processWord(std::vector<std::unique_ptr<Token>>& tokens, std::string&& word)
	{
		if(str.has_value())
		{
			str.value().append(1, ' ');
			if (!word.empty())
			{
				if (word.at(word.length() - 1) == '\"')
				{
					word.pop_back();
					str.value().append(std::move(word));
					tokens.emplace_back(std::make_unique<TokenString>(std::move(str.value())));
					str = std::nullopt;
				}
				else
				{
					str.value().append(std::move(word));
				}
			}
		}
		else if (!word.empty())
		{
			if (word.at(0) == '\"')
			{
				word.erase(0, 1);
				if (word.at(word.length() - 1) == '\"')
				{
					word.pop_back();
					tokens.emplace_back(std::make_unique<TokenString>(std::move(word)));
				}
				else
				{
					str = std::move(word);
				}
			}
			else
			{
				tokens.emplace_back(std::make_unique<TokenLiteral>(std::move(word)));
			}
		}
	}

	static void processLine(std::vector<std::unique_ptr<Token>>& tokens, std::string&& line)
	{
		line_num++;
		if (line.empty())
		{
			return;
		}
		if (line.at(line.length() - 1) == '\r')
		{
			line.pop_back();
		}
		size_t start = 0;
		while (true)
		{
			size_t delim = line.find(" ", start);
			if (delim == std::string::npos)
			{
				break;
			}
			processWord(tokens, line.substr(start, delim - start));
			start = delim + 1;
		}
		processWord(tokens, line.substr(start));

		if (str.has_value())
		{
			throw std::runtime_error(std::string("Unterminated string on line ").append(std::to_string(line_num)));
		}
	}

	Program Program::fromString(const std::string& code)
	{
		line_num = 0;
		str = std::nullopt;

		std::vector<std::unique_ptr<Token>> tokens{};
		size_t start = 0;
		while (true)
		{
			size_t delim = code.find("\n", start);
			if (delim == std::string::npos)
			{
				break;
			}
			processLine(tokens, code.substr(start, delim - start));
			start = delim + 1;
		}
		processLine(tokens, code.substr(start));

		Program p{};
		size_t i = 0;
		while (i < tokens.size())
		{
			auto* const token = tokens.at(i++).get();
			switch (token->type)
			{
			case TOKEN_LITERAL:
				{
					auto& literal = ((TokenLiteral*)token)->literal;
					if (literal == "echo")
					{
						p.ops.emplace_back(OP_ECHO);
						p.ops.emplace_back(p.data.size());
					}
				}
				break;

			case TOKEN_STRING:
				p.data.emplace_back(std::make_unique<DataString>(std::move(((TokenString*)token)->value)));
				break;
			}
		}
		return p;
	}

	Program Program::fromFile(const std::string& path)
	{
		std::ifstream file(path);
		file.seekg(0, std::ios::end);
		auto size = (size_t)file.tellg();
		std::string buffer(size, ' ');
		file.seekg(0);
		file.read(&buffer[0], size);

		return Program::fromString(buffer);
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
