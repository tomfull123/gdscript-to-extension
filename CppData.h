#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>

struct CppClassData;
class VariableDefinitionSyntaxNode;
class FunctionPrototypeSyntaxNode;
class ValueSyntaxNode;
struct Type;
struct Token;

struct CppData
{
	std::unordered_map<std::string, CppClassData*> classData;
	CppClassData* currentClass;
	std::unordered_map<std::string, std::string> inheritTypes = {};

	CppClassData* getClassDefinition(const std::string& className) const;

	VariableDefinitionSyntaxNode* getCurrentClassVariableDefinition(const std::string& variableName) const;

	VariableDefinitionSyntaxNode* getVariableDefinition(const std::string& className, const std::string& variableName) const;

	FunctionPrototypeSyntaxNode* getFunctionPrototype(const std::string& className, const std::string& functionName) const;

	std::string toWrappedCppFunction(ValueSyntaxNode* parentInstance, const Token* nameToken) const;

	std::string toCppType(const Type* type, bool isSubtype = false);

	bool isRefType(const std::string& type) const;

	bool isObjectType(const std::string& type) const;

	bool isSingletonType(const std::string& type) const;
};