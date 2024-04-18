#pragma once

#include "SyntaxNode.h"

class VariableDefinitionSyntaxNode : public SyntaxNode
{
public:
	VariableDefinitionSyntaxNode(
		Token* name,
		Type* dataType,
		ValueSyntaxNode* initialValue,
		bool isConstant,
		bool isClassMember
	) :
		name_(name),
		dataType_(dataType),
		initialValue_(initialValue),
		isConstant_(isConstant),
		isClassMember_(isClassMember)
	{
	}

	Type* getType()
	{
		return dataType_;
	}

	std::string getName() const
	{
		return name_->value;
	}

	bool isPrivate() const
	{
		return name_->value[0] == '_';
	}

	void hoist(CppData* data) override
	{
		data->variableDefinitions[name_->value] = this;
		if (initialValue_) initialValue_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		if (initialValue_) initialValue_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		if (initialValue_)
		{
			initialValue_->resolveTypes(data);

			if (!dataType_) dataType_ = initialValue_->getType();
		}
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code;

		if (isConstant_) code += "const ";

		if (!dataType_)
		{
			if (initialValue_)
			{
				auto valueType = initialValue_->getType();
				code += data->toCppType(valueType);
			}
			else code += "Variant";
		}
		else code += data->toCppType(dataType_);
		code += " " + name_->value;

		if (initialValue_) code += " = " + initialValue_->toCpp(data, indents);

		return code;
	}

private:
	Token* name_;
	Type* dataType_;
	ValueSyntaxNode* initialValue_;
	bool isConstant_;
	bool isClassMember_;
};