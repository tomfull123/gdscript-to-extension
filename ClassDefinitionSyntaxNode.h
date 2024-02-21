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

	void resolveType() override
	{
		for (auto v : memberVariableDefinitions_)
			v->resolveType();

		for (auto f : memberFunctionDefinitions_)
			f->resolveType();
	}

	std::string toCpp(CppData* data) override
	{
		std::string className = name_->value;

		std::string memberVariableDefinitionString;

		for (auto v : memberVariableDefinitions_)
			memberVariableDefinitionString += "\t\t" + v->toCpp(data);

		std::string memberFunctionDefinitionString;

		for (auto f : memberFunctionDefinitions_)
			memberFunctionDefinitionString += "\t\t" + f->toCpp(data);

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
			"\tprotected:\n"
			"\t\tstatic void _bind_methods()\n"
			"\t\t{\n"
			"\t\t\t\n"
			"\t\t}\n"
			"\t}\n"
			"}\n";
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
			for (const auto& type : types)
			{
				auto it = CPPTYPES_TO_INCLUDE_PATH.find(type);

				if (it == CPPTYPES_TO_INCLUDE_PATH.end()) continue;

				code += "#include " + it->second + "\n";
			}

			code += "\n";
		}

		return code;
	}
};