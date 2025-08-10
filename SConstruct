#!/usr/bin/env python
import os
import sys


#set arguments for compile
ARGUMENTS["disable_exceptions"]=False #use exceptions
if ARGUMENTS.get("platform", None) == "web":
    ARGUMENTS.setdefault("threads", "no") #don't use threads if the platform is web

env = SConscript("external_libraries/godot-cpp/SConstruct")

#compile_commands.json generator for clang
env.Tool('compilation_db')
cdb = env.CompilationDatabase('compile_commands.json')
Alias('compile_commands', cdb)



# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags


##PATHS

sources = Glob("src/*.cpp")
miniscript_sources = Glob("external_libraries/miniscript/MiniScript-cpp/src/MiniScript/*.cpp")
sources += miniscript_sources

env.Append(CPPPATH=["src/"])
env.Append(CPPPATH=['external_libraries/miniscript/MiniScript-cpp/src/MiniScript'])



##FLAGS

if env["platform"] == "windows": #make clang act like MSVC if the compiler is clang
    if "clang" in env["CXX"]:
        env.Append(CCFLAGS=["-fms-extensions"])


#Apple dependency related(needed for all gdextensions)
if env["platform"] == "macos":
    library = env.SharedLibrary(
        "demo/bin/libgdxms.{}.{}.framework/libgdxms.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "demo/bin/libgdxms.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "demo/bin/libgdxms.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )
else:
    library = env.SharedLibrary(
        "demo/bin/libgdxms{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
