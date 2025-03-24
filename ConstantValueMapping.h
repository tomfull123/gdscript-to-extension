#pragma once

#include "CppData.h"
#include "CppClassData.h"

class ConstantValueMapping
{
public:
	ConstantValueMapping(
		Token* name,
		Token* value
	) :
		name_(name),
		value_(value)
	{
	}

	void hoist(CppData* data) const
	{
		data->currentClass->constantValueMappings[name_->value] = value_->value;
	}

private:
	Token* name_;
	Token* value_;
};