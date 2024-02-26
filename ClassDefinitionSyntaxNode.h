#pragma once

#include "FunctionDefinitionSyntaxNode.h"
#include "VariableDefinitionSyntaxNode.h"
#include "EnumDefinitionSyntaxNode.h"

class ClassDefinitionSyntaxNode : public SyntaxNode
{
public:
	ClassDefinitionSyntaxNode(
		Token* name,
		const std::vector<FunctionDefinitionSyntaxNode*>& memberFunctionDefinitions,
		const std::vector<VariableDefinitionSyntaxNode*>& memberVariableDefinitions,
		const std::vector<EnumDefinitionSyntaxNode*>& enumDefinitions,
		const std::vector<FunctionDefinitionSyntaxNode*>& staticFunctionDefinitions,
		const std::vector<VariableDefinitionSyntaxNode*>& staticVariableDefinitions
	) :
		name_(name),
		memberFunctionDefinitions_(memberFunctionDefinitions),
		memberVariableDefinitions_(memberVariableDefinitions),
		enumDefinitions_(enumDefinitions),
		staticFunctionDefinitions_(staticFunctionDefinitions),
		staticVariableDefinitions_(staticVariableDefinitions)
	{}

	void hoist(CppData* data) override
	{
		for (auto enumDef : enumDefinitions_) enumDef->hoist(data);
		for (auto v : staticVariableDefinitions_) v->hoist(data);
		for (auto f : staticFunctionDefinitions_) f->hoist(data);
		for (auto v : memberVariableDefinitions_) v->hoist(data);
		for (auto f : memberFunctionDefinitions_) f->hoist(data);
	}

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

		std::string staticVariableDefinitionString;

		for (auto v : staticVariableDefinitions_)
			staticVariableDefinitionString += "\t\t" + v->toCpp(data, "\t\t") + ";\n";

		std::string publicStaticFunctionDefinitionString;
		std::string privateStaticFunctionDefinitionString;
		std::string bindStaticMethodsString;

		for (auto f : staticFunctionDefinitions_)
		{
			std::string functionDefString = f->toCpp(data, "\t\t") + "\n";

			if (f->isPrivate()) privateStaticFunctionDefinitionString += functionDefString;
			else
			{
				publicStaticFunctionDefinitionString += functionDefString;
				bindStaticMethodsString += bindMethod(className, f->getName(), f->getArgDefs(), "\t\t\t");
			}
		}

		std::string enumDefString;
		std::vector<std::string> enumNames;

		for (auto enumDef : enumDefinitions_)
		{
			enumDefString += "\t" + enumDef->toCpp(data, "\t");
			enumNames.push_back(enumDef->getName());
		}

		std::string memberVariableDefinitionString;

		for (auto v : memberVariableDefinitions_)
			memberVariableDefinitionString += "\t\t" + v->toCpp(data, "\t\t") + ";\n";

		std::string publicMemberFunctionDefinitionString;
		std::string privateMemberFunctionDefinitionString;
		std::string bindMethodsString;

		for (auto f : memberFunctionDefinitions_)
		{
			std::string functionDefString = f->toCpp(data, "\t\t") + "\n";

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
			+ enumDefString +
			"\tclass " + className + " : public " + inherits->name + "\n"
			"\t{\n"
			"\t\tGDCLASS(" + className + ", " + inherits->name + ")\n"
			"\tpublic:\n"
			+ publicMemberFunctionDefinitionString
			+ publicStaticFunctionDefinitionString +
			"\tprivate:\n"
			+ memberVariableDefinitionString
			+ staticVariableDefinitionString +
			"\n"
			+ privateMemberFunctionDefinitionString
			+ privateStaticFunctionDefinitionString +
			"\tprotected:\n"
			"\t\tstatic void _bind_methods()\n"
			"\t\t{\n"
			+ bindMethodsString +
			"\t\t}\n"
			"\t};\n"
			"}\n"
			+ enumCasts(enumNames);
	}

private:
	Token* name_;
	std::vector<FunctionDefinitionSyntaxNode*> memberFunctionDefinitions_;
	std::vector<VariableDefinitionSyntaxNode*> memberVariableDefinitions_;
	std::vector<EnumDefinitionSyntaxNode*> enumDefinitions_;
	std::vector<FunctionDefinitionSyntaxNode*> staticFunctionDefinitions_;
	std::vector<VariableDefinitionSyntaxNode*> staticVariableDefinitions_;

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

	std::string enumCasts(const std::vector<std::string>& enumNames)
	{
		std::string enumCasts;

		for (const auto& enumName : enumNames)
		{
			enumCasts += "VARIANT_ENUM_CAST(" + enumName + ");\n";
		}

		return enumCasts;
	}
};