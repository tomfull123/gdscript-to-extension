#pragma once

#include "SyntaxNode.h"

class TernarySyntaxNode : public ValueSyntaxNode
{
public:
	TernarySyntaxNode(
		ValueSyntaxNode* condition,
		ValueSyntaxNode* thenValue,
		ValueSyntaxNode* elseValue
	) :
		condition_(condition),
		thenValue_(thenValue),
		elseValue_(elseValue)
	{
	}

	Type* getType() override
	{
		return thenValue_->getType();
	}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		condition_->hoist(data);
		thenValue_->hoist(data);
		elseValue_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		condition_->resolveDefinitions(data);
		thenValue_->resolveDefinitions(data);
		elseValue_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		condition_->resolveTypes(data);
		thenValue_->resolveTypes(data);
		elseValue_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents, bool asValue) override
	{
		return condition_->toCpp(data, "", true)
			+ " ? "
			+ thenValue_->toCpp(data, "", true)
			+ " : "
			+ elseValue_->toCpp(data, "", true);
	}

private:
	ValueSyntaxNode* condition_;
	ValueSyntaxNode* thenValue_;
	ValueSyntaxNode* elseValue_;
};