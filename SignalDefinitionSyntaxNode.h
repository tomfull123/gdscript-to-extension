#pragma once

#include "SyntaxNode.h"

class SignalDefinitionSyntaxNode : public SyntaxNode
{
public:
	SignalDefinitionSyntaxNode(
		Token* name,
		const std::vector<VariableDefinitionSyntaxNode*>& args
	) :
		name_(name),
		args_(args)
	{}

	std::string toCpp(CppData* data, const std::string& indents)
	{
		std::string signalType = data->toCppType(new Type("Signal"));
		return indents + signalType + " " + name_->value + "()";
	}

private:
	Token* name_;
	std::vector<VariableDefinitionSyntaxNode*> args_;
};
