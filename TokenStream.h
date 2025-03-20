#pragma once

#include "GDLexer.h"

class TokenStream
{
public:
	explicit TokenStream(const std::vector<Token*>& tokens) :
		tokens_(tokens)
	{
		if (!tokens.empty())
		{
			const auto* lastToken = tokens.back();
			lastTokenColumnNumber_ = lastToken->columnNumber;
			lastTokenLineNumber_ = lastToken->lineNumber;
		}
	}

	Token* next()
	{
		if (tokens_.empty()) return nullptr;
		Token* token = tokens_.front();
		tokens_.erase(tokens_.begin(), tokens_.begin() + 1);
		//std::cout << "parsing token: " << token->value << std::endl;
		return token;
	}

	Token* peek(unsigned int offset = 0)
	{
		if (tokens_.empty()) return nullptr;
		return tokens_[offset];
	}

	bool end() const
	{
		return tokens_.empty();
	}

	int getLastTokenLineNumber() const
	{
		return lastTokenLineNumber_;
	}

	int getLastTokenColumnNumber() const
	{
		return lastTokenColumnNumber_;
	}

	size_t getLength() const
	{
		return tokens_.size();
	}

private:
	std::vector<Token*> tokens_;
	int lastTokenLineNumber_ = 0;
	int lastTokenColumnNumber_ = 0;
};