#pragma once

#include "FunctionDefinitionSyntaxNode.h"
#include "VariableDefinitionSyntaxNode.h"

class ClassDefinitionSyntaxNode : public SyntaxNode
{
public:
	ClassDefinitionSyntaxNode(
		Token* name,
		const std::vector<FunctionDefinitionSyntaxNode*>& memberFunctionDefinitions,
		const std::vector<VariableDefinitionSyntaxNode*>& memberVariableDefinitions
	) :
		name_(name),
		memberFunctionDefinitions_(memberFunctionDefinitions),
		memberVariableDefinitions_(memberVariableDefinitions)
	{}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string className = name_->value;

		std::string memberVariableDefinitionString;

		for (auto v : memberVariableDefinitions_)
			memberVariableDefinitionString += indents + v->toCpp(data, "\t\t") + ";\n";

		std::string memberFunctionDefinitionString;

		for (auto f : memberFunctionDefinitions_)
			memberFunctionDefinitionString += indents + f->toCpp(data, "\t\t") + "\n";

		std::string includesCode = cppIncludes(data);

		return "#pragma once\n\n"
			+ includesCode +
			"namespace godot\n"
			"{\n"
			"\tclass " + className + "\n"
			"\t{\n"
			"\tpublic:\n"
			+ memberFunctionDefinitionString +
			"\tprivate:\n"
			+ memberVariableDefinitionString +
			"\n"
			"\tprotected:\n"
			"\t\tstatic void _bind_methods()\n"
			"\t\t{\n"
			"\t\t\t\n"
			"\t\t}\n"
			"\t}\n"
			"};\n";
	}

private:
	Token* name_;
	std::vector<FunctionDefinitionSyntaxNode*> memberFunctionDefinitions_;
	std::vector<VariableDefinitionSyntaxNode*> memberVariableDefinitions_;

	std::string cppIncludes(const CppData* data) const
	{
		const auto& types = data->types;

		std::string code = "";

		if (!types.empty())
		{
			std::unordered_set<std::string> includes;
			for (const auto& type : types)
			{
				auto it = CPPTYPES_TO_INCLUDE_PATH.find(type);

				if (it == CPPTYPES_TO_INCLUDE_PATH.end()) continue;

				includes.emplace(it->second);
			}

			for (const auto& include : includes)
			{
				code += "#include " + include + "\n";
			}

			code += "\n";
		}

		return code;
	}
};