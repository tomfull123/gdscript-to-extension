#pragma once

#include "GDTokenStream.h"
#include <unordered_set>
#include <unordered_map>
#include "Type.h"
#include "TranspilerDefinitions.h"
#include "CppClassData.h"
#include "CppData.h"

class SyntaxNode
{
public:
	virtual bool needsSemiColon()
	{
		return true;
	}

	virtual void hoist(CppData* data) = 0;

	virtual void resolveDefinitions(CppData* data) = 0;

	virtual void resolveTypes(CppData* data, Type* otherType = nullptr) = 0;

	virtual std::string toCpp(CppData* data, const std::string& indents) = 0;
};

class ValueSyntaxNode : public SyntaxNode
{
public:
	virtual bool hasParent() const
	{
		return false;
	}

	virtual ValueSyntaxNode* getParent() const
	{
		return nullptr;
	}

	virtual bool isFunction(CppData* data) const
	{
		return false;
	}

	virtual Type* getType() = 0;

	virtual std::string getName() = 0;
};
