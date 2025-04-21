#pragma once

#include "FunctionDefinitionSyntaxNode.h"
#include "VariableDefinitionSyntaxNode.h"
#include "EnumDefinitionSyntaxNode.h"
#include "ReturnSyntaxNode.h"
#include "VariableSyntaxNode.h"
#include "AssignmentSyntaxNode.h"
#include "BodySyntaxNode.h"
#include "ConstantValueMapping.h"
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
		const std::vector<ConstantValueMapping*>& constantValueMappings,
		bool isInnerClass,
		const std::string& fileName,
		bool isDocsClass
	) :
		name_(name),
		extends_(extends),
		memberFunctionDefinitions_(memberFunctionDefinitions),
		memberVariableDefinitions_(memberVariableDefinitions),
		enumDefinitions_(enumDefinitions),
		staticFunctionDefinitions_(staticFunctionDefinitions),
		staticVariableDefinitions_(staticVariableDefinitions),
		innerClasses_(innerClasses),
		constantValueMappings_(constantValueMappings),
		isInnerClass_(isInnerClass),
		fileName_(fileName),
		isDocsClass_(isDocsClass)
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
		auto currentClass = new CppClassData();
		data->currentClass = currentClass;
		data->currentClass->currentClassName = getName();
		data->currentClass->currentClassType = new Type(data->currentClass->currentClassName);
		data->currentClass->classInheritedType = getInheritedType();
		if (extends_ || !isDocsClass_) data->inheritTypes[getName()] = getInheritedType()->getName();
		for (auto constantValueMapping : constantValueMappings_) constantValueMapping->hoist(data);
		for (auto enumDef : enumDefinitions_) enumDef->hoist(data);
		for (auto v : staticVariableDefinitions_) v->hoist(data);
		for (auto f : staticFunctionDefinitions_) f->hoist(data);
		for (auto v : memberVariableDefinitions_) v->hoist(data);
		for (auto f : memberFunctionDefinitions_) f->hoist(data);
		for (auto c : innerClasses_) c->hoist(data);
		data->classData[getName()] = currentClass;
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
	std::vector<ConstantValueMapping*> constantValueMappings_;
	bool isInnerClass_;
	std::string fileName_;
	bool isDocsClass_;

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

		std::string privateMemberVariableDefinitionString;
		std::string publicMemberVariableDefinitionString;
		std::string bindPropertysString;

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
					bindPropertysString += bindProperty(v, data, "\t\t\t");
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
				if (!GODOT_LIFECYCLE_METHODS.contains(f->getName()))
					bindMethodsString += bindMethod(className, f, "\t\t\t", false);
			}
		}

		Type* inherits = data->currentClass->classInheritedType;

		if (inherits) data->toCppType(inherits);

		std::string inheritsName = "";

		if (inherits) inheritsName = inherits->getName();

		std::string innerClassesString;

		for (auto innerClass : innerClasses_)
		{
			innerClassesString += innerClass->toCpp(data, "\t") + "\n";
		}

		return ""
			+ enumDefString
			+ innerClassesString +
			"\tclass " + className + " : public " + inheritsName + "\n"
			"\t{\n"
			"\t\tGDCLASS(" + className + ", " + inheritsName + ")\n"
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
			+ bindStaticMethodsString
			+ bindPropertysString +
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

	std::string bindProperty(VariableDefinitionSyntaxNode* variableDefinition, CppData* data, const std::string& indents) const
	{
		Type* type = variableDefinition->getType();
		std::string typeName;
		if (type) typeName = type->getName();
		std::string name = variableDefinition->getName();
		std::string getterName;
		Token* getterNameToken = variableDefinition->getGetterName();
		if (getterNameToken) getterName = getterNameToken->value;
		std::string setterName;
		Token* setterNameToken = variableDefinition->getSetterName();
		if (setterNameToken) setterName = setterNameToken->value;

		std::string code = indents + "ADD_PROPERTY(";

		code += "PropertyInfo(Variant::" + getVariantType(type) + ", \"" + name + "\"";

		if (data->isResourceType(typeName))
		{
			code += ", PROPERTY_HINT_RESOURCE_TYPE, \"" + typeName + "\"";
		}

		code += ")";

		code += ", \"" + setterName + "\", \"" + getterName + "\");\n";

		return code;
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
		auto getterName = variableDefinition->getGetterName();
		if (getterName == nullptr) return;
		std::string name = getterName->value;
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
		auto setterName = variableDefinition->getSetterName();
		if (setterName == nullptr) return;
		auto argNameToken = new GDToken("new" + variableDefinition->getName());
		auto arg = new VariableDefinitionSyntaxNode(argNameToken, variableDefinition->getType(), nullptr, false, false, false, nullptr, nullptr);
		auto prototype = new FunctionPrototypeSyntaxNode(new GDToken(setterName->value), { arg }, new Type("void"), false);

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
		if (isDocsClass_) return nullptr;
		return new Type("RefCounted");
	}

	void setCurrentClass(CppData* data)
	{
		data->currentClass = data->classData[getName()];
	}

	std::string getVariantType(const Type* type) const
	{
		std::string typeName;
		if (type) typeName = type->getName();

		if (typeName == "bool") return "BOOL";
		if (typeName == "int") return "INT";
		if (typeName == "float") return "FLOAT";
		if (typeName == "String") return "STRING";

		if (typeName == "Vector2") return "VECTOR2";
		if (typeName == "Vector2i") return "VECTOR2I";
		if (typeName == "Rect2") return "RECT2";
		if (typeName == "Rect2i") return "RECT2I";
		if (typeName == "Rect2i") return "RECT2I";
		if (typeName == "Vector3") return "VECTOR3";
		if (typeName == "Vector3i") return "VECTOR3I";
		if (typeName == "Transform2D") return "TRANSFORM2D";
		if (typeName == "Vector4") return "VECTOR4";
		if (typeName == "Vector4i") return "VECTOR4I";
		if (typeName == "Plane") return "PLANE";
		if (typeName == "Quaternion") return "QUATERNION";
		if (typeName == "AABB") return "AABB";
		if (typeName == "Basis") return "BASIS";
		if (typeName == "Transform3D") return "TRANSFORM3D";
		if (typeName == "Projection") return "PROJECTION";

		if (typeName == "Color") return "COLOR";
		if (typeName == "StringName") return "STRING_NAME";
		if (typeName == "NodePath") return "NODE_PATH";
		if (typeName == "Rid") return "RID";
		if (typeName == "Object") return "OBJECT";
		if (typeName == "Callable") return "CALLABLE";
		if (typeName == "Signal") return "SIGNAL";
		if (typeName == "Dictionary") return "DICTIONARY";
		if (typeName == "Array") return "ARRAY";

		if (typeName == "PackedByteArray") return "PACKED_BYTE_ARRAY";
		if (typeName == "PackedInt32Array") return "PACKED_INT32_ARRAY";
		if (typeName == "PackedInt64Array") return "PACKED_INT64_ARRAY";
		if (typeName == "PackedFloat32Array") return "PACKED_FLOAT32_ARRAY";
		if (typeName == "PackedFloat64Array") return "PACKED_FLOAT64_ARRAY";
		if (typeName == "PackedStringArray") return "PACKED_STRING_ARRAY";
		if (typeName == "PackedVector2Array") return "PACKED_VECTOR2_ARRAY";
		if (typeName == "PackedVector3Array") return "PACKED_VECTOR3_ARRAY";
		if (typeName == "PackedColorArray") return "PACKED_COLOR_ARRAY";
		if (typeName == "PackedVector4Array") return "PACKED_VECTOR4_ARRAY";

		return "OBJECT";
	}
};