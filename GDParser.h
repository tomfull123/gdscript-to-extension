#pragma once

#include "GDTokenStream.h"
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
#include "RangeSyntaxNode.h"
#include "TernarySyntaxNode.h"
#include "CastSyntaxNode.h"
#include "ElseSyntaxNode.h"
#include "ElseIfSyntaxNode.h"
#include "MatchSyntaxNode.h"
#include "MatchCaseSyntaxNode.h"

struct Result
{
	Result(
		AbstractSyntaxTree* ast,
		const std::vector<ParserError>& errors
	) :
		ast(ast),
		errors(errors)
	{
	}

	AbstractSyntaxTree* ast;
	std::vector<ParserError> errors;
};

class GDParser
{
public:
	explicit GDParser(const std::vector<GDToken*>& tokens) :
		stream_(tokens)
	{
	}

	ClassDefinitionSyntaxNode* buildAST(AbstractSyntaxTree* ast, const std::string& cppFilename)
	{
		return parseScriptBody(0, cppFilename);
	}

	const std::vector<ParserError>& getErrors() const { return errors_; }

	static Result* parse(const std::string& input, AbstractSyntaxTree* ast, const std::string& cppFilename, const std::string& filepath)
	{
		GDLexer lexer(input, filepath);

		std::vector<GDToken*> tokens = lexer.readAllTokens();

		GDParser parser(tokens);

		auto classDef = parser.buildAST(ast, cppFilename);

		const auto& errors = parser.getErrors();

		if (errors.empty()) ast->classes.push_back(classDef);

		return new Result(ast, errors);
	}

private:
	GDTokenStream stream_;
	std::vector<ParserError> errors_;

	bool isNextTokenType(GDTokenType type, unsigned int offset = 0)
	{
		if (offset >= stream_.getLength()) return false;
		return stream_.peek(offset)->type == type;
	}

	bool isNextTokenKeyword(const std::string& keyword, unsigned int offset = 0)
	{
		if (offset >= stream_.getLength()) return false;
		auto peek = stream_.peek(offset);
		return peek->type == GDTokenType::IdentifierOrKeyword && peek->value == keyword;
	}

	SyntaxNode* addError(const std::string& error, const GDToken* token)
	{
		if (token)
			errors_.emplace_back(error, token->lineNumber, token->columnNumber, token->filepath);
		else
			errors_.emplace_back(error, stream_.getLastTokenLineNumber(), stream_.getLastTokenColumnNumber(), "");

		if (stream_.end()) return nullptr;

		return nullptr;
	}

	SyntaxNode* addUnexpectedTokenError(const GDToken* token)
	{
		if (token == nullptr) return addError("Unexpected end of file", nullptr);
		return addError("Unexpected token: " + token->value, token);
	}

	SyntaxNode* addUnexpectedNextTokenError()
	{
		return addUnexpectedTokenError(next());
	}

	GDToken* next()
	{
		return stream_.next();
	}

	GDToken* peek(unsigned int offset = 0)
	{
		return stream_.peek(offset);
	}

	bool end() const
	{
		return stream_.end();
	}

	GDToken* consume(GDTokenType tokenType)
	{
		if (isNextTokenType(tokenType))
		{
			return next();
		}

		addUnexpectedNextTokenError();
		return nullptr;
	}

	GDToken* consumeKeyword(const std::string& keyword)
	{
		if (isNextTokenKeyword(keyword))
		{
			return next();
		}

		addUnexpectedNextTokenError();
		return nullptr;
	}

	Type* parseType(const Type* namespaceType = nullptr)
	{
		const GDToken* typeToken = next();
		std::vector<Type*> subtypes = {};

		if (isNextTokenType(GDTokenType::OpenSquareBracket))
		{
			next(); // eat [

			while (!isNextTokenType(GDTokenType::CloseSquareBracket))
			{
				auto subtype = parseType();
				if (subtype) subtypes.push_back(subtype);
				if (isNextTokenType(GDTokenType::CommaSeparator)) next(); // eat ,
			}

			if (!consume(GDTokenType::CloseSquareBracket)) return nullptr;
		}
		else if (isNextTokenType(GDTokenType::DotSeparator))
		{
			consume(GDTokenType::DotSeparator);

			return parseType(new Type(typeToken, {}, namespaceType));
		}

		return new Type(typeToken, subtypes, namespaceType);
	}

