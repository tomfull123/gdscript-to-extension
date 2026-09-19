#pragma once

#include "SyntaxNode.h"
#include "VariableSyntaxNode.h"

class ValueIndexValue : public ValueSyntaxNode
{
public:
	ValueIndexValue(
		ValueSyntaxNode* variable,
		ValueSyntaxNode* index,
		bool asValue
	) :
		variable_(variable),
		index_(index),
		asValue_(asValue)
	{
	}

	Type* getType() override
	{
		auto varType = variable_->getType();
		if (varType) return varType->subtype(varType->subtypes.size() - 1);
		return nullptr;
	}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
		variable_->hoist(data);
		index_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		variable_->resolveDefinitions(data);
		index_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		variable_->resolveTypes(data);
		index_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents, bool asValue) override
	{
		auto arrayType = variable_->getType();
		auto lastSubtypeIndex = arrayType->subtypes.size() - 1;
		auto elementType = arrayType->subtype(lastSubtypeIndex);
		bool shouldExplicitlyCast = asValue_ && elementType;

		std::string code;

		if (shouldExplicitlyCast) code += "((" + data->toCppType(elementType) + ")";

		code += variable_->toCpp(data, indents, true) + "[" + index_->toCpp(data, "", true) + "]";

		if (shouldExplicitlyCast) code += ")";

		return code;
	}

private:
	ValueSyntaxNode* variable_;
	ValueSyntaxNode* index_;
	bool asValue_;
};
