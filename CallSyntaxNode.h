#pragma once

#include "SyntaxNode.h"

class CallSyntaxNode : public ValueSyntaxNode
{
public:
	CallSyntaxNode(
		ValueSyntaxNode* instance,
		Token* name,
		const std::vector<ValueSyntaxNode*>& args
	) :
		instance_(instance),
		name_(name),
		args_(args)
	{}

	Type* getType() override
	{
		return nullptr;
	}

	void resolveType() override
	{
		instance_->resolveType();
		for (auto a : args_) a->resolveType();
	}

	std::string toCpp(CppData* data) override
	{
		return "";
	}
private:
	ValueSyntaxNode* instance_;
	Token* name_;
	std::vector<ValueSyntaxNode*> args_;
};
