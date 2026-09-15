#pragma once

#include "Lexer.h"
#include <vector>

class RpcSyntaxNode
{
public:
	explicit RpcSyntaxNode(const std::vector<Token*>& args) :
		args_(args)
	{
	}

	std::string toCpp(const std::string& functionName, CppData* data, const std::string& indents)
	{
		std::string variableName = functionName + "RpcConfig";
		std::string code = indents + "Dictionary " + variableName + ";";

		for (int i = 0; i < args_.size(); i++)
		{
			std::string argValue = args_[i]->value;

			if (i == 0)
			{
				data->currentClass->types.emplace("MultiplayerAPI");
				code += indents + variableName + "[\"rpc_mode\"] = " + mapRpcMode(argValue) + ";\n";
			}
			if (i == 1) code += indents + variableName + "[\"call_local\"] = " + mapSync(argValue) + ";\n";
			if (i == 2)
			{
				data->currentClass->types.emplace("MultiplayerPeer");
				code += indents + variableName + "[\"transfer_mode\"] = " + mapTransferMode(argValue) + ";\n";
			}
			if (i == 3) code += indents + variableName + "[\"channel\"] = " + argValue + ";\n";
		}

		code += indents + "Node::rpc_config(\"" + functionName + "\", " + variableName + ");";

		return code;
	}

private:
	std::vector<Token*> args_;

	std::string mapRpcMode(const std::string& value) const
	{
		if (value == "any_peer") return "MultiplayerAPI::RPC_MODE_ANY_PEER";
		if (value == "authority") return "MultiplayerAPI::RPC_MODE_AUTHORITY";

		return value;
	}

	std::string mapTransferMode(const std::string& value) const
	{
		if (value == "reliable") return "MultiplayerPeer::TRANSFER_MODE_RELIABLE";
		if (value == "unreliable") return "MultiplayerPeer::TRANSFER_MODE_UNRELIABLE";
		if (value == "unreliable_ordered") return "MultiplayerPeer::TRANSFER_MODE_UNRELIABLE_ORDERED";

		return value;
	}

	std::string mapSync(const std::string& value)
	{
		if (value == "call_local") return "true";
		return "false";
	}
};