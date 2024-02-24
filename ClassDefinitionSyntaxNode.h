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
		std::string className;

		if (name_) className = name_->value;
		else
		{
			auto fileName = data->fileName;
			char firstLetter = std::toupper(fileName[0]);
			className = firstLetter + fileName.substr(1);
		}

		std::string memberVariableDefinitionString;

		for (auto v : memberVariableDefinitions_)
			memberVariableDefinitionString += indents + v->toCpp(data, "\t\t") + ";\n";

		std::string publicMemberFunctionDefinitionString;
		std::string privateMemberFunctionDefinitionString;
		std::string bindMethodsString;

		for (auto f : memberFunctionDefinitions_)
		{
			std::string functionDefString = indents + f->toCpp(data, "\t\t") + "\n";

			if (f->isPrivate()) privateMemberFunctionDefinitionString += functionDefString;
			else
			{
				publicMemberFunctionDefinitionString += functionDefString;
				bindMethodsString += bindMethod(className, f->getName(), f->getArgDefs(), "\t\t\t");
			}
		}

		Type* inherits = new Type("RefCounted");

		data->toCppType(inherits);

		return "#pragma once\n\n"
			+ cppIncludes(data) +
			"namespace godot\n"
			"{\n"
			"\tclass " + className + " : public " + inherits->name + "\n"
			"\t{\n"
			"\t\tGDCLASS(" + className + ", " + inherits->name + ")\n"
			"\tpublic:\n"
			+ publicMemberFunctionDefinitionString +
			"\tprivate:\n"
			+ memberVariableDefinitionString +
			"\n"
			+ privateMemberFunctionDefinitionString +
			"\n"
			"\tprotected:\n"
			"\t\tstatic void _bind_methods()\n"
			"\t\t{\n"
			+ bindMethodsString +
			"\t\t}\n"
			"\t};\n"
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

	std::string bindMethod(const std::string& className, const std::string& functionName, const std::vector<VariableDefinitionSyntaxNode*>& argDefs, const std::string& indents)
	{
		std::string argsString;

		for (auto argDef : argDefs) argsString += ", \"" + argDef->getName() + "\"";

		return indents + "ClassDB::bind_method("
			"D_METHOD(\"" + functionName + "\"" + argsString + "), "
			"&" + className + "::" + functionName + ");\n";
	}
};