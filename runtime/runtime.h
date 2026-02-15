#pragma once
#include <cstdint>

// These functions are declared as extern in LLVM IR and linked at compile time.
extern "C" {
    void tiny_print_int(int64_t value);
    void tiny_print_float(double value);
    void tiny_print_str(const char* value);
    void tiny_print_bool(int8_t value);
    void tiny_print_newline(void);
}
