#!/usr/bin/env python
import os
import sys

env = SConscript("external_libraries/godot-cpp/SConstruct")

# For reference:
# - CCFLAGS are compilation flags shared between C and C++
# - CFLAGS are for C-specific compilation flags
# - CXXFLAGS are for C++-specific compilation flags
# - CPPFLAGS are for pre-processor flags
# - CPPDEFINES are for pre-processor defines
# - LINKFLAGS are for linking flags

# tweak this if you want to use different folders, or more folders, to store your source code in.
env.Append(CPPPATH=["src/"])
env.Append(CPPPATH=['external_libraries/miniscript/MiniScript-cpp/src/MiniScript'])

env.Append(CCFLAGS=["-fexceptions"])
env.Append(CCFLAGS=["-Wno-cpp"]) #anoying warnings
env.Append(CCFLAGS=["-fpermissive"])

sources = Glob("src/*.cpp")
miniscript_sources = Glob("external_libraries/miniscript/MiniScript-cpp/src/MiniScript/*.cpp")
sources += miniscript_sources

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
