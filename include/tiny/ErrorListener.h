#pragma once

#include "antlr4-runtime.h"
#include "tiny/Diagnostics.h"

namespace tiny {

/// ANTLR error listener that routes parse/lex errors into Diagnostics
/// instead of writing them directly to stderr.
class TinyErrorListener : public antlr4::BaseErrorListener {
public:
    explicit TinyErrorListener(Diagnostics& diags) : diags_(diags) {}

    void syntaxError(antlr4::Recognizer* /*recognizer*/,
                     antlr4::Token* /*offendingSymbol*/,
                     size_t line,
                     size_t charPositionInLine,
                     const std::string& msg,
                     std::exception_ptr /*e*/) override {
        diags_.error({static_cast<int>(line),
                      static_cast<int>(charPositionInLine)}, msg);
    }

private:
    Diagnostics& diags_;
};

} // namespace tiny
