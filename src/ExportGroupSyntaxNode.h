#pragma once

#include <string>

class ExportGroupSyntaxNode
{
public:
	ExportGroupSyntaxNode(const std::string& name, const std::string& prefix, bool isSubgroup) :
		name_(name),
		prefix_(prefix),
		isSubgroup_(isSubgroup)
	{
	}

	std::string toCpp(const std::string& className, const std::string& indents) const
	{
		std::string methodName = isSubgroup_ ? "add_property_subgroup" : "add_property_group";

		return indents + "ClassDB::" + methodName + "(\"" + className + "\", \"" + name_ + "\", \"" + prefix_ + "\");";
	}

private:
	std::string name_;
	std::string prefix_;
	bool isSubgroup_;
};