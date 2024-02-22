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

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string code = indents;

		bool isConstructorCall = name_->value == "new";

		if (instance_)
		{
			if (isConstructorCall)
			{
				code += "Ref<";
			}

			code += instance_->toCpp(data, indents);

			if (!isConstructorCall) code += "->";
			else code += ">";
		}

		std::string argsString;

		for (int a = 0; a < args_.size(); a++)
		{
			auto arg = args_[a];
			argsString += arg->toCpp(data, "");
			if (a < args_.size() - 1) argsString += ", ";
		}

		if (!isConstructorCall)
		{
			code += name_->value;
		}

		code += "(" + argsString + ")";

		return code;
	}
private:
	ValueSyntaxNode* instance_;
	Token* name_;
	std::vector<ValueSyntaxNode*> args_;
};
