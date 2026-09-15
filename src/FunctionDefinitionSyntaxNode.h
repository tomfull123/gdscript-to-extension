#pragma once

#include "FunctionPrototypeSyntaxNode.h"
#include "BodySyntaxNode.h"
#include "RpcSyntaxNode.h"

class FunctionDefinitionSyntaxNode : public SyntaxNode
{
public:
	FunctionDefinitionSyntaxNode(
		RpcSyntaxNode* rpc,
		FunctionPrototypeSyntaxNode* prototype,
		BodySyntaxNode* body
	) :
		rpc_(rpc),
		prototype_(prototype),
		body_(body)
	{
	}

	std::string getName() const
	{
		return prototype_->getName();
	}

	std::vector<VariableDefinitionSyntaxNode*> getArgDefs() const
	{
		return prototype_->getArgDefs();
	}

	bool isPrivate() const
	{
		return prototype_->isPrivate();
	}

	void hoist(CppData* data) override
	{
		prototype_->hoist(data);
		body_->hoist(data);
	}

	void resolveDefinitions(CppData* data) override
	{
		prototype_->resolveDefinitions(data);
		body_->resolveDefinitions(data);
	}

	void resolveTypes(CppData* data, Type* otherType = nullptr) override
	{
		prototype_->resolveTypes(data);
		body_->resolveTypes(data);
	}

	std::string toCpp(CppData* data, const std::string& indents) override
	{
		std::string prototypeString = prototype_->toCpp(data, indents) + "\n";
		std::string bodyString = body_->toCpp(data, indents);
		return ""
			+ prototypeString
			+ bodyString;
	}

	bool hasRpc() const
	{
		return rpc_ != nullptr;
	}

	std::string buildRpcCpp(CppData* data, const std::string& indents) const
	{
		if (rpc_)
		{
			return rpc_->toCpp(getName(), data, indents);
		}

		return "";
	}

private:
	RpcSyntaxNode* rpc_;
	FunctionPrototypeSyntaxNode* prototype_;
	BodySyntaxNode* body_;
};