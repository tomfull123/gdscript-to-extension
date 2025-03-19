#pragma once

#include "SyntaxNode.h"

class PreloadSyntaxNode : public ValueSyntaxNode
{
public:
	explicit PreloadSyntaxNode(Token* path) :
		path_(path),
		type_(new Type("PackedScene"))
	{
	}

	Type* getType() override
	{
		return type_;
	}

	std::string getName() override
	{
		return "";
	}

	void hoist(CppData* data) override
	{
	}

	void resolveDefinitions(CppData* data) override
	{
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		return data->currentClass->toCppType(new Type("ResourceLoader")) + "->load(\"" + path_->value + "\")";
	}

private:
	Token* path_;
	Type* type_;
};
