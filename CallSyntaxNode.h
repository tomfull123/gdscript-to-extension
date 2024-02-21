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

	std::string toCpp(CppData* data) override
	{
		std::string code;

		if (instance_) code += instance_->toCpp(data) + ".";

		std::string argsString;

		for (int a = 0; a < args_.size(); a++)
		{
			auto arg = args_[a];
			argsString += arg->toCpp(data);
			if (a < args_.size() - 1) argsString += ", ";
		}

		code += name_->value + "(" + argsString + ")";

		return code;
	}
private:
	ValueSyntaxNode* instance_;
	Token* name_;
	std::vector<ValueSyntaxNode*> args_;
};
