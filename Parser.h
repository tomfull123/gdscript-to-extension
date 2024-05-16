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
#include "ArrayValueSyntaxNode.h"
#include "EnumDefinitionSyntaxNode.h"
#include "DictionaryValueSyntaxNode.h"
#include "MathOperatorSyntaxNode.h"
#include "PrefixOperator.h"
#include "ValueIndexValue.h"
#include "PreloadSyntaxNode.h"
#include "BooleanOperatorSyntaxNode.h"
#include "WhileSyntaxNode.h"
#include "ForSyntaxNode.h"
#include "NullSyntaxNode.h"
#include "BitwiseOperatorSyntaxNode.h"
#include "IsOperatorSyntaxNode.h"

struct Result
{
	Result(
		AbstractSyntaxTree* ast,
		const std::vector<ParserError>& errors
	) :
		ast(ast),
		errors(errors)
	{}

	AbstractSyntaxTree* ast;
	std::vector<ParserError> errors;
};

class Parser
{
public:
	explicit Parser(const std::vector<Token*>& tokens);

	void buildAST(AbstractSyntaxTree* ast);

	const std::vector<ParserError>& getErrors() const { return errors_; }

	static Result* parse(const std::string& input)
	{
		AbstractSyntaxTree* ast = new AbstractSyntaxTree();

		Lexer lexer(input);

		std::vector<Token*> tokens = lexer.readAllTokens();

		Parser parser(tokens);

		parser.buildAST(ast);

		const auto& errors = parser.getErrors();

		return new Result(ast, errors);
	}

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
		const Token* typeToken = next();
		std::vector<Type*> subtypes = {};

		if (isNextTokenType(TokenType::OpenSquareBracket))
		{
			next(); // eat [

			auto subtype = parseType();
			if (subtype) subtypes.push_back(subtype);

			if (!consume(TokenType::CloseSquareBracket)) return nullptr;
		}

		return new Type(typeToken->value, subtypes);
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