	VariableDefinitionSyntaxNode* parseArgDefinition()
	{
		GDToken* name = consume(GDTokenType::IdentifierOrKeyword);

		if (!name) return nullptr;

		Type* type = nullptr;

		if (isNextTokenType(GDTokenType::ColonSeparator))
		{
			next(); // eat :
			if (!isNextTokenType(GDTokenType::AssignmentOperator))
			{
				type = parseType();
			}
		}

		ValueSyntaxNode* assignmentValue = nullptr;

		if (isNextTokenType(GDTokenType::AssignmentOperator))
		{
			next(); // eat =
			assignmentValue = parseValueExpression();
		}

		return new VariableDefinitionSyntaxNode(name, type, assignmentValue, false, false, false, nullptr, nullptr);
	}

	FunctionPrototypeSyntaxNode* parseFunctionProtoype(bool isStatic)
	{
		consumeKeyword("func"); // eat func

		GDToken* name = consume(GDTokenType::IdentifierOrKeyword);

		if (!name) return nullptr;

		if (!consume(GDTokenType::OpenBracketSeparator)) return nullptr;

		std::vector<VariableDefinitionSyntaxNode*> argDefs;

		while (stream_.peek()->type != GDTokenType::CloseBracketSeparator)
		{
			auto* argDef = parseArgDefinition();

			if (argDef)
			{
				argDefs.push_back(argDef);

				if (isNextTokenType(GDTokenType::CommaSeparator)) next(); // eat ,
				else if (!isNextTokenType(GDTokenType::CloseBracketSeparator)) return (FunctionPrototypeSyntaxNode*)addUnexpectedNextTokenError();
			}
			else
			{
				return (FunctionPrototypeSyntaxNode*)addUnexpectedNextTokenError();
			}
		}

		next(); // eat )

		Type* returnType = nullptr;

		if (isNextTokenType(GDTokenType::ArrowSeparator))
		{
			next(); // eat ->
			returnType = parseType();
		}

		if (!consume(GDTokenType::ColonSeparator)) return nullptr;

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
			auto thisToken = new GDToken();
			thisToken->value = "this";
			nodes.push_back(new ReturnSyntaxNode(new VariableSyntaxNode(thisToken, nullptr, false)));
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

	GDToken* parseClassName()
	{
		consumeKeyword("class_name"); // eat class_name

		return consume(GDTokenType::IdentifierOrKeyword);
	}

	VariableDefinitionSyntaxNode* parseVariableDefinition(bool isClassMember, bool isStatic)
	{
		auto varOrConst = next();

		auto name = consume(GDTokenType::IdentifierOrKeyword);

		if (!name) return nullptr;

		Type* type = nullptr;

		if (isNextTokenType(GDTokenType::ColonSeparator))
		{
			next(); // eat :
			if (!isNextTokenType(GDTokenType::AssignmentOperator))
			{
				type = parseType();
			}
		}

		ValueSyntaxNode* assignmentValue = nullptr;

		if (isNextTokenType(GDTokenType::AssignmentOperator))
		{
			next(); // eat =
			assignmentValue = parseValueExpression();
		}

		Token* getterName = nullptr;
		Token* setterName = nullptr;

		if (isClassMember)
		{
			if (isNextTokenType(GDTokenType::ColonSeparator))
			{
				next(); // eat :

				while (true)
				{
					if (isNextTokenKeyword("set"))
					{
						next(); // eat set

						consume(GDTokenType::AssignmentOperator);

						setterName = consume(GDTokenType::IdentifierOrKeyword);

						if (isNextTokenType(GDTokenType::CommaSeparator))
							next(); // eat ,
						else
							break;
					}
					else if (isNextTokenKeyword("get"))
					{
						next(); // eat get

						consume(GDTokenType::AssignmentOperator);

						getterName = consume(GDTokenType::IdentifierOrKeyword);

						if (isNextTokenType(GDTokenType::CommaSeparator))
							next(); // eat ,
						else
							break;
					}
					else break;
				}
			}
			else
			{
				getterName = new Token("get_" + name->value);
				setterName = new Token("set_" + name->value);
			}
		}

		return new VariableDefinitionSyntaxNode(name, type, assignmentValue, varOrConst->value == "const", isClassMember, isStatic, getterName, setterName);
	}

	VariableDefinitionSyntaxNode* parseSignalDefinitions()
	{
		next(); // eat signal

		auto signalName = consume(GDTokenType::IdentifierOrKeyword);

		if (!signalName) return nullptr;

		if (isNextTokenType(GDTokenType::OpenBracketSeparator))
		{
			next(); // eat (

			std::vector<VariableDefinitionSyntaxNode*> args;

			while (!isNextTokenType(GDTokenType::CloseBracketSeparator))
			{
				auto arg = parseArgDefinition();
				if (arg) args.push_back(arg);
			}

			next(); // eat )
		}

		return new VariableDefinitionSyntaxNode(signalName, new Type("Signal"), nullptr, false, true, false, nullptr, nullptr);
	}

	void parseAnnotation()
	{
		next(); // eat warning_ignore or tool

		if (isNextTokenType(GDTokenType::OpenBracketSeparator))
		{
			if (!consume(GDTokenType::OpenBracketSeparator)) return;
			if (!consume(GDTokenType::StringLiteral)) return;
			if (!consume(GDTokenType::CloseBracketSeparator)) return;
		}
	}

	EnumValueSyntaxNode* parseEnumValueDefinition()
	{
		auto name = consume(GDTokenType::IdentifierOrKeyword);
		if (!name) return nullptr;
		return new EnumValueSyntaxNode(name);
	}

	EnumDefinitionSyntaxNode* parseEnumDefinition()
	{
		next(); // eat enum

		auto name = consume(GDTokenType::IdentifierOrKeyword);

		if (!name) return nullptr;

		if (!consume(GDTokenType::OpenCurlyBracketSeparator)) return nullptr;

		std::vector<EnumValueSyntaxNode*> values;

		while (!isNextTokenType(GDTokenType::CloseCurlyBracketSeparator))
		{
			auto value = parseEnumValueDefinition();
			if (value) values.push_back(value);

			if (isNextTokenType(GDTokenType::CommaSeparator)) next(); // eat ,
			else if (!isNextTokenType(GDTokenType::CloseCurlyBracketSeparator)) return (EnumDefinitionSyntaxNode*)addUnexpectedNextTokenError();
		}

		next(); // eat }

		return new EnumDefinitionSyntaxNode(name, values);
	}

	GDToken* parseExtends()
	{
		consumeKeyword("extends"); // eat extends

		return consume(GDTokenType::IdentifierOrKeyword);
	}

	ClassDefinitionSyntaxNode* parseScriptBody(int indentDepth, const std::string& fileName, GDToken* nameToken = nullptr, bool isInnerClass = false, GDToken* overrideExtends = nullptr)
	{
		GDToken* name = nameToken;
		GDToken* extends = overrideExtends;
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
			case GDTokenType::IdentifierOrKeyword:
			{
				const std::string& value = t->value;

				if (value == "class_name") name = parseClassName();
				else if (value == "extends") extends = parseExtends();
				else if (value == "func") memberFunctionDefinitions.push_back(parseFunction(false));
				else if (value == "var" || value == "const") memberVariableDefinitions.push_back(parseVariableDefinition(true, false));
				else if (value == "signal") memberVariableDefinitions.push_back(parseSignalDefinitions());
				else if (value == "enum") enumDefinitions.push_back(parseEnumDefinition());
				else if (value == "static")
				{
					next(); // eat static

					if (isNextTokenKeyword("func"))
					{
						staticFunctionDefinitions.push_back(parseFunction(true));
						break;
					}

					if (isNextTokenKeyword("var") || isNextTokenKeyword("const"))
					{
						staticVariableDefinitions.push_back(parseVariableDefinition(false, true));
						break;
					}

					addUnexpectedNextTokenError();
				}
				else if (value == "class")
				{
					next(); // eat class
					auto subclassName = consume(GDTokenType::IdentifierOrKeyword);
					GDToken* subclassExtends = nullptr;
					if (isNextTokenKeyword("extends")) subclassExtends = parseExtends();
					consume(GDTokenType::ColonSeparator);
					auto internalClass = parseScriptBody(t->indentDepth + 1, "", subclassName, true, subclassExtends);
					if (internalClass) innerClasses.push_back(internalClass);
				}
				else if (value == "pass")
				{
					next(); // eat pass
				}
				else
				{
					return (ClassDefinitionSyntaxNode*)addUnexpectedNextTokenError();
				}
			}
			break;
			case GDTokenType::Annotation:
				parseAnnotation();
				break;
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
			{},
			isInnerClass,
			fileName,
			false
		);
	}

