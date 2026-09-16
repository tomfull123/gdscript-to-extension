#include "VariableDefinitionSyntaxNode.h"
#include "FunctionDefinitionSyntaxNode.h"

void VariableDefinitionSyntaxNode::hoist(CppData* data)
{
	if (isClassMember_ || isStatic_) data->currentClass->memberVariableDefinitions[name_->value] = this;
	else data->currentClass->currentFunction->variableDefinitions[name_->value] = this;
	if (initialValue_) initialValue_->hoist(data);
}

void VariableDefinitionSyntaxNode::resolveDefinitions(CppData* data)
{
	if (initialValue_) initialValue_->resolveDefinitions(data);
}

void VariableDefinitionSyntaxNode::resolveTypes(CppData* data, Type* otherType)
{
	if (initialValue_)
	{
		initialValue_->resolveTypes(data);

		if (!dataType_) dataType_ = initialValue_->getType();
		else
		{
			auto initialValueType = initialValue_->getType();
			if (initialValueType && dataType_->subtypes.size() < initialValueType->subtypes.size())
			{
				dataType_ = initialValueType;
			}
		}
	}

	if (!dataType_) dataType_ = otherType;
}