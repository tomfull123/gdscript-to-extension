#pragma once

#include "FunctionDefinitionSyntaxNode.h"
#include "VariableDefinitionSyntaxNode.h"
#include "EnumDefinitionSyntaxNode.h"

class ClassDefinitionSyntaxNode : public SyntaxNode
{
public:
	ClassDefinitionSyntaxNode(
		Token* name,
		Token* extends,
		const std::vector<FunctionDefinitionSyntaxNode*>& memberFunctionDefinitions,
		const std::vector<VariableDefinitionSyntaxNode*>& memberVariableDefinitions,
		const std::vector<EnumDefinitionSyntaxNode*>& enumDefinitions,
		const std::vector<FunctionDefinitionSyntaxNode*>& staticFunctionDefinitions,
		const std::vector<VariableDefinitionSyntaxNode*>& staticVariableDefinitions,
		const std::vector<ClassDefinitionSyntaxNode*>& innerClasses,
		bool isInnerClass
	) :
		name_(name),
		extends_(extends),
		memberFunctionDefinitions_(memberFunctionDefinitions),
		memberVariableDefinitions_(memberVariableDefinitions),
		enumDefinitions_(enumDefinitions),
		staticFunctionDefinitions_(staticFunctionDefinitions),
		staticVariableDefinitions_(staticVariableDefinitions),
		innerClasses_(innerClasses),
		isInnerClass_(isInnerClass)
	{}

	std::string getName(CppData* data) const
	{
		std::string className;

		if (name_)
		{
			className = name_->value;
			if (className[0] == '_') className.erase(0, 1);
		}
		else
		{
			auto& fileName = data->fileName;
			auto firstLetter = (char)std::toupper(fileName[0]);
			className = firstLetter + fileName.substr(1);
		}

		return className;
	}

	void hoist(CppData* data) override
	{
		data->currentClassName = getName(data);
		for (auto enumDef : enumDefinitions_) enumDef->hoist(data);
		for (auto v : staticVariableDefinitions_) v->hoist(data);
		for (auto f : staticFunctionDefinitions_) f->hoist(data);
		for (auto v : memberVariableDefinitions_) v->hoist(data);
		for (auto f : memberFunctionDefinitions_) f->hoist(data);
		for (auto c : innerClasses_) c->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		for (auto enumDef : enumDefinitions_) enumDef->resolveDefinitions(data);
		for (auto v : staticVariableDefinitions_) v->resolveDefinitions(data);
		for (auto f : staticFunctionDefinitions_) f->resolveDefinitions(data);
		for (auto v : memberVariableDefinitions_) v->resolveDefinitions(data);
		for (auto f : memberFunctionDefinitions_) f->resolveDefinitions(data);
		for (auto c : innerClasses_) c->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data) override
	{
		for (auto enumDef : enumDefinitions_) enumDef->resolveTypes(data);
		for (auto v : staticVariableDefinitions_) v->resolveTypes(data);
		for (auto f : staticFunctionDefinitions_) f->resolveTypes(data);
		for (auto v : memberVariableDefinitions_) v->resolveTypes(data);
		for (auto f : memberFunctionDefinitions_) f->resolveTypes(data);
		for (auto c : innerClasses_) c->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::vector<std::string> enumNames;

		for (auto enumDef : enumDefinitions_)
		{
			enumNames.push_back(enumDef->getName());
		}

		auto classBodyString = classBody(data);

		if (isInnerClass_) return classBodyString;

		return "#pragma once\n\n"
			+ cppIncludes(data) +
			"namespace godot\n"
			"{\n"
			+ classBodyString +
			"}\n"
			+ enumCasts(enumNames);
	}

private:
	Token* name_;
	Token* extends_;
	std::vector<FunctionDefinitionSyntaxNode*> memberFunctionDefinitions_;
	std::vector<VariableDefinitionSyntaxNode*> memberVariableDefinitions_;
	std::vector<EnumDefinitionSyntaxNode*> enumDefinitions_;
	std::vector<FunctionDefinitionSyntaxNode*> staticFunctionDefinitions_;
	std::vector<VariableDefinitionSyntaxNode*> staticVariableDefinitions_;
	std::vector<ClassDefinitionSyntaxNode*> innerClasses_;
	bool isInnerClass_;

