#pragma once

#include <string>
#include "CPPModule.h"

class RegisterTypesGenerator
{
public:
	static const std::string filename;

	static std::string generateHeader()
	{
		return
			"#pragma once\n"
			"\n"
			"#include <godot_cpp/godot.hpp>\n"
			"\n"
			"using namespace godot;\n"
			"\n"
			"void initialize_module(ModuleInitializationLevel p_level);\n"
			"void uninitialize_module(ModuleInitializationLevel p_level);\n";
	}

	static std::string generateSource(const CPPModule& cppModule)
	{
		std::string includes;
		std::string registerClasses;

		includes += "#include \"" + filename + ".h\"\n\n";

		for (const auto& classFile : cppModule.classes)
		{
			includes += "#include \"" + classFile.className + ".h\"\n";
			registerClasses += "\tGDREGISTER_CLASS(" + classFile.className + ")\n";
		}

		std::string code;

		return includes
			+ "\n"
			+ "using namespace godot;\n\n"
			+ "void initialize_module(ModuleInitializationLevel p_level)\n"
			+ "{\n"
			+ "\tif (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;\n\n"
			+ registerClasses
			+ "}\n\n"
			+ "void uninitialize_module(ModuleInitializationLevel p_level)\n"
			+ "{\n"
			+ "\tif (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) return;\n"
			+ "}\n\n"
			+ "extern \"C\"\n"
			+ "{\n"
			+ "\tGDExtensionBool GDE_EXPORT library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)\n"
			+ "\t{\n"
			+ "\t\tGDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);\n\n"
			+ "\t\tinit_obj.register_initializer(initialize_module);\n"
			+ "\t\tinit_obj.register_terminator(uninitialize_module);\n"
			+ "\t\tinit_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);\n\n"
			+ "\t\treturn init_obj.init();\n"
			+ "\t}\n"
			+ "}";
	}
};

const std::string RegisterTypesGenerator::filename = "RegisterTypes";
