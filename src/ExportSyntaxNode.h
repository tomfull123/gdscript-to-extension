#pragma once

#include "SyntaxNode.h"

class ExportSyntaxNode
{
public:
	ExportSyntaxNode(
		GDToken* annotationToken,
		const std::vector<ValueSyntaxNode*>& args
	) :
		annotationToken_(annotationToken),
		args_(args)
	{
	}

	std::string toCpp(CppData* data)
	{
		std::string annotation = annotationToken_->value;
		if (annotation == "export_range")
		{
			ValueSyntaxNode* min = args_[0];
			ValueSyntaxNode* max = args_[1];
			ValueSyntaxNode* step = args_[2];

			std::string code = "PROPERTY_HINT_RANGE, \"" + min->toCpp(data, "", true) + "," + max->toCpp(data, "", true);

			if (step) code += "," + step->toCpp(data, "", true);

			code += "\"";

			return code;
		}
		else if (annotation == "export_enum")
		{
			std::string argStrings;

			for (int i = 0; i < args_.size(); i++)
			{
				auto arg = args_[i];
				argStrings += arg->toCpp(data, "", true);
				bool isLast = i == (args_.size() - 1);
				if (!isLast) argStrings += ", ";
			}

			return "PROPERTY_HINT_ENUM, \"" + argStrings + "\"";
		}

		return "PROPERTY_HINT_NONE, \"\"";
	}

private:
	GDToken* annotationToken_;
	std::vector<ValueSyntaxNode*> args_;
};