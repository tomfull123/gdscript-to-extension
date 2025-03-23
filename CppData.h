#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>

struct CppClassData;
class VariableDefinitionSyntaxNode;
class FunctionPrototypeSyntaxNode;
struct Type;

struct CppData
{
	std::unordered_map<std::string, CppClassData*> classData;
	CppClassData* currentClass;
	std::unordered_map<std::string, std::string> inheritTypes = {};

	CppClassData* getClassDefinition(const std::string& className) const;

	VariableDefinitionSyntaxNode* getVariableDefinition(const std::string& typeName, const std::string& variableName) const;

	FunctionPrototypeSyntaxNode* getFunctionPrototype(const std::string& typeName, const std::string& functionName) const;

	std::string toCppType(const Type* type);

	bool isRefType(const std::string& type) const;
};