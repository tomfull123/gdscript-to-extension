#pragma once

#include "SyntaxNode.h"

class ExportRangeSyntaxNode
{
public:
	ExportRangeSyntaxNode(
		ValueSyntaxNode* min,
		ValueSyntaxNode* max,
		ValueSyntaxNode* step
	) :
		min_(min),
		max_(max),
		step_(step)
	{
	}

	std::string toCpp(CppData* data)
	{
		std::string code = "PROPERTY_HINT_RANGE, \"" + min_->toCpp(data, "") + "," + max_->toCpp(data, "");

		if (step_) code += "," + step_->toCpp(data, "");

		code += "\"";

		return code;
	}

private:
	ValueSyntaxNode* min_;
	ValueSyntaxNode* max_;
	ValueSyntaxNode* step_;
};