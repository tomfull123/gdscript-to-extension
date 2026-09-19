#pragma once

#include "SyntaxNode.h"
#include "ExportRangeSyntaxNode.h"

class FunctionDefinitionSyntaxNode;

class VariableDefinitionSyntaxNode : public SyntaxNode
{
public:
	VariableDefinitionSyntaxNode(
		Token* name,
		Type* dataType,
		ValueSyntaxNode* initialValue,
		bool isConstant,
		bool isClassMember,
		bool isStatic,
		bool exported,
		Token* getterName = nullptr,
		Token* setterName = nullptr,
		FunctionDefinitionSyntaxNode* getterFunctionDefinition = nullptr,
		FunctionDefinitionSyntaxNode* setterFunctionDefinition = nullptr,
		int exportGroupIndex = -1,
		int exportSubgroupIndex = -1,
		ExportRangeSyntaxNode* exportRange = nullptr
	) :
		name_(name),
		dataType_(dataType),
		initialValue_(initialValue),
		isConstant_(isConstant),
		isClassMember_(isClassMember),
		isStatic_(isStatic),
		exported_(exported),
		getterName_(getterName),
		setterName_(setterName),
		getterFunctionDefinition_(getterFunctionDefinition),
		setterFunctionDefinition_(setterFunctionDefinition),
		exportGroupIndex_(exportGroupIndex),
		exportSubgroupIndex_(exportSubgroupIndex),
		exportRange_(exportRange)
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

	bool isStatic() const
	{
		return isStatic_;
	}

	bool isPrivate() const
	{
		return name_->value[0] == '_';
	}

	bool isConstant() const
	{
		return isConstant_;
	}

	bool isExported() const
	{
		return exported_;
	}

	Token* getGetterName() const
	{
		return getterName_;
	}

	Token* getSetterName() const
	{
		return setterName_;
	}

	FunctionDefinitionSyntaxNode* getGetterFunctionDefinition() const
	{
		return getterFunctionDefinition_;
	}

	FunctionDefinitionSyntaxNode* getSetterFunctionDefinition() const
	{
		return setterFunctionDefinition_;
	}

	int getExportGroup() const
	{
		return exportGroupIndex_;
	}

	int getExportSubgroup() const
	{
		return exportSubgroupIndex_;
	}

	ExportRangeSyntaxNode* getExportRange() const
	{
		return exportRange_;
	}

	bool isTypeDef() const
	{
		return isClassMember_ && isConstant_ && initialValue_ && initialValue_->hasParent();
	}

	void hoist(CppData* data) override;

	void resolveDefinitions(CppData* data) override;

	void resolveTypes(CppData* data, Type* otherType = nullptr) override;

	std::string variableDeclarationCpp(CppData* data, const std::string& indents)
	{
		return variableCpp(data) + assignInitialValueCpp(data, indents);
	}

	std::string toCpp(CppData* data, const std::string& indents, bool asValue) override
	{
		if (isStatic_)
		{
			return indents + variableCpp(data, true) + "()\n"
				+ indents + "{\n"
				+ indents + "\t" + variableCpp(data) + assignInitialValueCpp(data, indents + "\t") + ";\n"
				+ indents + "\t" + "return " + name_->value + ";\n"
				+ indents + "}\n";
		}

		return variableCpp(data) + assignInitialValueCpp(data, indents);
	}

	void addTypeDef(CppData* data)
	{
		auto parent = initialValue_->getParent();
		if (parent)
		{
			data->toCppType(new Type(parent->getName()));
			data->currentClass->typeDefinitions.emplace(name_->value);
		}
	}

private:
	Token* name_;
	Type* dataType_;
	ValueSyntaxNode* initialValue_;
	bool isConstant_;
	bool isClassMember_;
	bool isStatic_;
	bool exported_;
	Token* getterName_;
	Token* setterName_;
	FunctionDefinitionSyntaxNode* getterFunctionDefinition_;
	FunctionDefinitionSyntaxNode* setterFunctionDefinition_;
	int exportGroupIndex_;
	int exportSubgroupIndex_;
	ExportRangeSyntaxNode* exportRange_;

	std::string variableCpp(CppData* data, bool ref = false)
	{
		std::string code;

		if (isStatic_) code += "static ";
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

		if (ref) code += "&";

		code += " ";
		code += name_->value;

		return code;
	}

	std::string assignInitialValueCpp(CppData* data, const std::string& indents)
	{
		if (initialValue_)
		{
			std::string code;

			code += " = ";

			code += data->castValue(dataType_, initialValue_->getType());

			code += initialValue_->toCpp(data, indents, true);

			return code;
		}
		return "";
	}
};