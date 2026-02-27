// test_lexer.cpp — Unit tests for TinyLexer (ANTLR4-generated)

#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "antlr4-runtime.h"
#include "TinyLexer.h"

// ── Helpers ───────────────────────────────────────────────────────────────────

// Owns token data so it remains valid after CommonTokenStream is destroyed.
struct TokenInfo {
    size_t      type;
    std::string text;
    size_t      line;
};

// Tokenize source; return all non-EOF tokens (whitespace/comments are skipped).
static std::vector<TokenInfo> tokenize(const std::string& src) {
    antlr4::ANTLRInputStream input(src);
    TinyLexer lexer(&input);
    lexer.removeErrorListeners();
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    std::vector<TokenInfo> result;
    for (auto* tok : tokens.getTokens()) {
        if (tok->getType() != antlr4::Token::EOF) {
            result.push_back({tok->getType(), tok->getText(), tok->getLine()});
        }
    }
    return result;
}

static std::vector<size_t> tokenTypes(const std::string& src) {
    std::vector<size_t> types;
    for (auto& tok : tokenize(src))
        types.push_back(tok.type);
    return types;
}

// ── Keywords ──────────────────────────────────────────────────────────────────

TEST(LexerTest, Keyword_let_var) {
    auto types = tokenTypes("let var");
    ASSERT_EQ(types.size(), 2u);
    EXPECT_EQ(types[0], (size_t)TinyLexer::LET);
    EXPECT_EQ(types[1], (size_t)TinyLexer::VAR);
}

TEST(LexerTest, Keyword_fn_return) {
    auto types = tokenTypes("fn return");
    ASSERT_EQ(types.size(), 2u);
    EXPECT_EQ(types[0], (size_t)TinyLexer::FN);
    EXPECT_EQ(types[1], (size_t)TinyLexer::RETURN);
}

TEST(LexerTest, Keyword_if_else_while) {
    auto types = tokenTypes("if else while");
    ASSERT_EQ(types.size(), 3u);
    EXPECT_EQ(types[0], (size_t)TinyLexer::IF);
    EXPECT_EQ(types[1], (size_t)TinyLexer::ELSE);
    EXPECT_EQ(types[2], (size_t)TinyLexer::WHILE);
}

TEST(LexerTest, Keyword_for_in) {
    auto types = tokenTypes("for in");
    ASSERT_EQ(types.size(), 2u);
    EXPECT_EQ(types[0], (size_t)TinyLexer::FOR);
    EXPECT_EQ(types[1], (size_t)TinyLexer::IN);
}

TEST(LexerTest, Keyword_print) {
    auto types = tokenTypes("print");
    ASSERT_EQ(types.size(), 1u);
    EXPECT_EQ(types[0], (size_t)TinyLexer::PRINT);
}

TEST(LexerTest, Keyword_true_false) {
    auto types = tokenTypes("true false");
    ASSERT_EQ(types.size(), 2u);
    EXPECT_EQ(types[0], (size_t)TinyLexer::TRUE);
    EXPECT_EQ(types[1], (size_t)TinyLexer::FALSE);
}

TEST(LexerTest, Keyword_import) {
    auto types = tokenTypes("import");
    ASSERT_EQ(types.size(), 1u);
    EXPECT_EQ(types[0], (size_t)TinyLexer::IMPORT);
}

TEST(LexerTest, Keyword_types) {
    auto types = tokenTypes("int float bool string");
    ASSERT_EQ(types.size(), 4u);
    EXPECT_EQ(types[0], (size_t)TinyLexer::INT_T);
    EXPECT_EQ(types[1], (size_t)TinyLexer::FLOAT_T);
    EXPECT_EQ(types[2], (size_t)TinyLexer::BOOL_T);
    EXPECT_EQ(types[3], (size_t)TinyLexer::STRING_T);
}

// Keywords must NOT be classified as identifiers
TEST(LexerTest, Keyword_NotClassifiedAsIdent) {
    auto types = tokenTypes("let");
    ASSERT_EQ(types.size(), 1u);
    EXPECT_NE(types[0], (size_t)TinyLexer::IDENT);
    EXPECT_EQ(types[0], (size_t)TinyLexer::LET);
}

// ── Integer Literals ──────────────────────────────────────────────────────────

TEST(LexerTest, IntLit_Zero) {
    auto toks = tokenize("0");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::INT_LIT);
    EXPECT_EQ(toks[0].text, "0");
}

TEST(LexerTest, IntLit_Positive) {
    auto toks = tokenize("42");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::INT_LIT);
    EXPECT_EQ(toks[0].text, "42");
}

TEST(LexerTest, IntLit_Large) {
    auto toks = tokenize("9999999");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::INT_LIT);
    EXPECT_EQ(toks[0].text, "9999999");
}

TEST(LexerTest, IntLit_Multiple) {
    auto toks = tokenize("1 2 3 4 5");
    ASSERT_EQ(toks.size(), 5u);
    for (auto& tok : toks)
        EXPECT_EQ(tok.type, (size_t)TinyLexer::INT_LIT);
}

// ── Float Literals ────────────────────────────────────────────────────────────

TEST(LexerTest, FloatLit_Basic) {
    auto toks = tokenize("3.14");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::FLOAT_LIT);
    EXPECT_EQ(toks[0].text, "3.14");
}

TEST(LexerTest, FloatLit_Zero) {
    auto toks = tokenize("0.0");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::FLOAT_LIT);
    EXPECT_EQ(toks[0].text, "0.0");
}

