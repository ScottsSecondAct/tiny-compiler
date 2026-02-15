cmake_minimum_required(VERSION 3.20)
#project(TinyCompiler VERSION 0.1.0 LANGUAGES CXX)
project(TinyCompiler VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── ANTLR4 Grammar Generation ───────────────────────────────────────────────
# Adjust ANTLR4_JAR to your local path
set(ANTLR4_JAR "$ENV{HOME}/lib/antlr-4.13.2-complete.jar" CACHE FILEPATH "Path to ANTLR4 jar")
set(ANTLR4_GENERATED_DIR "${CMAKE_SOURCE_DIR}/generated")

add_custom_command(
    OUTPUT
        ${ANTLR4_GENERATED_DIR}/TinyLexer.cpp
        ${ANTLR4_GENERATED_DIR}/TinyLexer.h
        ${ANTLR4_GENERATED_DIR}/TinyParser.cpp
        ${ANTLR4_GENERATED_DIR}/TinyParser.h
        ${ANTLR4_GENERATED_DIR}/TinyVisitor.h
        ${ANTLR4_GENERATED_DIR}/TinyBaseVisitor.h
        ${ANTLR4_GENERATED_DIR}/TinyBaseVisitor.cpp
    COMMAND java -jar ${ANTLR4_JAR}
        -Dlanguage=Cpp
        -visitor
        -no-listener
        -o ${ANTLR4_GENERATED_DIR}
        ${CMAKE_SOURCE_DIR}/grammar/Tiny.g4
    DEPENDS ${CMAKE_SOURCE_DIR}/grammar/Tiny.g4
    COMMENT "Generating ANTLR4 C++ lexer/parser from Tiny.g4"
)

add_custom_target(antlr4_generate
    DEPENDS ${ANTLR4_GENERATED_DIR}/TinyLexer.cpp
)

# ── ANTLR4 Runtime ──────────────────────────────────────────────────────────
find_library(ANTLR4_RUNTIME antlr4-runtime)
find_path(ANTLR4_INCLUDE antlr4-runtime.h PATH_SUFFIXES antlr4-runtime)

# ── LLVM ─────────────────────────────────────────────────────────────────────
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
llvm_map_components_to_libnames(LLVM_LIBS
    core
    support
    irreader
    passes
    native
    orcjit
)

# ── Runtime Library ──────────────────────────────────────────────────────────
add_subdirectory(runtime)

# ── Compiler Executable ──────────────────────────────────────────────────────
add_executable(tinyc
    src/main.cpp
    src/AST.cpp
    src/ASTBuilder.cpp
    src/ASTPrinter.cpp
    src/SemanticAnalyzer.cpp
    src/SymbolTable.cpp
    src/TypeSystem.cpp
    src/CodeGen.cpp
    src/Diagnostics.cpp
    ${ANTLR4_GENERATED_DIR}/TinyLexer.cpp
    ${ANTLR4_GENERATED_DIR}/TinyParser.cpp
    ${ANTLR4_GENERATED_DIR}/TinyBaseVisitor.cpp
)

add_dependencies(tinyc antlr4_generate)

target_include_directories(tinyc PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${ANTLR4_GENERATED_DIR}
    ${ANTLR4_INCLUDE}
    ${LLVM_INCLUDE_DIRS}
)

target_compile_definitions(tinyc PRIVATE ${LLVM_DEFINITIONS})

target_link_libraries(tinyc PRIVATE
    ${ANTLR4_RUNTIME}
    ${LLVM_LIBS}
)

# ── Tests ────────────────────────────────────────────────────────────────────
option(TINY_BUILD_TESTS "Build unit tests" ON)
if(TINY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