	LiteralValueSyntaxNode* parseLiteralValue()
	{
		auto value = next();
		GDTokenType type = value->type;

		switch (type)
		{
		case GDTokenType::IntLiteral: return new LiteralValueSyntaxNode(value, new Type("int"));
		case GDTokenType::FloatLiteral: return new LiteralValueSyntaxNode(value, new Type("float"));
		case GDTokenType::StringLiteral: return new LiteralValueSyntaxNode(value, new Type("string"));
		}

		return (LiteralValueSyntaxNode*)addUnexpectedTokenError(value);
	}

	NotOperatorSyntaxNode* parseNotOperator()
	{
		auto token = next(); // eat not

		ValueSyntaxNode* condition = parseValueExpression();

		return new NotOperatorSyntaxNode(token, condition);
	}

	BooleanLiteralSyntaxNode* parseBooleanLiteral()
	{
		GDToken* t = next();

		if (t->value == "true") return new BooleanLiteralSyntaxNode(t, true);
		if (t->value == "false") return new BooleanLiteralSyntaxNode(t, false);

		return (BooleanLiteralSyntaxNode*)addUnexpectedTokenError(t);
	}

	ValueSyntaxNode* parseArrayValue()
	{
		if (!consume(GDTokenType::OpenSquareBracket)) return nullptr;

		std::vector<ValueSyntaxNode*> expressions;

		while (!isNextTokenType(GDTokenType::CloseSquareBracket))
		{
			auto ex = parseValueExpression();
			if (ex) expressions.push_back(ex);

			if (isNextTokenType(GDTokenType::CommaSeparator)) next(); // eat ,
			else if (!isNextTokenType(GDTokenType::CloseSquareBracket)) return (ValueSyntaxNode*)addUnexpectedNextTokenError();
		}

		next(); // eat ]

		return new ArrayValueSyntaxNode(expressions);
	}