TEST(LexerTest, FloatLit_SmallFraction) {
    auto toks = tokenize("1.5");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::FLOAT_LIT);
}

// ── String Literals ───────────────────────────────────────────────────────────

TEST(LexerTest, StringLit_Empty) {
    auto toks = tokenize("\"\"");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::STRING_LIT);
    EXPECT_EQ(toks[0].text, "\"\"");
}

TEST(LexerTest, StringLit_Basic) {
    auto toks = tokenize("\"hello\"");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::STRING_LIT);
    EXPECT_EQ(toks[0].text, "\"hello\"");
}

TEST(LexerTest, StringLit_WithSpaces) {
    auto toks = tokenize("\"hello world\"");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::STRING_LIT);
    EXPECT_EQ(toks[0].text, "\"hello world\"");
}

TEST(LexerTest, StringLit_WithEscape) {
    auto toks = tokenize("\"say \\\"hi\\\"\"");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::STRING_LIT);
}

// ── Identifiers ───────────────────────────────────────────────────────────────

TEST(LexerTest, Ident_Simple) {
    auto toks = tokenize("myVar");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::IDENT);
    EXPECT_EQ(toks[0].text, "myVar");
}

TEST(LexerTest, Ident_LeadingUnderscore) {
    auto toks = tokenize("_hidden");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::IDENT);
    EXPECT_EQ(toks[0].text, "_hidden");
}

TEST(LexerTest, Ident_WithDigits) {
    auto toks = tokenize("var1");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::IDENT);
    EXPECT_EQ(toks[0].text, "var1");
}

TEST(LexerTest, Ident_AllUnderscores) {
    auto toks = tokenize("___");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::IDENT);
}

TEST(LexerTest, Ident_CamelCase) {
    auto toks = tokenize("myFunctionName");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::IDENT);
    EXPECT_EQ(toks[0].text, "myFunctionName");
}

// ── Whitespace & Comments Skipped ─────────────────────────────────────────────

TEST(LexerTest, Whitespace_Skipped) {
    auto toks = tokenize("  42  ");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].type, (size_t)TinyLexer::INT_LIT);
}

TEST(LexerTest, Tabs_And_Newlines_Skipped) {
    auto toks = tokenize("\t1\n2\r\n3");
    ASSERT_EQ(toks.size(), 3u);
}

TEST(LexerTest, LineComment_Skipped) {
    auto toks = tokenize("42 // this is a comment\n99");
    ASSERT_EQ(toks.size(), 2u);
    EXPECT_EQ(toks[0].text, "42");
    EXPECT_EQ(toks[1].text, "99");
}

TEST(LexerTest, BlockComment_Skipped) {
    auto toks = tokenize("42 /* block comment */ 99");
    ASSERT_EQ(toks.size(), 2u);
    EXPECT_EQ(toks[0].text, "42");
    EXPECT_EQ(toks[1].text, "99");
}

TEST(LexerTest, BlockComment_Multiline_Skipped) {
    auto toks = tokenize("1 /* multi\nline\ncomment */ 2");
    ASSERT_EQ(toks.size(), 2u);
    EXPECT_EQ(toks[0].text, "1");
    EXPECT_EQ(toks[1].text, "2");
}

TEST(LexerTest, EmptyInput_NoTokens) {
    EXPECT_TRUE(tokenize("").empty());
}

TEST(LexerTest, OnlyWhitespace_NoTokens) {
    EXPECT_TRUE(tokenize("   \n\t  ").empty());
}

TEST(LexerTest, OnlyLineComment_NoTokens) {
    EXPECT_TRUE(tokenize("// just a comment").empty());
}

TEST(LexerTest, OnlyBlockComment_NoTokens) {
    EXPECT_TRUE(tokenize("/* nothing here */").empty());
}

// ── Source Location ───────────────────────────────────────────────────────────

TEST(LexerTest, SourceLoc_Line1) {
    auto toks = tokenize("let");
    ASSERT_EQ(toks.size(), 1u);
    EXPECT_EQ(toks[0].line, (size_t)1);
}

TEST(LexerTest, SourceLoc_SecondLine) {
    auto toks = tokenize("let\nx");
    ASSERT_EQ(toks.size(), 2u);
    EXPECT_EQ(toks[0].line, (size_t)1);
    EXPECT_EQ(toks[1].line, (size_t)2);
}

// ── Mixed Sequences ───────────────────────────────────────────────────────────

TEST(LexerTest, VarDecl_Sequence) {
    // let x: int = 5;  →  LET IDENT ':' INT_T '=' INT_LIT ';'
    auto types = tokenTypes("let x: int = 5;");
    ASSERT_EQ(types.size(), 7u);
    EXPECT_EQ(types[0], (size_t)TinyLexer::LET);
    EXPECT_EQ(types[1], (size_t)TinyLexer::IDENT);
    EXPECT_EQ(types[3], (size_t)TinyLexer::INT_T);
    EXPECT_EQ(types[5], (size_t)TinyLexer::INT_LIT);
}

TEST(LexerTest, FunctionDecl_Sequence) {
    // fn add(a: int, b: int) -> int
    auto types = tokenTypes("fn add(a: int, b: int) -> int");
    EXPECT_EQ(types[0], (size_t)TinyLexer::FN);
    EXPECT_EQ(types[1], (size_t)TinyLexer::IDENT);  // 'add'
    EXPECT_EQ(types[3], (size_t)TinyLexer::IDENT);  // 'a'
    EXPECT_EQ(types[5], (size_t)TinyLexer::INT_T);  // 'int'
}
