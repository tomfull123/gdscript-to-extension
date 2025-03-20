#pragma once

#include "GDLexer.h"
#include "TokenStream.h"

class GDTokenStream : public TokenStream
{
public:
	explicit GDTokenStream(const std::vector<GDToken*>& tokens) :
		tokens_(tokens)
	{
		if (!tokens.empty())
		{
			const auto* lastToken = tokens.back();
			lastTokenColumnNumber_ = lastToken->columnNumber;
			lastTokenLineNumber_ = lastToken->lineNumber;
		}
	}

	GDToken* next()
	{
		if (tokens_.empty()) return nullptr;
		GDToken* token = tokens_.front();
		tokens_.erase(tokens_.begin(), tokens_.begin() + 1);
		//std::cout << "parsing token: " << token->value << std::endl;
		return token;
	}

	GDToken* peek(unsigned int offset = 0)
	{
		if (tokens_.empty()) return nullptr;
		return tokens_[offset];
	}

	bool end() const
	{
		return tokens_.empty();
	}

	size_t getLength() const
	{
		return tokens_.size();
	}

private:
	std::vector<GDToken*> tokens_;
};