	ValueSyntaxNode* parseDictionaryValue()
	{
		next(); // eat {
		std::vector<ValueSyntaxNode*> keys;
		std::vector<ValueSyntaxNode*> values;

		while (!isNextTokenType(GDTokenType::CloseCurlyBracketSeparator))
		{
			auto key = parseSingleValueObject();

			if (!consume(GDTokenType::ColonSeparator)) return nullptr;

			auto value = parseValueExpression();
			if (value)
			{
				keys.push_back(key);
				values.push_back(value);
			}

			if (isNextTokenType(GDTokenType::CommaSeparator)) next(); // eat ,
			else if (!isNextTokenType(GDTokenType::CloseCurlyBracketSeparator)) return (ValueSyntaxNode*)addUnexpectedNextTokenError();
		}

		next(); // eat }

		return new DictionaryValueSyntaxNode(keys, values);
	}

	ValueSyntaxNode* parsePreload()
	{
		if (!consumeKeyword("preload")) return nullptr;
		if (!consume(GDTokenType::OpenBracketSeparator)) return nullptr;
		auto preloadPath = consume(GDTokenType::StringLiteral);

		if (!preloadPath) return nullptr;

		if (!consume(GDTokenType::CloseBracketSeparator)) return nullptr;

		return new PreloadSyntaxNode(preloadPath);
	}

	NullSyntaxNode* parseNullValue()
	{
		if (!consumeKeyword("null")) return nullptr;

		return new NullSyntaxNode();
	}

	RangeSyntaxNode* parseRange()
	{
		if (!consumeKeyword("range")) return nullptr;
		if (!consume(GDTokenType::OpenBracketSeparator)) return nullptr;

		ValueSyntaxNode* startValue = parseValueExpression();

		if (!consume(GDTokenType::CommaSeparator)) return nullptr;

		ValueSyntaxNode* endValue = parseValueExpression();

		if (isNextTokenType(GDTokenType::CommaSeparator)) next();

		ValueSyntaxNode* incrementValue = nullptr;

		if (!isNextTokenType(GDTokenType::CloseBracketSeparator))
		{
			incrementValue = parseValueExpression();
		}

		if (!consume(GDTokenType::CloseBracketSeparator)) return nullptr;

		return new RangeSyntaxNode(startValue, endValue, incrementValue);
	}

	CastSyntaxNode* parseCast(ValueSyntaxNode* value)
	{
		if (!consumeKeyword("as")) return nullptr;

		auto type = parseType();

		if (!type) return nullptr;

		return new CastSyntaxNode(value, type);
	}

