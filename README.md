# Gdscript to Extension transpiler

## Installing

Download release
Extract `gdtoex` folder to a tools folder (e.g. c:/dev-tools/)
Add `gdtoex` folder to `PATH` system variable (e.g. c:/dev-tools/gdtoex)

## How to Use

It's recommended that you set the `Untyped Declaration` option to `error` in your godot project (Project > Project Settings (Advanced Settings) > Debug > GDScript > Untyped Declaration)
and fix any type errors first

### Running tool

Setup GDExtension
Open command line at the project path (c:/dev/godot/project1)
Run `gdtoex` command
GDExtension files should be written to output path

## Args

Project Path `-p c:/dev/godot/project1` Override the project path (defaults to current directory)
Output Path `-o ./cpp/src` Overrides the output path (defaults to ./cpp/src)
