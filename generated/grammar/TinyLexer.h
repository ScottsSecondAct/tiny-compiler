
// Generated from grammar/Tiny.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  TinyLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    LET = 27, VAR = 28, FN = 29, RETURN = 30, IF = 31, ELSE = 32, WHILE = 33, 
    FOR = 34, IN = 35, PRINT = 36, TRUE = 37, FALSE = 38, INT_T = 39, FLOAT_T = 40, 
    BOOL_T = 41, STRING_T = 42, INT_LIT = 43, FLOAT_LIT = 44, STRING_LIT = 45, 
    IDENT = 46, WS = 47, LINE_COMMENT = 48, BLOCK_COMMENT = 49
  };

  explicit TinyLexer(antlr4::CharStream *input);

  ~TinyLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

