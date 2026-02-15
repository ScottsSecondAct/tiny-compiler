#include "runtime.h"
#include <cstdio>

extern "C" {

void tiny_print_int(int64_t value) {
    printf("%lld", (long long)value);
}

void tiny_print_float(double value) {
    printf("%g", value);
}

void tiny_print_str(const char* value) {
    printf("%s", value);
}

void tiny_print_bool(int8_t value) {
    printf("%s", value ? "true" : "false");
}

void tiny_print_newline(void) {
    printf("\n");
}

}
