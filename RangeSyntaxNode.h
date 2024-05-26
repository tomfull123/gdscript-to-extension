#pragma once

#pragma once

#include "SyntaxNode.h"

class RangeSyntaxNode : public ValueSyntaxNode
{
public:
	RangeSyntaxNode(
		ValueSyntaxNode* startValue,
		ValueSyntaxNode* endValue,
		ValueSyntaxNode* incrementValue
	) :
		startValue_(startValue),
		endValue_(endValue),
		incrementValue_(incrementValue)
	{}

	ValueSyntaxNode* getStartValue() { return startValue_; }
	ValueSyntaxNode* getEndValue() { return endValue_; }
	ValueSyntaxNode* getIncrementValue() { return incrementValue_; }

	Type* getType() override
	{
		return nullptr;
	}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		startValue_->hoist(data);
		endValue_->hoist(data);
		if (incrementValue_) incrementValue_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		startValue_->resolveDefinitions(data);
		endValue_->resolveDefinitions(data);
		if (incrementValue_) incrementValue_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType) override
	{
		startValue_->resolveTypes(data);
		endValue_->resolveTypes(data);
		if (incrementValue_) incrementValue_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return "";
	}

private:
	ValueSyntaxNode* startValue_;
	ValueSyntaxNode* endValue_;
	ValueSyntaxNode* incrementValue_;
};