	ValueSyntaxNode* parseSingleValueObject()
	{
		const GDToken* token = peek();
		GDTokenType type = token->type;

		switch (type)
		{
		case GDTokenType::IntLiteral:
		case GDTokenType::FloatLiteral:
		case GDTokenType::StringLiteral:
		{
			auto literalValue = parseLiteralValue();

			return literalValue;
		}
		case GDTokenType::IdentifierOrKeyword:
		{
			const std::string& value = token->value;

			if (value == "true" || value == "false") return parseBooleanLiteral();
			else if (value == "preload") return parsePreload();
			else if (value == "null") return parseNullValue();
			else if (value == "range" && isNextTokenType(GDTokenType::OpenBracketSeparator, 1)) return parseRange();
			else if (value == "not") return parseNotOperator();

			return parseVariableOrFunctionCall(true);
		}
		case GDTokenType::NotOperator: return parseNotOperator();
		case GDTokenType::OpenSquareBracket: return parseArrayValue();
		case GDTokenType::OpenCurlyBracketSeparator: return parseDictionaryValue();
		case GDTokenType::Operator:
		{
			auto operatorToken = next(); // eat operator

			return new PrefixOperator(operatorToken, parseSingleValueObject());
		}
		}

		return (ValueSyntaxNode*)addUnexpectedNextTokenError();
	}

	ValueSyntaxNode* parseValueExpression()
	{
		int bracketCount = 0;

		while (isNextTokenType(GDTokenType::OpenBracketSeparator))
		{
			next(); // eat (
			bracketCount++;
		}

		const GDToken* name = peek();

		ValueSyntaxNode* lhs = parseSingleValueObject();

		if (!lhs) return (ValueSyntaxNode*)addUnexpectedNextTokenError();

		while (true)
		{
			// Bool operators
			if (isNextTokenType(GDTokenType::EqualityOperator) || isNextTokenType(GDTokenType::NotEqualityOperator)
				|| isNextTokenType(GDTokenType::GreaterThanSeparator) || isNextTokenType(GDTokenType::GreaterThanEqualSeparator)
				|| isNextTokenType(GDTokenType::LessThanSeparator) || isNextTokenType(GDTokenType::LessThanEqualSeparator))
			{
				GDToken* boolOperator = next();

				ValueSyntaxNode* rhs = parseValueExpression();

				if (!rhs) return nullptr;

				lhs = new EqualityOperatorSyntaxNode(boolOperator, lhs, rhs);

				continue;
			}

			if (isNextTokenKeyword("is"))
			{
				auto isToken = next();

				Type* type = parseType();

				lhs = new IsOperatorSyntaxNode(isToken, lhs, type);

				continue;
			}

			// Math operators
			if (isNextTokenType(GDTokenType::Operator))
			{
				GDToken* operatorToken = next();

				ValueSyntaxNode* rhs = nullptr;

				rhs = parseValueExpression();

				if (!rhs) return (ValueSyntaxNode*)addUnexpectedNextTokenError();

				lhs = new MathOperatorSyntaxNode(operatorToken, lhs, rhs);

				continue;
			}

			if (isNextTokenType(GDTokenType::AndOperator) || isNextTokenType(GDTokenType::OrOperator) || isNextTokenKeyword("or"))
			{
				auto booleanOperator = next(); // eat && or ||

				if (booleanOperator->value == "or") booleanOperator->type = GDTokenType::OrOperator;

				ValueSyntaxNode* rhs = parseValueExpression();

				lhs = new BooleanOperatorSyntaxNode(booleanOperator, lhs, rhs);
				continue;
			}

			if (isNextTokenKeyword("if") && peek()->lineNumber == name->lineNumber)
			{
				lhs = parseTernaryValue(lhs);
				continue;
			}

			if (isNextTokenKeyword("as"))
			{
				lhs = parseCast(lhs);
				continue;
			}

			if (bracketCount > 0 && isNextTokenType(GDTokenType::CloseBracketSeparator))
			{
				next(); // eat )
				bracketCount--;
				continue;
			}

			if (isNextTokenType(GDTokenType::DotSeparator))
			{
				next(); // eat .

				if (isNextTokenType(GDTokenType::IdentifierOrKeyword))
				{
					lhs = parseVariableOrFunctionCall(true, lhs);
					continue;
				}
			}

			break;
		}

		return lhs;
	}

