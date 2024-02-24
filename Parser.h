#pragma once

#include "TokenStream.h"
#include "AbstractSyntaxTree.h"
#include "ParserError.h"
#include "ClassDefinitionSyntaxNode.h"
#include "FunctionDefinitionSyntaxNode.h"
#include "ReturnSyntaxNode.h"
#include "BreakSyntaxNode.h"
#include "ContinueSyntaxNode.h"
#include "IfSyntaxNode.h"
#include "VariableSyntaxNode.h"
#include "CallSyntaxNode.h"
#include "LiteralValueSyntaxNode.h"
#include "AssignmentSyntaxNode.h"
#include "EqualityOperatorSyntaxNode.h"
#include "NotOperatorSyntaxNode.h"
#include "BooleanLiteralSyntaxNode.h"

class Parser
{
public:
	explicit Parser(const std::vector<Token*>& tokens);

	void buildAST(AbstractSyntaxTree* ast);

	const std::vector<ParserError>& getErrors() const { return errors_; }

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
		return addUnexpectedTokenError(next());
	}

	Token* next()
	{
		return stream_.next();
	}

	Token* peek(unsigned int offset = 0)
	{
		return stream_.peek(offset);
	}

	bool end() const
	{
		return stream_.end();
	}

	Token* consume(TokenType tokenType)
	{
		if (isNextTokenType(tokenType))
		{
			return next();
		}

		addUnexpectedNextTokenError();
		return nullptr;
	}

	Type* parseType()
	{
		const Token* type = next();

		return new Type(type->value);
	}

	VariableDefinitionSyntaxNode* parseArgDefinition()
	{
		Token* name = consume(TokenType::Identifier);

		if (!name) return nullptr;

		Type* type = nullptr;

		if (isNextTokenType(TokenType::ColonSeparator))
		{
			next(); // eat :
			if (!isNextTokenType(TokenType::AssignmentOperator))
			{
				type = parseType();
			}
		}

		ValueSyntaxNode* assignmentValue = nullptr;

		if (isNextTokenType(TokenType::AssignmentOperator))
		{
			next(); // eat =
			assignmentValue = parseValueExpression();
		}

		return new VariableDefinitionSyntaxNode(name, type, assignmentValue);
	}

	FunctionPrototypeSyntaxNode* parseFunctionProtoype()
	{
		consume(TokenType::FuncKeyword); // eat func

		Token* name = consume(TokenType::Identifier);

		if (!name) return nullptr;

		if (!consume(TokenType::OpenBracketSeparator)) return nullptr;

		std::vector<VariableDefinitionSyntaxNode*> argDefs;

		while (stream_.peek()->type != TokenType::CloseBracketSeparator)
		{
			auto* argDef = parseArgDefinition();

			if (argDef)
			{
				argDefs.push_back(argDef);

				if (isNextTokenType(TokenType::CommaSeparator)) next(); // eat ,
				else if (!isNextTokenType(TokenType::CloseBracketSeparator)) return (FunctionPrototypeSyntaxNode*)addUnexpectedNextTokenError();
			}
			else
			{
				return (FunctionPrototypeSyntaxNode*)addUnexpectedNextTokenError();
			}
		}

		next(); // eat )

		Type* returnType = nullptr;

		if (isNextTokenType(TokenType::ArrowSeparator))
		{
			next(); // eat ->
			returnType = parseType();
		}

		if (!consume(TokenType::ColonSeparator)) return nullptr;

		return new FunctionPrototypeSyntaxNode(name, argDefs, returnType, false);
	}

	FunctionBodySyntaxNode* parseFunctionBody()
	{
		std::vector<SyntaxNode*> nodes;

		while (!stream_.end() && !isNextTokenType(TokenType::EndOfBlock))
		{
			auto ex = parseExpression();
			if (ex) nodes.push_back(ex);
		}

		if (isNextTokenType(TokenType::EndOfBlock)) next();

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

	Token* parseClassName()
	{
		consume(TokenType::ClassNameKeyword); // eat class_name

		return consume(TokenType::Identifier);
	}

	VariableDefinitionSyntaxNode* parseVariableDefinition()
	{
		auto varOrConst = next();

		auto name = consume(TokenType::Identifier);

		if (!name) return nullptr;

		Type* type = nullptr;

		if (isNextTokenType(TokenType::ColonSeparator))
		{
			next(); // eat :
			if (!isNextTokenType(TokenType::AssignmentOperator))
			{
				type = parseType();
			}
		}

		ValueSyntaxNode* assignmentValue = nullptr;

		if (isNextTokenType(TokenType::AssignmentOperator))
		{
			next(); // eat =
			assignmentValue = parseValueExpression();
		}

		return new VariableDefinitionSyntaxNode(name, type, assignmentValue);
	}

	ClassDefinitionSyntaxNode* parseScriptBody()
	{
		Token* name = nullptr;
		std::vector<FunctionDefinitionSyntaxNode*> memberFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> memberVariableDefinitions;

		bool endOfClass = false;

		while (!end() && !endOfClass)
		{
			const auto* t = peek();

			switch (t->type)
			{
			case TokenType::ClassNameKeyword:
				name = parseClassName();
				break;
			case TokenType::FuncKeyword:
				memberFunctionDefinitions.push_back(parseFunction());
				break;
			case TokenType::VarKeyword:
			case TokenType::ConstKeyword:
				memberVariableDefinitions.push_back(parseVariableDefinition());
				break;
			case TokenType::EndOfBlock:
				endOfClass = true;
				break;
			default:
				return (ClassDefinitionSyntaxNode*)addUnexpectedNextTokenError();
			}
		}

		return new ClassDefinitionSyntaxNode(name, memberFunctionDefinitions, memberVariableDefinitions);
	}

	LiteralValueSyntaxNode* parseLiteralValue()
	{
		auto value = next();
		TokenType type = value->type;

		switch (type)
		{
		case TokenType::IntLiteral: return new LiteralValueSyntaxNode(value, new Type("int"));
		case TokenType::FloatLiteral: return new LiteralValueSyntaxNode(value, new Type("float"));
		}

		return (LiteralValueSyntaxNode*)addUnexpectedTokenError(value);
	}

	NotOperatorSyntaxNode* parseNotOperator()
	{
		if (auto token = consume(TokenType::NotOperator))
		{
			ValueSyntaxNode* condition = parseValueExpression();

			return new NotOperatorSyntaxNode(token, condition);
		}

		return nullptr;
	}

	BooleanLiteralSyntaxNode* parseBooleanLiteral()
	{
		Token* t = next();

		if (t->type == TokenType::TrueKeyword) return new BooleanLiteralSyntaxNode(t, true);
		if (t->type == TokenType::FalseKeyword) return new BooleanLiteralSyntaxNode(t, false);

		return (BooleanLiteralSyntaxNode*)addUnexpectedTokenError(t);
	}

	ValueSyntaxNode* parseSingleValueObject()
	{
		Token* value = peek();
		TokenType type = value->type;

		switch (type)
		{
		case TokenType::IntLiteral:
		{
			auto literalValue = parseLiteralValue();

			return literalValue;
		}
		case TokenType::Identifier: return parseVariableOrFunctionCall(true);
		case TokenType::NotOperator: return parseNotOperator();
		case TokenType::TrueKeyword: return parseBooleanLiteral();
		case TokenType::FalseKeyword: return parseBooleanLiteral();
		}

		return (ValueSyntaxNode*)addUnexpectedNextTokenError();
	}

	ValueSyntaxNode* parseValueExpression()
	{
		int bracketCount = 0;

		while (isNextTokenType(TokenType::OpenBracketSeparator))
		{
			next(); // eat (
			bracketCount++;
		}

		ValueSyntaxNode* lhs = parseSingleValueObject();

		if (!lhs) return (ValueSyntaxNode*)addUnexpectedNextTokenError();

		while (true)
		{
			// Bool operators
			if (isNextTokenType(TokenType::EqualityOperator) || isNextTokenType(TokenType::NotEqualityOperator)
				|| isNextTokenType(TokenType::GreaterThanSeparator) || isNextTokenType(TokenType::GreaterThanEqualSeparator)
				|| isNextTokenType(TokenType::LessThanSeparator) || isNextTokenType(TokenType::LessThanEqualSeparator))
			{
				Token* boolOperator = next();

				ValueSyntaxNode* rhs = parseValueExpression();

				if (!rhs) return nullptr;

				lhs = new EqualityOperatorSyntaxNode(boolOperator, lhs, rhs);

				continue;
			}

			break;
		}

		return lhs;
	}

	ReturnSyntaxNode* parseReturnStatement()
	{
		if (!consume(TokenType::ReturnKeyword)) return nullptr;

		return new ReturnSyntaxNode(parseValueExpression());
	}

	BreakSyntaxNode* parseBreakStatement()
	{
		if (!consume(TokenType::BreakKeyword)) return nullptr;

		return new BreakSyntaxNode();
	}

	ContinueSyntaxNode* parseContinueStatement()
	{
		if (!consume(TokenType::ContinueKeyword)) return nullptr;

		return new ContinueSyntaxNode();
	}

	ValueSyntaxNode* parseVariableOrFunctionCall(bool asValue, ValueSyntaxNode* instance = nullptr)
	{
		auto name = consume(TokenType::Identifier);

		if (!name) return nullptr;

		if (isNextTokenType(TokenType::DotSeparator))
		{
			next(); // eat .

			if (isNextTokenType(TokenType::Identifier))
			{
				return parseVariableOrFunctionCall(asValue, new VariableSyntaxNode(name, instance));
			}
		}

		// Function call
		if (isNextTokenType(TokenType::OpenBracketSeparator))
		{
			next(); // eat (

			std::vector<ValueSyntaxNode*> args;

			while (!isNextTokenType(TokenType::CloseBracketSeparator))
			{
				auto e = parseValueExpression();
				if (e)
				{
					args.push_back(e);
					if (isNextTokenType(TokenType::CommaSeparator)) next(); // eat ,
					else if (!isNextTokenType(TokenType::CloseBracketSeparator)) return (ValueSyntaxNode*)addUnexpectedNextTokenError();
				}
				else
				{
					return (ValueSyntaxNode*)addUnexpectedNextTokenError();
				}
			}

			if (!consume(TokenType::CloseBracketSeparator)) return nullptr;

			auto call = new CallSyntaxNode(instance, name, args);

			return call;
		}

		VariableSyntaxNode* variable = new VariableSyntaxNode(name, instance);

		if (asValue)
		{
			return variable;
		}
		else
		{
			// Variable assignment
			if (isNextTokenType(TokenType::AssignmentOperator))
			{
				next(); // eat =

				ValueSyntaxNode* assignmentValue = parseValueExpression();

				if (!assignmentValue) return nullptr;

				return new AssignmentSyntaxNode(variable, assignmentValue);
			}
		}

		return nullptr;
	}

	IfSyntaxNode* parseIfStatement()
	{
		next(); // eat if

		ValueSyntaxNode* condition = parseValueExpression();

		if (!condition) return nullptr;

		if (!consume(TokenType::ColonSeparator)) return nullptr;

		std::vector<SyntaxNode*> thenExpressions;

		while (!isNextTokenType(TokenType::EndOfBlock))
		{
			auto ex = parseExpression();
			if (ex) thenExpressions.push_back(ex);
		}

		next(); // eat EndOfBlock

		return new IfSyntaxNode(condition, thenExpressions, {});
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
			return parseVariableDefinition();
		case TokenType::PassKeyword:
			next(); // eat pass
			return nullptr;
		case TokenType::WhileKeyword:
			//return parseWhileLoop();
		case TokenType::ForKeyword:
			//return parseForLoop();
		default:
			return (SyntaxNode*)addUnexpectedNextTokenError();
		}
	}
};