	std::string classBody(CppData* data) const
	{
		std::string className = getName(data);

		std::string staticVariableDefinitionString;

		for (auto v : staticVariableDefinitions_)
			staticVariableDefinitionString += "\t" + v->toCpp(data, "\t\t") + ";\n";

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
				bindStaticMethodsString += bindMethod(className, f, "\t\t\t", true);
			}
		}

		std::string enumDefString;

		for (auto enumDef : enumDefinitions_)
		{
			enumDefString += "\t" + enumDef->toCpp(data, "\t");
		}

		std::string innerClassesString;

		for (auto innerClass : innerClasses_)
		{
			innerClassesString += innerClass->toCpp(data, "\t") + "\n";
		}

		std::string privateMemberVariableDefinitionString;
		std::string publicMemberVariableDefinitionString;

		for (auto v : memberVariableDefinitions_)
		{
			if (v->isPrivate()) privateMemberVariableDefinitionString += "\t\t" + v->toCpp(data, "\t\t") + ";\n";
			else publicMemberVariableDefinitionString += "\t\t" + v->toCpp(data, "\t\t") + ";\n";
		}

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
				bindMethodsString += bindMethod(className, f, "\t\t\t", false);
			}
		}

		Type* inherits;

		if (extends_) inherits = new Type(extends_->value);
		else inherits = new Type("RefCounted");

		data->toCppType(inherits);

		return ""
			+ enumDefString
			+ innerClassesString
			+ staticVariableDefinitionString +
			"\tclass " + className + " : public " + inherits->name + "\n"
			"\t{\n"
			"\t\tGDCLASS(" + className + ", " + inherits->name + ")\n"
			"\tpublic:\n"
			+ publicMemberFunctionDefinitionString
			+ publicStaticFunctionDefinitionString
			+ publicMemberVariableDefinitionString +
			"\tprivate:\n"
			+ privateMemberVariableDefinitionString +
			"\n"
			+ privateMemberFunctionDefinitionString
			+ privateStaticFunctionDefinitionString +
			"\tprotected:\n"
			"\t\tstatic void _bind_methods()\n"
			"\t\t{\n"
			+ bindMethodsString
			+ bindStaticMethodsString +
			"\t\t}\n"
			"\t};\n";
	}

	std::string cppIncludes(const CppData* data) const
	{
		const auto& types = data->types;

		std::string code = "";

		if (!types.empty())
		{
			std::unordered_set<std::string> includes;
			for (const auto& type : types)
			{
				auto include = data->getIncludePath(type);
				if (include != "") includes.emplace(include);
			}

			for (const auto& include : includes)
			{
				code += "#include " + include + "\n";
			}

			code += "\n";
		}

		return code;
	}

	std::string bindMethod(const std::string& className, const FunctionDefinitionSyntaxNode* functionDefinition, const std::string& indents, bool isStatic) const
	{
		std::string functionName = functionDefinition->getName();
		std::vector<VariableDefinitionSyntaxNode*> argDefs = functionDefinition->getArgDefs();

		std::string argsString;

		for (auto argDef : argDefs) argsString += ", \"" + argDef->getName() + "\"";

		std::string code = indents;

		if (isStatic) code += "ClassDB::bind_static_method(\"" + className + "\", ";
		else code += "ClassDB::bind_method(";

		return code + "D_METHOD(\"" + functionName + "\"" + argsString + "), "
			"&" + className + "::" + functionName + ");\n";
	}

	std::string bindProperty(const std::string& className) const
	{
		return "ClassDB::add_property(\"" + className + "\", PropertyInfo(Variant::INT, \"type\"), \"\", \"_getType\");\n";
	}

	std::string enumCasts(const std::vector<std::string>& enumNames) const
	{
		std::string enumCasts;

		for (const auto& enumName : enumNames)
		{
			enumCasts += "VARIANT_ENUM_CAST(" + enumName + ");\n";
		}

		return enumCasts;
	}
};