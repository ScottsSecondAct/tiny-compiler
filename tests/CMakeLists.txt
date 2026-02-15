# Unit tests using GoogleTest (or catch2, adjust as preferred)
# find_package(GTest REQUIRED)
#
# add_executable(tiny_tests
#     unit/test_lexer.cpp
#     unit/test_parser.cpp
#     unit/test_ast.cpp
#     unit/test_semantic.cpp
#     unit/test_codegen.cpp
# )
# target_link_libraries(tiny_tests GTest::gtest_main)
# gtest_discover_tests(tiny_tests)

# End-to-end tests
add_test(
    NAME integration_tests
    COMMAND ${CMAKE_SOURCE_DIR}/tests/programs/run_tests.py
        --compiler $<TARGET_FILE:tinyc>
        --runtime $<TARGET_FILE:tiny_runtime>
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
