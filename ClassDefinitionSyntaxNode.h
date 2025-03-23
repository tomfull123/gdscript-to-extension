#pragma once

#include "FunctionDefinitionSyntaxNode.h"
#include "VariableDefinitionSyntaxNode.h"
#include "EnumDefinitionSyntaxNode.h"
#include "ReturnSyntaxNode.h"
#include "VariableSyntaxNode.h"
#include "AssignmentSyntaxNode.h"
#include "BodySyntaxNode.h"
#include <algorithm>

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
		bool isInnerClass,
		const std::string& fileName
	) :
		name_(name),
		extends_(extends),
		memberFunctionDefinitions_(memberFunctionDefinitions),
		memberVariableDefinitions_(memberVariableDefinitions),
		enumDefinitions_(enumDefinitions),
		staticFunctionDefinitions_(staticFunctionDefinitions),
		staticVariableDefinitions_(staticVariableDefinitions),
		innerClasses_(innerClasses),
		isInnerClass_(isInnerClass),
		fileName_(fileName)
	{
	}

	Token* getNameToken()
	{
		return name_;
	}

	std::string getName() const
	{
		std::string className;

		if (name_)
		{
			className = name_->value;
			if (className[0] == '_') className.erase(0, 1);
		}
		else
		{
			auto& fileName = fileName_;
			auto firstLetter = (char)std::toupper(fileName[0]);
			className = firstLetter + fileName.substr(1);
		}

		return className;
	}

	void hoist(CppData* data) override
	{
		data->currentClass = new CppClassData();
		data->currentClass->currentClassName = getName();
		data->currentClass->classInheritedType = getInheritedType();
		if (extends_) data->inheritTypes[getName()] = extends_->value;
		for (auto enumDef : enumDefinitions_) enumDef->hoist(data);
		for (auto v : staticVariableDefinitions_) v->hoist(data);
		for (auto f : staticFunctionDefinitions_) f->hoist(data);
		for (auto v : memberVariableDefinitions_) v->hoist(data);
		for (auto f : memberFunctionDefinitions_) f->hoist(data);
		for (auto c : innerClasses_) c->hoist(data);
		data->classData[getName()] = data->currentClass;
	}

	void resolveDefinitions(CppData* data) override
	{
		setCurrentClass(data);

		for (auto enumDef : enumDefinitions_) enumDef->resolveDefinitions(data);
		for (auto v : staticVariableDefinitions_) v->resolveDefinitions(data);
		for (auto f : staticFunctionDefinitions_) f->resolveDefinitions(data);
		for (auto v : memberVariableDefinitions_) v->resolveDefinitions(data);
		for (auto f : memberFunctionDefinitions_) f->resolveDefinitions(data);
		for (auto c : innerClasses_) c->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		setCurrentClass(data);

		for (auto enumDef : enumDefinitions_) enumDef->resolveTypes(data);
		for (auto v : staticVariableDefinitions_) v->resolveTypes(data);
		for (auto f : staticFunctionDefinitions_) f->resolveTypes(data);
		for (auto v : memberVariableDefinitions_) v->resolveTypes(data);
		for (auto f : memberFunctionDefinitions_) f->resolveTypes(data);
		for (auto c : innerClasses_) c->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		setCurrentClass(data);

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
	std::string fileName_;

	std::string classBody(CppData* data)
	{
		std::string className = getName();

		std::string staticVariableDefinitionString;
		std::string staticVariableDeclarationString;

		for (auto v : staticVariableDefinitions_)
		{
			staticVariableDefinitionString += "\t\t" + v->variableCpp(data) + ";\n";
			staticVariableDeclarationString += "\t" + v->variableDeclarationCpp(data, "\t") + ";\n";
		}

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
			if (v->isTypeDef())
			{
				v->addTypeDef(data);
				continue;
			}

			std::string variableDefString = "\t\t" + v->toCpp(data, "\t\t") + ";\n";

			if (v->isPrivate()) privateMemberVariableDefinitionString += variableDefString;
			else
			{
				publicMemberVariableDefinitionString += variableDefString;
				if (!v->isConstant())
				{
					addGetter(v, data);
					addSetter(v, data);
				}
			}
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

		Type* inherits = data->currentClass->classInheritedType;

		data->currentClass->toCppType(inherits);

		return ""
			+ enumDefString
			+ innerClassesString +
			"\tclass " + className + " : public " + inherits->name + "\n"
			"\t{\n"
			"\t\tGDCLASS(" + className + ", " + inherits->name + ")\n"
			"\tpublic:\n"
			+ publicMemberFunctionDefinitionString
			+ publicStaticFunctionDefinitionString
			+ staticVariableDefinitionString
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
			"\t};\n"
			+ staticVariableDeclarationString;
	}

	std::string cppIncludes(const CppData* data) const
	{
		const auto& types = data->currentClass->types;
		const auto& externalFunctions = data->currentClass->externalFunctions;
		const auto& typeDefinitions = data->currentClass->typeDefinitions;

		std::string code = "";

		if (!types.empty() || !externalFunctions.empty())
		{
			std::vector<std::string> includes;
			for (const auto& type : types)
			{
				if (CPP_PRIMITIVE_TYPES.contains(type)) continue;
				if (type == data->currentClass->currentClassName) continue;

				if (typeDefinitions.contains(type)) continue;

				auto include = data->currentClass->getIncludePath(type);
				if (include != "") includes.push_back(include);
				else includes.push_back("\"" + type + ".h\"");
			}

			for (const auto& externalFunction : externalFunctions)
			{
				auto include = data->currentClass->getIncludePath(externalFunction);
				if (include != "") includes.push_back(include);
			}

			std::sort(includes.begin(), includes.end());
			includes.erase(std::unique(includes.begin(), includes.end()), includes.end());

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

	void addGetter(VariableDefinitionSyntaxNode* variableDefinition, CppData* data)
	{
		std::string name = "get_" + variableDefinition->getName();
		auto prototype = new FunctionPrototypeSyntaxNode(new GDToken(name), {}, variableDefinition->getType(), false);

		auto returnVariableStatement = new ReturnSyntaxNode(new VariableSyntaxNode(new GDToken(variableDefinition->getName()), nullptr, true));

		auto body = new BodySyntaxNode({ returnVariableStatement });

		auto getterFunctionDef = new FunctionDefinitionSyntaxNode(prototype, body);

		getterFunctionDef->hoist(data);
		getterFunctionDef->resolveDefinitions(data);
		getterFunctionDef->resolveDefinitions(data);

		memberFunctionDefinitions_.push_back(getterFunctionDef);
	}

	void addSetter(VariableDefinitionSyntaxNode* variableDefinition, CppData* data)
	{
		auto argNameToken = new GDToken("new" + variableDefinition->getName());
		auto arg = new VariableDefinitionSyntaxNode(argNameToken, variableDefinition->getType(), nullptr, false, false, false, nullptr, nullptr);
		auto prototype = new FunctionPrototypeSyntaxNode(new GDToken("set_" + variableDefinition->getName()), { arg }, new Type("void"), false);

		auto setVariableStatement = new AssignmentSyntaxNode(new VariableSyntaxNode(new GDToken(variableDefinition->getName()), nullptr, false), new VariableSyntaxNode(argNameToken, nullptr, true));

		auto body = new BodySyntaxNode({ setVariableStatement });

		auto setterFunctionDef = new FunctionDefinitionSyntaxNode(prototype, body);

		setterFunctionDef->hoist(data);
		setterFunctionDef->resolveDefinitions(data);
		setterFunctionDef->resolveDefinitions(data);

		memberFunctionDefinitions_.push_back(setterFunctionDef);
	}

	Type* getInheritedType()
	{
		if (extends_) return new Type(extends_->value);
		return new Type("RefCounted");
	}

	void setCurrentClass(CppData* data)
	{
		data->currentClass = data->classData[getName()];
	}
};