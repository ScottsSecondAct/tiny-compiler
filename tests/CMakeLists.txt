# ── Unit Tests (GoogleTest) ────────────────────────────────────────────────────
# GTest is loaded from the extracted package at /tmp/gtestdev (same pattern as
# the LLVM_DIR workaround). Re-extract if missing after a reboot:
#   apt-get download libgtest-dev
#   dpkg-deb -x /tmp/libgtest-dev_*.deb /tmp/gtestdev
set(GTEST_ROOT "/tmp/gtestdev/usr" CACHE PATH "Root of extracted libgtest-dev package")

find_library(GTEST_LIB     gtest      HINTS "${GTEST_ROOT}/lib/x86_64-linux-gnu" NO_DEFAULT_PATH)
find_library(GTEST_MAIN    gtest_main HINTS "${GTEST_ROOT}/lib/x86_64-linux-gnu" NO_DEFAULT_PATH)
find_path(   GTEST_INCLUDE gtest/gtest.h HINTS "${GTEST_ROOT}/include" NO_DEFAULT_PATH)

if(GTEST_LIB AND GTEST_MAIN AND GTEST_INCLUDE)
    message(STATUS "Found GoogleTest: ${GTEST_LIB}")

    add_executable(tiny_tests
        unit/test_ast.cpp
        unit/test_lexer.cpp
        unit/test_parser.cpp
        unit/test_semantic.cpp
        # Compiler sources — no main.cpp, no CodeGen.cpp (avoids LLVM dependency)
        ${CMAKE_SOURCE_DIR}/src/AST.cpp
        ${CMAKE_SOURCE_DIR}/src/ASTBuilder.cpp
        ${CMAKE_SOURCE_DIR}/src/ASTPrinter.cpp
        ${CMAKE_SOURCE_DIR}/src/ModuleLoader.cpp
        ${CMAKE_SOURCE_DIR}/src/SemanticAnalyzer.cpp
        ${CMAKE_SOURCE_DIR}/src/SymbolTable.cpp
        ${CMAKE_SOURCE_DIR}/src/TypeSystem.cpp
        ${CMAKE_SOURCE_DIR}/src/Diagnostics.cpp
        ${ANTLR4_GENERATED_DIR}/TinyLexer.cpp
        ${ANTLR4_GENERATED_DIR}/TinyParser.cpp
        ${ANTLR4_GENERATED_DIR}/TinyBaseVisitor.cpp
    )

    add_dependencies(tiny_tests antlr4_generate)

    target_include_directories(tiny_tests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${ANTLR4_GENERATED_DIR}
        ${ANTLR4_INCLUDE}
        ${GTEST_INCLUDE}
    )

    target_compile_definitions(tiny_tests PRIVATE
        TINY_TEST_DATA_DIR="${CMAKE_SOURCE_DIR}/tests/programs"
    )

    target_link_libraries(tiny_tests PRIVATE
        ${GTEST_MAIN}
        ${GTEST_LIB}
        ${ANTLR4_RUNTIME}
        pthread
    )

    include(GoogleTest)
    gtest_discover_tests(tiny_tests)

    message(STATUS "Unit tests enabled — run with: ./build/tests/tiny_tests")
else()
    message(STATUS "GoogleTest not found at ${GTEST_ROOT} — unit tests disabled.")
    message(STATUS "  To enable: apt-get download libgtest-dev && dpkg-deb -x /tmp/libgtest-dev_*.deb /tmp/gtestdev")
endif()

# ── End-to-end integration tests ──────────────────────────────────────────────
add_test(
    NAME integration_tests
    COMMAND ${CMAKE_SOURCE_DIR}/tests/programs/run_tests.py
        --compiler $<TARGET_FILE:tinyc>
        --runtime $<TARGET_FILE:tiny_runtime>
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