	ReturnSyntaxNode* parseReturnStatement()
	{
		auto returnToken = consumeKeyword("return");

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
		if (!consumeKeyword("break")) return nullptr;

		return new BreakSyntaxNode();
	}

	ContinueSyntaxNode* parseContinueStatement()
	{
		if (!consumeKeyword("continue")) return nullptr;

		return new ContinueSyntaxNode();
	}

	ValueSyntaxNode* parseValueIndexValue(ValueSyntaxNode* variable)
	{
		if (isNextTokenType(GDTokenType::OpenSquareBracket))
		{
			next(); // eat [

			ValueSyntaxNode* indexNode = parseValueExpression();

			if (!isNextTokenType(GDTokenType::CloseSquareBracket)) return (ValueSyntaxNode*)addUnexpectedNextTokenError();

			next(); // eat ]

			return new ValueIndexValue(variable, indexNode);
		}

		return (ValueSyntaxNode*)addUnexpectedNextTokenError();
	}

	ValueSyntaxNode* parseTernaryValue(ValueSyntaxNode* thenValue)
	{
		if (!consumeKeyword("if")) return nullptr;

		ValueSyntaxNode* condition = parseValueExpression();

		if (!condition) return nullptr;

		if (!consumeKeyword("else")) return nullptr;

		ValueSyntaxNode* elseValue = parseValueExpression();

		return new TernarySyntaxNode(condition, thenValue, elseValue);
	}

