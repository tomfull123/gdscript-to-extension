#pragma once

#include "TokenStream.h"
#include "AbstractSyntaxTree.h"
#include "ParserError.h"
#include "ClassDefinitionSyntaxNode.h"
#include "FunctionDefinitionSyntaxNode.h"

class Parser
{
public:
	explicit Parser(const std::vector<Token*>& tokens);

	void buildAST(AbstractSyntaxTree* ast);

private:
	TokenStream stream_;
	std::vector<ParserError> errors_;

	bool isNextTokenType(TokenType type, unsigned int offset = 0)
	{
		if (offset >= stream_.getLength()) return false;
		return stream_.peek(offset)->type == type;
	}

	SyntaxNode* addError(const std::string& error, const Token* token)
	{
		if (token)
			errors_.emplace_back(error, token->lineNumber, token->columnNumber, token->filename);
		else
			errors_.emplace_back(error, stream_.getLastTokenLineNumber(), stream_.getLastTokenColumnNumber(), "");

		if (stream_.end()) return nullptr;

		return nullptr;
	}

	SyntaxNode* addUnexpectedTokenError(const Token* token)
	{
		if (token == nullptr) return addError("Unexpected end of file", nullptr);
		return addError("Unexpected token: " + token->value, token);
	}

	SyntaxNode* addUnexpectedNextTokenError()
	{
		return addUnexpectedTokenError(stream_.next());
	}

	Type* parseType()
	{
		const Token* type = stream_.next();

		return new Type(type->value);
	}

	VariableDefinitionSyntaxNode* parseArgDefinition()
	{
		if (!isNextTokenType(TokenType::Identifier)) return (VariableDefinitionSyntaxNode*)addUnexpectedNextTokenError();

		Token* name = stream_.next();

		if (!isNextTokenType(TokenType::ColonSeparator)) return (VariableDefinitionSyntaxNode*)addUnexpectedNextTokenError();

		stream_.next(); // eat :

		Type* dataType = parseType();

		return new VariableDefinitionSyntaxNode(name, dataType);
	}

	FunctionPrototypeSyntaxNode* parseFunctionProtoype()
	{
		stream_.next(); // eat func

		if (!isNextTokenType(TokenType::Identifier)) return (FunctionPrototypeSyntaxNode*)addUnexpectedNextTokenError();

		Token* name = stream_.next();

		if (!isNextTokenType(TokenType::OpenBracketSeparator)) return (FunctionPrototypeSyntaxNode*)addUnexpectedNextTokenError();

		stream_.next(); // eat (

		std::vector<VariableDefinitionSyntaxNode*> argDefs;

		while (stream_.peek()->type != TokenType::CloseBracketSeparator)
		{
			auto* argDef = parseArgDefinition();

			if (argDef)
			{
				argDefs.push_back(argDef);

				if (isNextTokenType(TokenType::CommaSeparator)) stream_.next(); // eat ,
				else if (!isNextTokenType(TokenType::CloseBracketSeparator)) return (FunctionPrototypeSyntaxNode*)addUnexpectedNextTokenError();
			}
			else
			{
				return (FunctionPrototypeSyntaxNode*)addUnexpectedNextTokenError();
			}
		}

		stream_.next(); // eat )

		Type* returnType = nullptr;

		if (isNextTokenType(TokenType::ArrowSeparator))
		{
			stream_.next(); // eat ->
			returnType = parseType();
		}

		if (!isNextTokenType(TokenType::ColonSeparator)) return (FunctionPrototypeSyntaxNode*)addUnexpectedNextTokenError();

		stream_.next(); // eat :

		return new FunctionPrototypeSyntaxNode(name, argDefs, returnType, false);
	}

	FunctionBodySyntaxNode* parseFunctionBody()
	{
		std::vector<SyntaxNode*> nodes;

		while (!stream_.end())
		{
			nodes.push_back(parseExpression());
		}

		return new FunctionBodySyntaxNode(nodes);
	}

	FunctionDefinitionSyntaxNode* parseFunction()
	{
		FunctionPrototypeSyntaxNode* prototype = parseFunctionProtoype();

		if (!prototype) return nullptr;

		FunctionBodySyntaxNode* body = parseFunctionBody();

		if (!body) return nullptr;

		return new FunctionDefinitionSyntaxNode(prototype, body);
	}

	ClassDefinitionSyntaxNode* parseScriptBody()
	{
		std::vector<FunctionDefinitionSyntaxNode*> functionDefinitions;

		while (!stream_.end())
		{
			const auto* t = stream_.peek();

			switch (t->type)
			{
			case TokenType::FuncKeyword:
				functionDefinitions.push_back(parseFunction());
			}
		}

		return new ClassDefinitionSyntaxNode();
	}

	SyntaxNode* parseExpression()
	{
		const Token* t = stream_.peek();

		switch (t->type) {
		case TokenType::ReturnKeyword:
			return parseReturnStatement();
		case TokenType::BreakKeyword:
			return parseBreakStatement();
		case TokenType::ContinueKeyword:
			return parseContinueStatement();
		case TokenType::Identifier:
			return parseVariableOrFunctionCall(false);
		case TokenType::IfKeyword:
			return parseIfStatement();
		case TokenType::VarKeyword:
		case TokenType::ConstKeyword:
			return parseVariableDefinition(Visibility::Public, "", false);
		case TokenType::WhileKeyword:
			return parseWhileLoop();
		case TokenType::ForKeyword:
			return parseForLoop();
		default:
			return (SyntaxNode*)addUnexpectedTokenError(stream_.next());
		}
	}
};
