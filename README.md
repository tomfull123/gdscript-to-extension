# GDScript to Extension Transpiler

A tool for transpiling GDScripts into GDExtension files

## Installing

Download release (currently only windows)<br>
Extract `gdtoex` folder to a tools folder (e.g. c:/dev-tools/)<br>
Add `gdtoex` folder to `PATH` system variable (e.g. c:/dev-tools/gdtoex)<br>

## How to Use

It's recommended that you set the `Untyped Declaration` option to `error` in your godot project (Project > Project Settings (Advanced Settings) > Debug > GDScript > Untyped Declaration)
and fix any type errors first

Prefix any GDScript class names with an underscore to have them exported as GDExtension classes,
this is to make it a little easier to switch between GDScript and GDExtension versions.

### Running tool

Setup GDExtension<br>
Open command line at the project path (c:/dev/godot/project1)<br>
Run `gdtoex` command<br>
GDExtension files should be written to output path<br>
Run `scons` to compile the GDExtension<br>
Run godot project<br>

## Arguments

Project Path `-p c:/dev/godot/project1` Override the project path (defaults to current directory)<br>
Output Path `-o ./cpp/src` Overrides the output path (defaults to ./cpp/src)<br>

## Known Issues

Underscoring `node` classes won't replace the GDScript node with a GDExtension node in the scene tree, you will manually have to delete and add them<br>
There is an issue with nested array initialisers - something that will need to be fixed in Godot<br>