	ValueSyntaxNode* parseVariableOrFunctionCall(bool asValue, ValueSyntaxNode* instance = nullptr)
	{
		auto name = consume(GDTokenType::IdentifierOrKeyword);

		if (!name) return nullptr;

		if (!asValue && instance && isNextTokenType(GDTokenType::DotSeparator))
		{
			asValue = true; // getter
		}

		ValueSyntaxNode* variable = new VariableSyntaxNode(name, instance, asValue);

		while (true)
		{
			if (variable == nullptr) return nullptr;

			if (isNextTokenType(GDTokenType::DotSeparator))
			{
				next(); // eat .

				if (isNextTokenType(GDTokenType::IdentifierOrKeyword))
				{
					variable = parseVariableOrFunctionCall(asValue, variable);
					continue;
				}
			}

			// Function call
			if (isNextTokenType(GDTokenType::OpenBracketSeparator))
			{
				next(); // eat (

				std::vector<ValueSyntaxNode*> args;

				while (!isNextTokenType(GDTokenType::CloseBracketSeparator))
				{
					auto e = parseValueExpression();
					if (e)
					{
						args.push_back(e);

						if (isNextTokenType(GDTokenType::CommaSeparator)) next(); // eat ,
						else if (!isNextTokenType(GDTokenType::CloseBracketSeparator)) return (ValueSyntaxNode*)addUnexpectedNextTokenError();
					}
					else
					{
						return (ValueSyntaxNode*)addUnexpectedNextTokenError();
					}
				}

				if (!consume(GDTokenType::CloseBracketSeparator)) return nullptr;

				variable = new CallSyntaxNode(instance, name, args);
				continue;
			}

			if (isNextTokenType(GDTokenType::OpenSquareBracket))
			{
				variable = parseValueIndexValue(variable);
				continue;
			}

			if (isNextTokenKeyword("if") && peek()->lineNumber == name->lineNumber)
			{
				variable = parseTernaryValue(variable);
				continue;
			}

			if (asValue)
			{
				break;
			}
			else
			{
				// Variable assignment
				if (isNextTokenType(GDTokenType::AssignmentOperator))
				{
					next(); // eat =

					ValueSyntaxNode* assignmentValue = parseValueExpression();

					if (!assignmentValue) return nullptr;

					variable = new AssignmentSyntaxNode(variable, assignmentValue);

					continue;
				}

				// Math operators
				if (isNextTokenType(GDTokenType::Operator))
				{
					GDToken* operatorToken = next();

					ValueSyntaxNode* rhs = nullptr;

					rhs = parseValueExpression();

					if (!rhs) return (ValueSyntaxNode*)addUnexpectedNextTokenError();

					variable = new MathOperatorSyntaxNode(operatorToken, variable, rhs);

					continue;
				}

				// bitwise operators
				if (isNextTokenType(GDTokenType::LogicalOrOperator) || isNextTokenType(GDTokenType::LogicalOrAssignOperator))
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
		const GDToken* ifToken;

		if (isNextTokenKeyword("if"))
			ifToken = consumeKeyword("if"); // eat if
		else
			ifToken = consumeKeyword("elif"); // eat elif

		if (!ifToken) return nullptr;

		ValueSyntaxNode* condition = parseValueExpression();

		if (!condition) return nullptr;

		if (!consume(GDTokenType::ColonSeparator)) return nullptr;

		BodySyntaxNode* thenBody = parseBody(ifToken->indentDepth, ifToken->lineNumber);

		SyntaxNode* elseBody = nullptr;

		if (isNextTokenKeyword("elif"))
		{
			elseBody = new ElseIfSyntaxNode(parseIfStatement());
		}
		else if (isNextTokenKeyword("else"))
		{
			auto elseToken = next(); // eat else

			if (!consume(GDTokenType::ColonSeparator)) return nullptr;

			elseBody = new ElseSyntaxNode(parseBody(elseToken->indentDepth, elseToken->lineNumber));
		}

		return new IfSyntaxNode(condition, thenBody, elseBody);
	}

	WhileSyntaxNode* parseWhileLoop()
	{
		auto whileToken = consumeKeyword("while");

		if (!whileToken) return nullptr;

		auto condition = parseValueExpression();

		if (!consume(GDTokenType::ColonSeparator)) return nullptr;

		BodySyntaxNode* body = parseBody(whileToken->indentDepth, whileToken->lineNumber);

		if (!body) return nullptr;

		return new WhileSyntaxNode(condition, body);
	}

	ForSyntaxNode* parseForLoop()
	{
		auto forToken = consumeKeyword("for");

		if (!forToken) return nullptr;

		auto variableToken = consume(GDTokenType::IdentifierOrKeyword);

		if (!variableToken) return nullptr;

		Type* variableType = nullptr;

		if (isNextTokenType(GDTokenType::ColonSeparator))
		{
			next(); // eat :

			variableType = parseType();
		}

		if (!consumeKeyword("in")) return nullptr;

		auto arrayToken = parseValueExpression();

		if (!arrayToken) return nullptr;

		if (!consume(GDTokenType::ColonSeparator)) return nullptr;

		auto body = parseBody(forToken->indentDepth, forToken->lineNumber);

		auto variableDefinition = new VariableDefinitionSyntaxNode(variableToken, variableType, nullptr, false, false, false, nullptr, nullptr);

		return new ForSyntaxNode(variableDefinition, arrayToken, body);
	}

	MatchCaseSyntaxNode* parseMatchCase()
	{
		std::vector<ValueSyntaxNode*> values;

		auto p = peek();

		while (!isNextTokenType(GDTokenType::ColonSeparator))
		{
			ValueSyntaxNode* value = parseValueExpression();

			if (!value) return nullptr;

			values.push_back(value);

			if (isNextTokenType(GDTokenType::CommaSeparator)) consume(GDTokenType::CommaSeparator);
			else break;
		}

		if (!consume(GDTokenType::ColonSeparator)) return nullptr;

		BodySyntaxNode* body = parseBody(p->indentDepth, p->lineNumber);

		return new MatchCaseSyntaxNode(values, body);
	}

	MatchSyntaxNode* parseMatch()
	{
		auto matchToken = consumeKeyword("match");

		ValueSyntaxNode* condition = parseValueExpression();

		if (!condition) return nullptr;

		if (!consume(GDTokenType::ColonSeparator)) return nullptr;

		std::vector<MatchCaseSyntaxNode*> cases;

		while (!end() && peek()->indentDepth > matchToken->indentDepth)
		{
			cases.push_back(parseMatchCase());
		}

		return new MatchSyntaxNode(condition, cases);
	}

	SyntaxNode* parseExpression()
	{
		const GDToken* t = stream_.peek();
		const std::string& value = t->value;

		if (t->type == GDTokenType::IdentifierOrKeyword) {
			if (value == "return") return parseReturnStatement();
			else if (value == "break") return parseBreakStatement();
			else if (value == "continue") return parseContinueStatement();
			else if (value == "if") return parseIfStatement();
			else if (value == "var" || value == "const") return parseVariableDefinition(false, false);
			else if (value == "pass")
			{
				next(); // eat pass
				return nullptr;
			}
			else if (value == "while") return parseWhileLoop();
			else if (value == "for") return parseForLoop();
			else if (value == "match") return parseMatch();
			else return parseVariableOrFunctionCall(false);
		}

		return addUnexpectedNextTokenError();
	}
};
