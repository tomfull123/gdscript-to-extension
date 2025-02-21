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
		bool isClassMember,
		bool isStatic
	) :
		name_(name),
		dataType_(dataType),
		initialValue_(initialValue),
		isConstant_(isConstant),
		isClassMember_(isClassMember),
		isStatic_(isStatic)
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

	bool isConstant() const
	{
		return isConstant_;
	}

	bool isTypeDef() const
	{
		return isClassMember_ && isConstant_ && initialValue_ && initialValue_->hasParent();
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

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		if (initialValue_)
		{
			initialValue_->resolveTypes(data);

			if (!dataType_) dataType_ = initialValue_->getType();
			else
			{
				auto initialValueType = initialValue_->getType();
				if (initialValueType && dataType_->subtypes.size() < initialValueType->subtypes.size())
				{
					dataType_ = initialValueType;
				}
			}
		}

		if (!dataType_) dataType_ = otherType;
	}

	std::string variableCpp(CppData* data, bool outsideClass = false)
	{
		std::string code;

		if (isStatic_ && !outsideClass) code += "static ";
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
		code += " ";
		if (outsideClass) code += data->currentClassName + "::";
		code += name_->value;

		return code;
	}

	std::string variableDeclarationCpp(CppData* data, const std::string& indents)
	{
		return variableCpp(data, true) + assignInitialValueCpp(data, indents);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return variableCpp(data) + assignInitialValueCpp(data, indents);
	}

	void addTypeDef(CppData* data)
	{
		auto parent = initialValue_->getParent();
		if (parent)
		{
			data->toCppType(new Type(parent->getName()));
			data->typeDefinitions.emplace(name_->value);
		}
	}

private:
	Token* name_;
	Type* dataType_;
	ValueSyntaxNode* initialValue_;
	bool isConstant_;
	bool isClassMember_;
	bool isStatic_;

	std::string assignInitialValueCpp(CppData* data, const std::string& indents)
	{
		if (initialValue_) return " = " + initialValue_->toCpp(data, indents);
		return "";
	}
};