		return new VariableDefinitionSyntaxNode(name, type, assignmentValue, false, false);
	}

	FunctionPrototypeSyntaxNode* parseFunctionProtoype(bool isStatic)
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

		return new FunctionPrototypeSyntaxNode(name, argDefs, returnType, isStatic);
	}

	BodySyntaxNode* parseBody(int indentDepth, int lineNumber, bool returnThis = false)
	{
		std::vector<SyntaxNode*> nodes;

		while (!stream_.end() && (peek()->indentDepth > indentDepth || peek()->lineNumber == lineNumber))
		{
			auto ex = parseExpression();
			if (ex) nodes.push_back(ex);
		}

		if (returnThis)
		{
			auto thisToken = new Token();
			thisToken->value = "this";
			nodes.push_back(new ReturnSyntaxNode(new VariableSyntaxNode(thisToken, nullptr)));
		}

		return new BodySyntaxNode(nodes);
	}

	FunctionDefinitionSyntaxNode* parseFunction(bool isStatic)
	{
		auto token = peek();

		FunctionPrototypeSyntaxNode* prototype = parseFunctionProtoype(isStatic);

		if (!prototype) return nullptr;

		BodySyntaxNode* body = parseBody(token->indentDepth, token->lineNumber, prototype->getToken()->value == "_init");

		if (!body) return nullptr;

		return new FunctionDefinitionSyntaxNode(prototype, body);
	}

	Token* parseClassName()
	{
		consume(TokenType::ClassNameKeyword); // eat class_name

		return consume(TokenType::Identifier);
	}

	VariableDefinitionSyntaxNode* parseVariableDefinition(bool isClassMember)
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

		return new VariableDefinitionSyntaxNode(name, type, assignmentValue, varOrConst->type == TokenType::ConstKeyword, isClassMember);
	}

	VariableDefinitionSyntaxNode* parseSignalDefinitions()
	{
		next(); // eat signal

		auto signalName = consume(TokenType::Identifier);

		if (!signalName) return nullptr;

		if (isNextTokenType(TokenType::OpenBracketSeparator))
		{
			next(); // eat (

			std::vector<VariableDefinitionSyntaxNode*> args;

			while (!isNextTokenType(TokenType::CloseBracketSeparator))
			{
				auto arg = parseArgDefinition();
				if (arg) args.push_back(arg);
			}

			next(); // eat )
		}

		return new VariableDefinitionSyntaxNode(signalName, new Type("Signal"), nullptr, false, true);
	}

	void parseAnnotation()
	{
		next(); // eat warning_ignore or tool

		if (isNextTokenType(TokenType::OpenBracketSeparator))
		{
			if (!consume(TokenType::OpenBracketSeparator)) return;
			if (!consume(TokenType::StringLiteral)) return;
			if (!consume(TokenType::CloseBracketSeparator)) return;
		}
	}

	EnumValueSyntaxNode* parseEnumValueDefinition()
	{
		auto name = consume(TokenType::Identifier);
		if (!name) return nullptr;
		return new EnumValueSyntaxNode(name);
	}

	EnumDefinitionSyntaxNode* parseEnumDefinition()
	{
		next(); // eat enum

		auto name = consume(TokenType::Identifier);

		if (!name) return nullptr;

		if (!consume(TokenType::OpenCurlyBracketSeparator)) return nullptr;

		std::vector<EnumValueSyntaxNode*> values;

		while (!isNextTokenType(TokenType::CloseCurlyBracketSeparator))
		{
			auto value = parseEnumValueDefinition();
			if (value) values.push_back(value);

			if (isNextTokenType(TokenType::CommaSeparator)) next(); // eat ,
			else if (!isNextTokenType(TokenType::CloseCurlyBracketSeparator)) return (EnumDefinitionSyntaxNode*)addUnexpectedNextTokenError();
		}

		next(); // eat }

		return new EnumDefinitionSyntaxNode(name, values);
	}

	Token* parseExtends()
	{
		consume(TokenType::ExtendsKeyword); // eat extends

		return consume(TokenType::Identifier);
	}

	ClassDefinitionSyntaxNode* parseScriptBody(int indentDepth, Token* nameToken = nullptr, bool isInnerClass = false)
	{
		Token* name = nameToken;
		Token* extends = nullptr;
		std::vector<FunctionDefinitionSyntaxNode*> memberFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> memberVariableDefinitions;
		std::vector<EnumDefinitionSyntaxNode*> enumDefinitions;
		std::vector<FunctionDefinitionSyntaxNode*> staticFunctionDefinitions;
		std::vector<VariableDefinitionSyntaxNode*> staticVariableDefinitions;
		std::vector<ClassDefinitionSyntaxNode*> innerClasses;

		bool endOfClass = false;

		while (!end() && !endOfClass)
		{
			const auto* t = peek();

			if (t->indentDepth < indentDepth)
			{
				endOfClass = true;
				continue;
			}

			switch (t->type)
			{
			case TokenType::ClassNameKeyword:
				name = parseClassName();
				break;
			case TokenType::ExtendsKeyword:
				extends = parseExtends();
				break;
			case TokenType::FuncKeyword:
				memberFunctionDefinitions.push_back(parseFunction(false));
				break;
			case TokenType::VarKeyword:
			case TokenType::ConstKeyword:
				memberVariableDefinitions.push_back(parseVariableDefinition(true));
				break;
			case TokenType::SignalKeyword:
				memberVariableDefinitions.push_back(parseSignalDefinitions());
				break;
			case TokenType::Annotation:
				parseAnnotation();
				break;
			case TokenType::EnumKeyword:
				enumDefinitions.push_back(parseEnumDefinition());
				break;
			case TokenType::StaticKeyword:
				next(); // eat static

				if (isNextTokenType(TokenType::FuncKeyword))
				{
					staticFunctionDefinitions.push_back(parseFunction(true));
					break;
				}

				if (isNextTokenType(TokenType::VarKeyword) || isNextTokenType(TokenType::ConstKeyword))
				{
					staticVariableDefinitions.push_back(parseVariableDefinition(false));
					break;
				}

				addUnexpectedNextTokenError();

				break;
			case TokenType::ClassKeyword:
			{
				next(); // eat class
				auto subclassName = consume(TokenType::Identifier);
				consume(TokenType::ColonSeparator);
				auto internalClass = parseScriptBody(t->indentDepth + 1, subclassName, true);
				if (internalClass) innerClasses.push_back(internalClass);
				break;
			}
			default:
				return (ClassDefinitionSyntaxNode*)addUnexpectedNextTokenError();
			}
		}

		return new ClassDefinitionSyntaxNode(
			name,
			extends,
			memberFunctionDefinitions,
			memberVariableDefinitions,
			enumDefinitions,
			staticFunctionDefinitions,
			staticVariableDefinitions,
			innerClasses,
			isInnerClass
		);
	}

	LiteralValueSyntaxNode* parseLiteralValue()
	{
		auto value = next();
		TokenType type = value->type;

		switch (type)
		{
		case TokenType::IntLiteral: return new LiteralValueSyntaxNode(value, new Type("int"));
		case TokenType::FloatLiteral: return new LiteralValueSyntaxNode(value, new Type("float"));
		case TokenType::StringLiteral: return new LiteralValueSyntaxNode(value, new Type("string"));
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

	ValueSyntaxNode* parseArrayValue()
	{
		if (!consume(TokenType::OpenSquareBracket)) return nullptr;

		std::vector<ValueSyntaxNode*> expressions;

		while (!isNextTokenType(TokenType::CloseSquareBracket))
		{
			auto ex = parseValueExpression();
			if (ex) expressions.push_back(ex);

			if (isNextTokenType(TokenType::CommaSeparator)) next(); // eat ,
			else if (!isNextTokenType(TokenType::CloseSquareBracket)) return (ValueSyntaxNode*)addUnexpectedNextTokenError();
		}

		next(); // eat ]

		return new ArrayValueSyntaxNode(expressions);
	}

	ValueSyntaxNode* parseDictionaryValue()
	{
		next(); // eat {
		std::map<ValueSyntaxNode*, ValueSyntaxNode*> values;

		while (!isNextTokenType(TokenType::CloseCurlyBracketSeparator))
		{
			auto key = parseSingleValueObject();

			if (!consume(TokenType::ColonSeparator)) return nullptr;

			auto value = parseValueExpression();
			if (value) values[key] = value;

			if (isNextTokenType(TokenType::CommaSeparator)) next(); // eat ,
			else if (!isNextTokenType(TokenType::CloseCurlyBracketSeparator)) return (ValueSyntaxNode*)addUnexpectedNextTokenError();
		}

		next(); // eat }

		return new DictionaryValueSyntaxNode(values);
	}

	ValueSyntaxNode* parsePreload()
	{
		if (!consume(TokenType::PreloadKeyword)) return nullptr;
		if (!consume(TokenType::OpenBracketSeparator)) return nullptr;
		auto preloadPath = consume(TokenType::StringLiteral);

		if (!preloadPath) return nullptr;

		if (!consume(TokenType::CloseBracketSeparator)) return nullptr;

		return new PreloadSyntaxNode(preloadPath);
	}

	NullSyntaxNode* parseNullValue()
	{
		if (!consume(TokenType::NullKeyword)) return nullptr;

		return new NullSyntaxNode();
	}

	ValueSyntaxNode* parseSingleValueObject()
	{
		Token* value = peek();
		TokenType type = value->type;

		switch (type)
		{
		case TokenType::IntLiteral:
		case TokenType::FloatLiteral:
		case TokenType::StringLiteral:
		{
			auto literalValue = parseLiteralValue();

			return literalValue;
		}
		case TokenType::Identifier: return parseVariableOrFunctionCall(true);
		case TokenType::NotOperator: return parseNotOperator();
		case TokenType::TrueKeyword: return parseBooleanLiteral();
		case TokenType::FalseKeyword: return parseBooleanLiteral();
		case TokenType::OpenSquareBracket: return parseArrayValue();
		case TokenType::OpenCurlyBracketSeparator: return parseDictionaryValue();
		case TokenType::Operator:
		{
			if (value->value == "-")
			{
				auto operatorToken = next(); // eat -

				return new PrefixOperator(operatorToken, parseSingleValueObject());
			}
			break;
		}
		case TokenType::PreloadKeyword:
			return parsePreload();
		case TokenType::NullKeyword:
			return parseNullValue();
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

			if (isNextTokenType(TokenType::IsKeyword))
			{
				auto isToken = next();

				Type* type = parseType();

				lhs = new IsOperatorSyntaxNode(isToken, lhs, type);

				continue;
			}

			// Math operators
			if (isNextTokenType(TokenType::Operator))
			{
				Token* operatorToken = next();

				ValueSyntaxNode* rhs = nullptr;

				rhs = parseValueExpression();

				if (!rhs) return (ValueSyntaxNode*)addUnexpectedNextTokenError();

				lhs = new MathOperatorSyntaxNode(operatorToken, lhs, rhs);

				continue;
			}

			if (isNextTokenType(TokenType::AndOperator) || isNextTokenType(TokenType::OrOperator))
			{
				auto booleanOperator = next(); // eat && or ||

				ValueSyntaxNode* rhs = parseValueExpression();

				lhs = new BooleanOperatorSyntaxNode(booleanOperator, lhs, rhs);
				continue;
			}

			if (bracketCount > 0 && isNextTokenType(TokenType::CloseBracketSeparator))
			{
				next(); // eat )
				bracketCount--;
				continue;
			}

			break;
		}

		return lhs;
	}

	ReturnSyntaxNode* parseReturnStatement()
	{
		auto returnToken = consume(TokenType::ReturnKeyword);

		if (!returnToken) return nullptr;

		ValueSyntaxNode* value = nullptr;

		if (!end() && peek()->indentDepth >= returnToken->indentDepth)
		{
			value = parseValueExpression();
		}

		return new ReturnSyntaxNode(value);
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

	ValueSyntaxNode* parseValueIndexValue(ValueSyntaxNode* variable)
	{
		if (isNextTokenType(TokenType::OpenSquareBracket))
		{
			next(); // eat [

			ValueSyntaxNode* indexNode = parseValueExpression();

			if (!isNextTokenType(TokenType::CloseSquareBracket)) return (ValueSyntaxNode*)addUnexpectedNextTokenError();

			next(); // eat ]

			return new ValueIndexValue(variable, indexNode);
		}

		return (ValueSyntaxNode*)addUnexpectedNextTokenError();
	}

	ValueSyntaxNode* parseVariableOrFunctionCall(bool asValue, ValueSyntaxNode* instance = nullptr)
	{
		auto name = consume(TokenType::Identifier);

		if (!name) return nullptr;

		ValueSyntaxNode* variable = new VariableSyntaxNode(name, instance);

		while (true)
		{
			if (variable == nullptr) return nullptr;

			if (isNextTokenType(TokenType::DotSeparator))
			{
				next(); // eat .

				if (isNextTokenType(TokenType::Identifier))
				{
					variable = parseVariableOrFunctionCall(asValue, variable);
					continue;
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

				variable = new CallSyntaxNode(instance, name, args);
				continue;
			}

			if (isNextTokenType(TokenType::OpenSquareBracket))
			{
				variable = parseValueIndexValue(variable);
				continue;
			}

			if (asValue)
			{
				break;
			}
			else
			{
				// Variable assignment
				if (isNextTokenType(TokenType::AssignmentOperator))
				{
					next(); // eat =

					ValueSyntaxNode* assignmentValue = parseValueExpression();

					if (!assignmentValue) return nullptr;

					variable = new AssignmentSyntaxNode(variable, assignmentValue);

					continue;
				}

				// Math operators
				if (isNextTokenType(TokenType::Operator))
				{
					Token* operatorToken = next();

					ValueSyntaxNode* rhs = nullptr;

					rhs = parseValueExpression();

					if (!rhs) return (ValueSyntaxNode*)addUnexpectedNextTokenError();

					variable = new MathOperatorSyntaxNode(operatorToken, variable, rhs);

					continue;
				}

				// bitwise operators
				if (isNextTokenType(TokenType::LogicalOrOperator) || isNextTokenType(TokenType::LogicalOrAssignOperator))
				{
					auto operatorToken = next(); // eat | or |=

					ValueSyntaxNode* rhs = parseValueExpression();

					variable = new BitwiseOperatorSyntaxNode(operatorToken, variable, rhs);
					continue;
				}
			}
			break;
		}

		return variable;
	}

	IfSyntaxNode* parseIfStatement()
	{
		auto ifToken = consume(TokenType::IfKeyword); // eat if

		if (!ifToken) return nullptr;

		ValueSyntaxNode* condition = parseValueExpression();

		if (!condition) return nullptr;

		if (!consume(TokenType::ColonSeparator)) return nullptr;

		BodySyntaxNode* thenBody = parseBody(ifToken->indentDepth, ifToken->lineNumber);

		BodySyntaxNode* elseBody = nullptr;

		if (isNextTokenType(TokenType::ElseKeyword))
		{
			auto elseToken = next(); // eat else

			if (!consume(TokenType::ColonSeparator)) return nullptr;

			elseBody = parseBody(elseToken->indentDepth, elseToken->lineNumber);
		}

		return new IfSyntaxNode(condition, thenBody, elseBody);
	}

	WhileSyntaxNode* parseWhileLoop()
	{
		auto whileToken = consume(TokenType::WhileKeyword);

		if (!whileToken) return nullptr;

		auto condition = parseValueExpression();

		if (!consume(TokenType::ColonSeparator)) return nullptr;

		BodySyntaxNode* body = parseBody(whileToken->indentDepth, whileToken->lineNumber);

		if (!body) return nullptr;

		return new WhileSyntaxNode(condition, body);
	}

	ForSyntaxNode* parseForLoop()
	{
		auto forToken = consume(TokenType::ForKeyword);

		if (!forToken) return nullptr;

		auto variableToken = consume(TokenType::Identifier);

		if (!variableToken) return nullptr;

		if (!consume(TokenType::InKeyword)) return nullptr;

		auto arrayToken = parseValueExpression();

		if (!arrayToken) return nullptr;

		if (!consume(TokenType::ColonSeparator)) return nullptr;

		auto body = parseBody(forToken->indentDepth, forToken->lineNumber);

		auto variableDefinition = new VariableDefinitionSyntaxNode(variableToken, nullptr, nullptr, false, false);

		return new ForSyntaxNode(variableDefinition, arrayToken, body);
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
			return parseVariableDefinition(false);
		case TokenType::PassKeyword:
			next(); // eat pass
			return nullptr;
		case TokenType::WhileKeyword:
			return parseWhileLoop();
		case TokenType::ForKeyword:
			return parseForLoop();
		default:
			return addUnexpectedNextTokenError();
		}
	}
};
