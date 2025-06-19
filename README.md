# GDScript to Extension Transpiler

A tool for transpiling GDScripts into GDExtension files

## Installing

Download release (currently only windows)
Extract `gdtoex` folder to a tools folder (e.g. c:/dev-tools/)
Add `gdtoex` folder to `PATH` system variable (e.g. c:/dev-tools/gdtoex)

## How to Use

It's recommended that you set the `Untyped Declaration` option to `error` in your godot project (Project > Project Settings (Advanced Settings) > Debug > GDScript > Untyped Declaration)
and fix any type errors first

Prefix any GDScript class names with an underscore to have them exported as GDExtension classes,
this is to make it a little easier to switch between GDScript and GDExtension versions.

### Running tool

Setup GDExtension
Open command line at the project path (c:/dev/godot/project1)
Run `gdtoex` command
GDExtension files should be written to output path
Run `scons` to compile the GDExtension
Run godot project

## Args

Project Path `-p c:/dev/godot/project1` Override the project path (defaults to current directory)
Output Path `-o ./cpp/src` Overrides the output path (defaults to ./cpp/src)

## Known Issues

Underscoring `node` classes won't replace the GDScript node with a GDExtension node in the scene tree, you will manually have to delete and add them
There is an issue with nested array initialisers - something that will need to be fixed in Godot
