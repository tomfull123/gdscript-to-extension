#pragma once

#include "XMLLexer.h"

class XMLTokenStream
{
public:
	explicit XMLTokenStream(const std::vector<XMLToken*>& tokens) :
		tokens_(tokens)
	{
		if (!tokens.empty())
		{
			const auto* lastToken = tokens.back();
			lastTokenColumnNumber_ = lastToken->columnNumber;
			lastTokenLineNumber_ = lastToken->lineNumber;
		}
	}

	XMLToken* next()
	{
		if (tokens_.empty()) return nullptr;
		XMLToken* token = tokens_.front();
		tokens_.erase(tokens_.begin(), tokens_.begin() + 1);
		//std::cout << "parsing token: " << token->value << std::endl;
		return token;
	}

	XMLToken* peek(unsigned int offset = 0)
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
	std::vector<XMLToken*> tokens_;
	int lastTokenLineNumber_ = 0;
	int lastTokenColumnNumber_ = 0;
};