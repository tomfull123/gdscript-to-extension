#pragma once

class TokenStream
{
public:
	int getLastTokenLineNumber() const
	{
		return lastTokenLineNumber_;
	}

	int getLastTokenColumnNumber() const
	{
		return lastTokenColumnNumber_;
	}

protected:
	int lastTokenLineNumber_ = 0;
	int lastTokenColumnNumber_ = 0;
};