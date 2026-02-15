
// Generated from grammar/Tiny.g4 by ANTLR 4.13.1


#include "TinyVisitor.h"

#include "TinyParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct TinyParserStaticData final {
  TinyParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  TinyParserStaticData(const TinyParserStaticData&) = delete;
  TinyParserStaticData(TinyParserStaticData&&) = delete;
  TinyParserStaticData& operator=(const TinyParserStaticData&) = delete;
  TinyParserStaticData& operator=(TinyParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag tinyParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
TinyParserStaticData *tinyParserStaticData = nullptr;

void tinyParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (tinyParserStaticData != nullptr) {
    return;
  }
#else
  assert(tinyParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<TinyParserStaticData>(
    std::vector<std::string>{
      "program", "declaration", "functionDecl", "paramList", "param", "statement", 
      "varDecl", "assignment", "returnStmt", "printStmt", "ifStmt", "whileStmt", 
      "forStmt", "exprStmt", "block", "expression", "logicalOr", "logicalAnd", 
      "equality", "comparison", "addition", "multiplication", "unary", "postfix", 
      "primary", "arrayLiteral", "expressionList", "typeSpec"
    },
    std::vector<std::string>{
      "", "'('", "')'", "'->'", "','", "':'", "';'", "'='", "'['", "']'", 
      "'..'", "'{'", "'}'", "'||'", "'&&'", "'=='", "'!='", "'<'", "'<='", 
      "'>'", "'>='", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "'let'", 
      "'var'", "'fn'", "'return'", "'if'", "'else'", "'while'", "'for'", 
      "'in'", "'print'", "'true'", "'false'", "'int'", "'float'", "'bool'", 
      "'string'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "", "", "LET", "VAR", "FN", "RETURN", 
      "IF", "ELSE", "WHILE", "FOR", "IN", "PRINT", "TRUE", "FALSE", "INT_T", 
      "FLOAT_T", "BOOL_T", "STRING_T", "INT_LIT", "FLOAT_LIT", "STRING_LIT", 
      "IDENT", "WS", "LINE_COMMENT", "BLOCK_COMMENT"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,49,308,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,7,
  	21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,1,0,5,
  	0,58,8,0,10,0,12,0,61,9,0,1,0,1,0,1,1,1,1,3,1,67,8,1,1,2,1,2,1,2,1,2,
  	3,2,73,8,2,1,2,1,2,1,2,3,2,78,8,2,1,2,1,2,1,3,1,3,1,3,5,3,85,8,3,10,3,
  	12,3,88,9,3,1,4,1,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,
  	5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,1,5,3,5,113,8,5,1,6,1,6,1,6,1,6,3,6,119,
  	8,6,1,6,1,6,1,6,1,6,1,6,1,6,3,6,127,8,6,1,6,1,6,3,6,131,8,6,1,7,1,7,1,
  	7,1,7,1,7,1,7,1,7,1,7,1,7,1,7,3,7,143,8,7,1,8,1,8,3,8,147,8,8,1,9,1,9,
  	1,9,3,9,152,8,9,1,9,1,9,1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,3,10,
  	164,8,10,3,10,166,8,10,1,11,1,11,1,11,1,11,1,11,1,11,1,12,1,12,1,12,1,
  	12,1,12,1,12,1,12,1,12,1,12,1,12,1,13,1,13,1,14,1,14,5,14,188,8,14,10,
  	14,12,14,191,9,14,1,14,1,14,1,15,1,15,1,16,1,16,1,16,5,16,200,8,16,10,
  	16,12,16,203,9,16,1,17,1,17,1,17,5,17,208,8,17,10,17,12,17,211,9,17,1,
  	18,1,18,1,18,5,18,216,8,18,10,18,12,18,219,9,18,1,19,1,19,1,19,5,19,224,
  	8,19,10,19,12,19,227,9,19,1,20,1,20,1,20,5,20,232,8,20,10,20,12,20,235,
  	9,20,1,21,1,21,1,21,5,21,240,8,21,10,21,12,21,243,9,21,1,22,1,22,1,22,
  	3,22,248,8,22,1,23,1,23,1,23,3,23,253,8,23,1,23,1,23,1,23,1,23,1,23,5,
  	23,260,8,23,10,23,12,23,263,9,23,1,24,1,24,1,24,1,24,1,24,1,24,1,24,1,
  	24,1,24,1,24,1,24,3,24,276,8,24,1,25,1,25,3,25,280,8,25,1,25,1,25,1,26,
  	1,26,1,26,5,26,287,8,26,10,26,12,26,290,9,26,1,27,1,27,1,27,1,27,1,27,
  	3,27,297,8,27,1,27,1,27,1,27,1,27,5,27,303,8,27,10,27,12,27,306,9,27,
  	1,27,0,1,54,28,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,34,36,38,
  	40,42,44,46,48,50,52,54,0,5,1,0,15,16,1,0,17,20,1,0,21,22,1,0,23,25,2,
  	0,22,22,26,26,324,0,59,1,0,0,0,2,66,1,0,0,0,4,68,1,0,0,0,6,81,1,0,0,0,
  	8,89,1,0,0,0,10,112,1,0,0,0,12,130,1,0,0,0,14,142,1,0,0,0,16,144,1,0,
  	0,0,18,148,1,0,0,0,20,155,1,0,0,0,22,167,1,0,0,0,24,173,1,0,0,0,26,183,
  	1,0,0,0,28,185,1,0,0,0,30,194,1,0,0,0,32,196,1,0,0,0,34,204,1,0,0,0,36,
  	212,1,0,0,0,38,220,1,0,0,0,40,228,1,0,0,0,42,236,1,0,0,0,44,247,1,0,0,
  	0,46,249,1,0,0,0,48,275,1,0,0,0,50,277,1,0,0,0,52,283,1,0,0,0,54,296,
  	1,0,0,0,56,58,3,2,1,0,57,56,1,0,0,0,58,61,1,0,0,0,59,57,1,0,0,0,59,60,
  	1,0,0,0,60,62,1,0,0,0,61,59,1,0,0,0,62,63,5,0,0,1,63,1,1,0,0,0,64,67,
  	3,4,2,0,65,67,3,10,5,0,66,64,1,0,0,0,66,65,1,0,0,0,67,3,1,0,0,0,68,69,
  	5,29,0,0,69,70,5,46,0,0,70,72,5,1,0,0,71,73,3,6,3,0,72,71,1,0,0,0,72,
  	73,1,0,0,0,73,74,1,0,0,0,74,77,5,2,0,0,75,76,5,3,0,0,76,78,3,54,27,0,
  	77,75,1,0,0,0,77,78,1,0,0,0,78,79,1,0,0,0,79,80,3,28,14,0,80,5,1,0,0,
  	0,81,86,3,8,4,0,82,83,5,4,0,0,83,85,3,8,4,0,84,82,1,0,0,0,85,88,1,0,0,
  	0,86,84,1,0,0,0,86,87,1,0,0,0,87,7,1,0,0,0,88,86,1,0,0,0,89,90,5,46,0,
  	0,90,91,5,5,0,0,91,92,3,54,27,0,92,9,1,0,0,0,93,94,3,12,6,0,94,95,5,6,
  	0,0,95,113,1,0,0,0,96,97,3,14,7,0,97,98,5,6,0,0,98,113,1,0,0,0,99,100,
  	3,16,8,0,100,101,5,6,0,0,101,113,1,0,0,0,102,103,3,18,9,0,103,104,5,6,
  	0,0,104,113,1,0,0,0,105,113,3,20,10,0,106,113,3,22,11,0,107,113,3,24,
  	12,0,108,109,3,26,13,0,109,110,5,6,0,0,110,113,1,0,0,0,111,113,3,28,14,
  	0,112,93,1,0,0,0,112,96,1,0,0,0,112,99,1,0,0,0,112,102,1,0,0,0,112,105,
  	1,0,0,0,112,106,1,0,0,0,112,107,1,0,0,0,112,108,1,0,0,0,112,111,1,0,0,
  	0,113,11,1,0,0,0,114,115,5,27,0,0,115,118,5,46,0,0,116,117,5,5,0,0,117,
  	119,3,54,27,0,118,116,1,0,0,0,118,119,1,0,0,0,119,120,1,0,0,0,120,121,
  	5,7,0,0,121,131,3,30,15,0,122,123,5,28,0,0,123,126,5,46,0,0,124,125,5,
  	5,0,0,125,127,3,54,27,0,126,124,1,0,0,0,126,127,1,0,0,0,127,128,1,0,0,
  	0,128,129,5,7,0,0,129,131,3,30,15,0,130,114,1,0,0,0,130,122,1,0,0,0,131,
  	13,1,0,0,0,132,133,5,46,0,0,133,134,5,7,0,0,134,143,3,30,15,0,135,136,
  	5,46,0,0,136,137,5,8,0,0,137,138,3,30,15,0,138,139,5,9,0,0,139,140,5,
  	7,0,0,140,141,3,30,15,0,141,143,1,0,0,0,142,132,1,0,0,0,142,135,1,0,0,
  	0,143,15,1,0,0,0,144,146,5,30,0,0,145,147,3,30,15,0,146,145,1,0,0,0,146,
  	147,1,0,0,0,147,17,1,0,0,0,148,149,5,36,0,0,149,151,5,1,0,0,150,152,3,
  	52,26,0,151,150,1,0,0,0,151,152,1,0,0,0,152,153,1,0,0,0,153,154,5,2,0,
  	0,154,19,1,0,0,0,155,156,5,31,0,0,156,157,5,1,0,0,157,158,3,30,15,0,158,
  	159,5,2,0,0,159,165,3,28,14,0,160,163,5,32,0,0,161,164,3,20,10,0,162,
  	164,3,28,14,0,163,161,1,0,0,0,163,162,1,0,0,0,164,166,1,0,0,0,165,160,
  	1,0,0,0,165,166,1,0,0,0,166,21,1,0,0,0,167,168,5,33,0,0,168,169,5,1,0,
  	0,169,170,3,30,15,0,170,171,5,2,0,0,171,172,3,28,14,0,172,23,1,0,0,0,
  	173,174,5,34,0,0,174,175,5,1,0,0,175,176,5,46,0,0,176,177,5,35,0,0,177,
  	178,3,30,15,0,178,179,5,10,0,0,179,180,3,30,15,0,180,181,5,2,0,0,181,
  	182,3,28,14,0,182,25,1,0,0,0,183,184,3,30,15,0,184,27,1,0,0,0,185,189,
  	5,11,0,0,186,188,3,10,5,0,187,186,1,0,0,0,188,191,1,0,0,0,189,187,1,0,
  	0,0,189,190,1,0,0,0,190,192,1,0,0,0,191,189,1,0,0,0,192,193,5,12,0,0,
  	193,29,1,0,0,0,194,195,3,32,16,0,195,31,1,0,0,0,196,201,3,34,17,0,197,
  	198,5,13,0,0,198,200,3,34,17,0,199,197,1,0,0,0,200,203,1,0,0,0,201,199,
  	1,0,0,0,201,202,1,0,0,0,202,33,1,0,0,0,203,201,1,0,0,0,204,209,3,36,18,
  	0,205,206,5,14,0,0,206,208,3,36,18,0,207,205,1,0,0,0,208,211,1,0,0,0,
  	209,207,1,0,0,0,209,210,1,0,0,0,210,35,1,0,0,0,211,209,1,0,0,0,212,217,
  	3,38,19,0,213,214,7,0,0,0,214,216,3,38,19,0,215,213,1,0,0,0,216,219,1,
  	0,0,0,217,215,1,0,0,0,217,218,1,0,0,0,218,37,1,0,0,0,219,217,1,0,0,0,
  	220,225,3,40,20,0,221,222,7,1,0,0,222,224,3,40,20,0,223,221,1,0,0,0,224,
  	227,1,0,0,0,225,223,1,0,0,0,225,226,1,0,0,0,226,39,1,0,0,0,227,225,1,
  	0,0,0,228,233,3,42,21,0,229,230,7,2,0,0,230,232,3,42,21,0,231,229,1,0,
  	0,0,232,235,1,0,0,0,233,231,1,0,0,0,233,234,1,0,0,0,234,41,1,0,0,0,235,
  	233,1,0,0,0,236,241,3,44,22,0,237,238,7,3,0,0,238,240,3,44,22,0,239,237,
  	1,0,0,0,240,243,1,0,0,0,241,239,1,0,0,0,241,242,1,0,0,0,242,43,1,0,0,
  	0,243,241,1,0,0,0,244,245,7,4,0,0,245,248,3,44,22,0,246,248,3,46,23,0,
  	247,244,1,0,0,0,247,246,1,0,0,0,248,45,1,0,0,0,249,261,3,48,24,0,250,
  	252,5,1,0,0,251,253,3,52,26,0,252,251,1,0,0,0,252,253,1,0,0,0,253,254,
  	1,0,0,0,254,260,5,2,0,0,255,256,5,8,0,0,256,257,3,30,15,0,257,258,5,9,
  	0,0,258,260,1,0,0,0,259,250,1,0,0,0,259,255,1,0,0,0,260,263,1,0,0,0,261,
  	259,1,0,0,0,261,262,1,0,0,0,262,47,1,0,0,0,263,261,1,0,0,0,264,276,5,
  	43,0,0,265,276,5,44,0,0,266,276,5,45,0,0,267,276,5,37,0,0,268,276,5,38,
  	0,0,269,276,5,46,0,0,270,276,3,50,25,0,271,272,5,1,0,0,272,273,3,30,15,
  	0,273,274,5,2,0,0,274,276,1,0,0,0,275,264,1,0,0,0,275,265,1,0,0,0,275,
  	266,1,0,0,0,275,267,1,0,0,0,275,268,1,0,0,0,275,269,1,0,0,0,275,270,1,
  	0,0,0,275,271,1,0,0,0,276,49,1,0,0,0,277,279,5,8,0,0,278,280,3,52,26,
  	0,279,278,1,0,0,0,279,280,1,0,0,0,280,281,1,0,0,0,281,282,5,9,0,0,282,
  	51,1,0,0,0,283,288,3,30,15,0,284,285,5,4,0,0,285,287,3,30,15,0,286,284,
  	1,0,0,0,287,290,1,0,0,0,288,286,1,0,0,0,288,289,1,0,0,0,289,53,1,0,0,
  	0,290,288,1,0,0,0,291,292,6,27,-1,0,292,297,5,39,0,0,293,297,5,40,0,0,
  	294,297,5,41,0,0,295,297,5,42,0,0,296,291,1,0,0,0,296,293,1,0,0,0,296,
  	294,1,0,0,0,296,295,1,0,0,0,297,304,1,0,0,0,298,299,10,1,0,0,299,300,
  	5,8,0,0,300,301,5,43,0,0,301,303,5,9,0,0,302,298,1,0,0,0,303,306,1,0,
  	0,0,304,302,1,0,0,0,304,305,1,0,0,0,305,55,1,0,0,0,306,304,1,0,0,0,30,
  	59,66,72,77,86,112,118,126,130,142,146,151,163,165,189,201,209,217,225,
  	233,241,247,252,259,261,275,279,288,296,304
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  tinyParserStaticData = staticData.release();
}

}

TinyParser::TinyParser(TokenStream *input) : TinyParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

TinyParser::TinyParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  TinyParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *tinyParserStaticData->atn, tinyParserStaticData->decisionToDFA, tinyParserStaticData->sharedContextCache, options);
}

TinyParser::~TinyParser() {
  delete _interpreter;
}

const atn::ATN& TinyParser::getATN() const {
  return *tinyParserStaticData->atn;
}

std::string TinyParser::getGrammarFileName() const {
  return "Tiny.g4";
}

const std::vector<std::string>& TinyParser::getRuleNames() const {
  return tinyParserStaticData->ruleNames;
}

const dfa::Vocabulary& TinyParser::getVocabulary() const {
  return tinyParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView TinyParser::getSerializedATN() const {
  return tinyParserStaticData->serializedATN;
}


//----------------- ProgramContext ------------------------------------------------------------------

TinyParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::ProgramContext::EOF() {
  return getToken(TinyParser::EOF, 0);
}

std::vector<TinyParser::DeclarationContext *> TinyParser::ProgramContext::declaration() {
  return getRuleContexts<TinyParser::DeclarationContext>();
}

TinyParser::DeclarationContext* TinyParser::ProgramContext::declaration(size_t i) {
  return getRuleContext<TinyParser::DeclarationContext>(i);
}


size_t TinyParser::ProgramContext::getRuleIndex() const {
  return TinyParser::RuleProgram;
}


std::any TinyParser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ProgramContext* TinyParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, TinyParser::RuleProgram);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(59);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 132452433529090) != 0)) {
      setState(56);
      declaration();
      setState(61);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(62);
    match(TinyParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

TinyParser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TinyParser::FunctionDeclContext* TinyParser::DeclarationContext::functionDecl() {
  return getRuleContext<TinyParser::FunctionDeclContext>(0);
}

TinyParser::StatementContext* TinyParser::DeclarationContext::statement() {
  return getRuleContext<TinyParser::StatementContext>(0);
}


size_t TinyParser::DeclarationContext::getRuleIndex() const {
  return TinyParser::RuleDeclaration;
}


std::any TinyParser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::DeclarationContext* TinyParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 2, TinyParser::RuleDeclaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(66);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TinyParser::FN: {
        enterOuterAlt(_localctx, 1);
        setState(64);
        functionDecl();
        break;
      }

      case TinyParser::T__0:
      case TinyParser::T__7:
      case TinyParser::T__10:
      case TinyParser::T__21:
      case TinyParser::T__25:
      case TinyParser::LET:
      case TinyParser::VAR:
      case TinyParser::RETURN:
      case TinyParser::IF:
      case TinyParser::WHILE:
      case TinyParser::FOR:
      case TinyParser::PRINT:
      case TinyParser::TRUE:
      case TinyParser::FALSE:
      case TinyParser::INT_LIT:
      case TinyParser::FLOAT_LIT:
      case TinyParser::STRING_LIT:
      case TinyParser::IDENT: {
        enterOuterAlt(_localctx, 2);
        setState(65);
        statement();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionDeclContext ------------------------------------------------------------------

TinyParser::FunctionDeclContext::FunctionDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::FunctionDeclContext::FN() {
  return getToken(TinyParser::FN, 0);
}

tree::TerminalNode* TinyParser::FunctionDeclContext::IDENT() {
  return getToken(TinyParser::IDENT, 0);
}

TinyParser::BlockContext* TinyParser::FunctionDeclContext::block() {
  return getRuleContext<TinyParser::BlockContext>(0);
}

TinyParser::ParamListContext* TinyParser::FunctionDeclContext::paramList() {
  return getRuleContext<TinyParser::ParamListContext>(0);
}

TinyParser::TypeSpecContext* TinyParser::FunctionDeclContext::typeSpec() {
  return getRuleContext<TinyParser::TypeSpecContext>(0);
}


size_t TinyParser::FunctionDeclContext::getRuleIndex() const {
  return TinyParser::RuleFunctionDecl;
}


std::any TinyParser::FunctionDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitFunctionDecl(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::FunctionDeclContext* TinyParser::functionDecl() {
  FunctionDeclContext *_localctx = _tracker.createInstance<FunctionDeclContext>(_ctx, getState());
  enterRule(_localctx, 4, TinyParser::RuleFunctionDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(68);
    match(TinyParser::FN);
    setState(69);
    match(TinyParser::IDENT);
    setState(70);
    match(TinyParser::T__0);
    setState(72);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TinyParser::IDENT) {
      setState(71);
      paramList();
    }
    setState(74);
    match(TinyParser::T__1);
    setState(77);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TinyParser::T__2) {
      setState(75);
      match(TinyParser::T__2);
      setState(76);
      typeSpec(0);
    }
    setState(79);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamListContext ------------------------------------------------------------------

TinyParser::ParamListContext::ParamListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TinyParser::ParamContext *> TinyParser::ParamListContext::param() {
  return getRuleContexts<TinyParser::ParamContext>();
}

TinyParser::ParamContext* TinyParser::ParamListContext::param(size_t i) {
  return getRuleContext<TinyParser::ParamContext>(i);
}


size_t TinyParser::ParamListContext::getRuleIndex() const {
  return TinyParser::RuleParamList;
}


std::any TinyParser::ParamListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitParamList(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ParamListContext* TinyParser::paramList() {
  ParamListContext *_localctx = _tracker.createInstance<ParamListContext>(_ctx, getState());
  enterRule(_localctx, 6, TinyParser::RuleParamList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(81);
    param();
    setState(86);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TinyParser::T__3) {
      setState(82);
      match(TinyParser::T__3);
      setState(83);
      param();
      setState(88);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamContext ------------------------------------------------------------------

TinyParser::ParamContext::ParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::ParamContext::IDENT() {
  return getToken(TinyParser::IDENT, 0);
}

TinyParser::TypeSpecContext* TinyParser::ParamContext::typeSpec() {
  return getRuleContext<TinyParser::TypeSpecContext>(0);
}


size_t TinyParser::ParamContext::getRuleIndex() const {
  return TinyParser::RuleParam;
}


std::any TinyParser::ParamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitParam(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ParamContext* TinyParser::param() {
  ParamContext *_localctx = _tracker.createInstance<ParamContext>(_ctx, getState());
  enterRule(_localctx, 8, TinyParser::RuleParam);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(89);
    match(TinyParser::IDENT);
    setState(90);
    match(TinyParser::T__4);
    setState(91);
    typeSpec(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

TinyParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TinyParser::VarDeclContext* TinyParser::StatementContext::varDecl() {
  return getRuleContext<TinyParser::VarDeclContext>(0);
}

TinyParser::AssignmentContext* TinyParser::StatementContext::assignment() {
  return getRuleContext<TinyParser::AssignmentContext>(0);
}

TinyParser::ReturnStmtContext* TinyParser::StatementContext::returnStmt() {
  return getRuleContext<TinyParser::ReturnStmtContext>(0);
}

TinyParser::PrintStmtContext* TinyParser::StatementContext::printStmt() {
  return getRuleContext<TinyParser::PrintStmtContext>(0);
}

TinyParser::IfStmtContext* TinyParser::StatementContext::ifStmt() {
  return getRuleContext<TinyParser::IfStmtContext>(0);
}

TinyParser::WhileStmtContext* TinyParser::StatementContext::whileStmt() {
  return getRuleContext<TinyParser::WhileStmtContext>(0);
}

TinyParser::ForStmtContext* TinyParser::StatementContext::forStmt() {
  return getRuleContext<TinyParser::ForStmtContext>(0);
}

TinyParser::ExprStmtContext* TinyParser::StatementContext::exprStmt() {
  return getRuleContext<TinyParser::ExprStmtContext>(0);
}

TinyParser::BlockContext* TinyParser::StatementContext::block() {
  return getRuleContext<TinyParser::BlockContext>(0);
}


size_t TinyParser::StatementContext::getRuleIndex() const {
  return TinyParser::RuleStatement;
}


std::any TinyParser::StatementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitStatement(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::StatementContext* TinyParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 10, TinyParser::RuleStatement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(112);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(93);
      varDecl();
      setState(94);
      match(TinyParser::T__5);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(96);
      assignment();
      setState(97);
      match(TinyParser::T__5);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(99);
      returnStmt();
      setState(100);
      match(TinyParser::T__5);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(102);
      printStmt();
      setState(103);
      match(TinyParser::T__5);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(105);
      ifStmt();
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(106);
      whileStmt();
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(107);
      forStmt();
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(108);
      exprStmt();
      setState(109);
      match(TinyParser::T__5);
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(111);
      block();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VarDeclContext ------------------------------------------------------------------

TinyParser::VarDeclContext::VarDeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::VarDeclContext::LET() {
  return getToken(TinyParser::LET, 0);
}

tree::TerminalNode* TinyParser::VarDeclContext::IDENT() {
  return getToken(TinyParser::IDENT, 0);
}

TinyParser::ExpressionContext* TinyParser::VarDeclContext::expression() {
  return getRuleContext<TinyParser::ExpressionContext>(0);
}

TinyParser::TypeSpecContext* TinyParser::VarDeclContext::typeSpec() {
  return getRuleContext<TinyParser::TypeSpecContext>(0);
}

tree::TerminalNode* TinyParser::VarDeclContext::VAR() {
  return getToken(TinyParser::VAR, 0);
}


size_t TinyParser::VarDeclContext::getRuleIndex() const {
  return TinyParser::RuleVarDecl;
}


std::any TinyParser::VarDeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitVarDecl(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::VarDeclContext* TinyParser::varDecl() {
  VarDeclContext *_localctx = _tracker.createInstance<VarDeclContext>(_ctx, getState());
  enterRule(_localctx, 12, TinyParser::RuleVarDecl);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(130);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TinyParser::LET: {
        enterOuterAlt(_localctx, 1);
        setState(114);
        match(TinyParser::LET);
        setState(115);
        match(TinyParser::IDENT);
        setState(118);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == TinyParser::T__4) {
          setState(116);
          match(TinyParser::T__4);
          setState(117);
          typeSpec(0);
        }
        setState(120);
        match(TinyParser::T__6);
        setState(121);
        expression();
        break;
      }

      case TinyParser::VAR: {
        enterOuterAlt(_localctx, 2);
        setState(122);
        match(TinyParser::VAR);
        setState(123);
        match(TinyParser::IDENT);
        setState(126);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == TinyParser::T__4) {
          setState(124);
          match(TinyParser::T__4);
          setState(125);
          typeSpec(0);
        }
        setState(128);
        match(TinyParser::T__6);
        setState(129);
        expression();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentContext ------------------------------------------------------------------

TinyParser::AssignmentContext::AssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::AssignmentContext::IDENT() {
  return getToken(TinyParser::IDENT, 0);
}

std::vector<TinyParser::ExpressionContext *> TinyParser::AssignmentContext::expression() {
  return getRuleContexts<TinyParser::ExpressionContext>();
}

TinyParser::ExpressionContext* TinyParser::AssignmentContext::expression(size_t i) {
  return getRuleContext<TinyParser::ExpressionContext>(i);
}


size_t TinyParser::AssignmentContext::getRuleIndex() const {
  return TinyParser::RuleAssignment;
}


std::any TinyParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::AssignmentContext* TinyParser::assignment() {
  AssignmentContext *_localctx = _tracker.createInstance<AssignmentContext>(_ctx, getState());
  enterRule(_localctx, 14, TinyParser::RuleAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(142);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(132);
      match(TinyParser::IDENT);
      setState(133);
      match(TinyParser::T__6);
      setState(134);
      expression();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(135);
      match(TinyParser::IDENT);
      setState(136);
      match(TinyParser::T__7);
      setState(137);
      expression();
      setState(138);
      match(TinyParser::T__8);
      setState(139);
      match(TinyParser::T__6);
      setState(140);
      expression();
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ReturnStmtContext ------------------------------------------------------------------

TinyParser::ReturnStmtContext::ReturnStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::ReturnStmtContext::RETURN() {
  return getToken(TinyParser::RETURN, 0);
}

TinyParser::ExpressionContext* TinyParser::ReturnStmtContext::expression() {
  return getRuleContext<TinyParser::ExpressionContext>(0);
}


size_t TinyParser::ReturnStmtContext::getRuleIndex() const {
  return TinyParser::RuleReturnStmt;
}


std::any TinyParser::ReturnStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitReturnStmt(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ReturnStmtContext* TinyParser::returnStmt() {
  ReturnStmtContext *_localctx = _tracker.createInstance<ReturnStmtContext>(_ctx, getState());
  enterRule(_localctx, 16, TinyParser::RuleReturnStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(144);
    match(TinyParser::RETURN);
    setState(146);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 132353783496962) != 0)) {
      setState(145);
      expression();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrintStmtContext ------------------------------------------------------------------

TinyParser::PrintStmtContext::PrintStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::PrintStmtContext::PRINT() {
  return getToken(TinyParser::PRINT, 0);
}

TinyParser::ExpressionListContext* TinyParser::PrintStmtContext::expressionList() {
  return getRuleContext<TinyParser::ExpressionListContext>(0);
}


size_t TinyParser::PrintStmtContext::getRuleIndex() const {
  return TinyParser::RulePrintStmt;
}


std::any TinyParser::PrintStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitPrintStmt(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::PrintStmtContext* TinyParser::printStmt() {
  PrintStmtContext *_localctx = _tracker.createInstance<PrintStmtContext>(_ctx, getState());
  enterRule(_localctx, 18, TinyParser::RulePrintStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(148);
    match(TinyParser::PRINT);
    setState(149);
    match(TinyParser::T__0);
    setState(151);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 132353783496962) != 0)) {
      setState(150);
      expressionList();
    }
    setState(153);
    match(TinyParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IfStmtContext ------------------------------------------------------------------

TinyParser::IfStmtContext::IfStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::IfStmtContext::IF() {
  return getToken(TinyParser::IF, 0);
}

TinyParser::ExpressionContext* TinyParser::IfStmtContext::expression() {
  return getRuleContext<TinyParser::ExpressionContext>(0);
}

std::vector<TinyParser::BlockContext *> TinyParser::IfStmtContext::block() {
  return getRuleContexts<TinyParser::BlockContext>();
}

TinyParser::BlockContext* TinyParser::IfStmtContext::block(size_t i) {
  return getRuleContext<TinyParser::BlockContext>(i);
}

tree::TerminalNode* TinyParser::IfStmtContext::ELSE() {
  return getToken(TinyParser::ELSE, 0);
}

TinyParser::IfStmtContext* TinyParser::IfStmtContext::ifStmt() {
  return getRuleContext<TinyParser::IfStmtContext>(0);
}


size_t TinyParser::IfStmtContext::getRuleIndex() const {
  return TinyParser::RuleIfStmt;
}


std::any TinyParser::IfStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitIfStmt(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::IfStmtContext* TinyParser::ifStmt() {
  IfStmtContext *_localctx = _tracker.createInstance<IfStmtContext>(_ctx, getState());
  enterRule(_localctx, 20, TinyParser::RuleIfStmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(155);
    match(TinyParser::IF);
    setState(156);
    match(TinyParser::T__0);
    setState(157);
    expression();
    setState(158);
    match(TinyParser::T__1);
    setState(159);
    block();
    setState(165);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == TinyParser::ELSE) {
      setState(160);
      match(TinyParser::ELSE);
      setState(163);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case TinyParser::IF: {
          setState(161);
          ifStmt();
          break;
        }

        case TinyParser::T__10: {
          setState(162);
          block();
          break;
        }

      default:
        throw NoViableAltException(this);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WhileStmtContext ------------------------------------------------------------------

TinyParser::WhileStmtContext::WhileStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::WhileStmtContext::WHILE() {
  return getToken(TinyParser::WHILE, 0);
}

TinyParser::ExpressionContext* TinyParser::WhileStmtContext::expression() {
  return getRuleContext<TinyParser::ExpressionContext>(0);
}

TinyParser::BlockContext* TinyParser::WhileStmtContext::block() {
  return getRuleContext<TinyParser::BlockContext>(0);
}


size_t TinyParser::WhileStmtContext::getRuleIndex() const {
  return TinyParser::RuleWhileStmt;
}


std::any TinyParser::WhileStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitWhileStmt(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::WhileStmtContext* TinyParser::whileStmt() {
  WhileStmtContext *_localctx = _tracker.createInstance<WhileStmtContext>(_ctx, getState());
  enterRule(_localctx, 22, TinyParser::RuleWhileStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(167);
    match(TinyParser::WHILE);
    setState(168);
    match(TinyParser::T__0);
    setState(169);
    expression();
    setState(170);
    match(TinyParser::T__1);
    setState(171);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ForStmtContext ------------------------------------------------------------------

TinyParser::ForStmtContext::ForStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::ForStmtContext::FOR() {
  return getToken(TinyParser::FOR, 0);
}

tree::TerminalNode* TinyParser::ForStmtContext::IDENT() {
  return getToken(TinyParser::IDENT, 0);
}

tree::TerminalNode* TinyParser::ForStmtContext::IN() {
  return getToken(TinyParser::IN, 0);
}

std::vector<TinyParser::ExpressionContext *> TinyParser::ForStmtContext::expression() {
  return getRuleContexts<TinyParser::ExpressionContext>();
}

TinyParser::ExpressionContext* TinyParser::ForStmtContext::expression(size_t i) {
  return getRuleContext<TinyParser::ExpressionContext>(i);
}

TinyParser::BlockContext* TinyParser::ForStmtContext::block() {
  return getRuleContext<TinyParser::BlockContext>(0);
}


size_t TinyParser::ForStmtContext::getRuleIndex() const {
  return TinyParser::RuleForStmt;
}


std::any TinyParser::ForStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitForStmt(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ForStmtContext* TinyParser::forStmt() {
  ForStmtContext *_localctx = _tracker.createInstance<ForStmtContext>(_ctx, getState());
  enterRule(_localctx, 24, TinyParser::RuleForStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(173);
    match(TinyParser::FOR);
    setState(174);
    match(TinyParser::T__0);
    setState(175);
    match(TinyParser::IDENT);
    setState(176);
    match(TinyParser::IN);
    setState(177);
    expression();
    setState(178);
    match(TinyParser::T__9);
    setState(179);
    expression();
    setState(180);
    match(TinyParser::T__1);
    setState(181);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprStmtContext ------------------------------------------------------------------

TinyParser::ExprStmtContext::ExprStmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TinyParser::ExpressionContext* TinyParser::ExprStmtContext::expression() {
  return getRuleContext<TinyParser::ExpressionContext>(0);
}


size_t TinyParser::ExprStmtContext::getRuleIndex() const {
  return TinyParser::RuleExprStmt;
}


std::any TinyParser::ExprStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitExprStmt(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ExprStmtContext* TinyParser::exprStmt() {
  ExprStmtContext *_localctx = _tracker.createInstance<ExprStmtContext>(_ctx, getState());
  enterRule(_localctx, 26, TinyParser::RuleExprStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(183);
    expression();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

TinyParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TinyParser::StatementContext *> TinyParser::BlockContext::statement() {
  return getRuleContexts<TinyParser::StatementContext>();
}

TinyParser::StatementContext* TinyParser::BlockContext::statement(size_t i) {
  return getRuleContext<TinyParser::StatementContext>(i);
}


size_t TinyParser::BlockContext::getRuleIndex() const {
  return TinyParser::RuleBlock;
}


std::any TinyParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::BlockContext* TinyParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 28, TinyParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(185);
    match(TinyParser::T__10);
    setState(189);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 132451896658178) != 0)) {
      setState(186);
      statement();
      setState(191);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(192);
    match(TinyParser::T__11);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

TinyParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TinyParser::LogicalOrContext* TinyParser::ExpressionContext::logicalOr() {
  return getRuleContext<TinyParser::LogicalOrContext>(0);
}


size_t TinyParser::ExpressionContext::getRuleIndex() const {
  return TinyParser::RuleExpression;
}


std::any TinyParser::ExpressionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitExpression(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ExpressionContext* TinyParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 30, TinyParser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(194);
    logicalOr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalOrContext ------------------------------------------------------------------

TinyParser::LogicalOrContext::LogicalOrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TinyParser::LogicalAndContext *> TinyParser::LogicalOrContext::logicalAnd() {
  return getRuleContexts<TinyParser::LogicalAndContext>();
}

TinyParser::LogicalAndContext* TinyParser::LogicalOrContext::logicalAnd(size_t i) {
  return getRuleContext<TinyParser::LogicalAndContext>(i);
}


size_t TinyParser::LogicalOrContext::getRuleIndex() const {
  return TinyParser::RuleLogicalOr;
}


std::any TinyParser::LogicalOrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitLogicalOr(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::LogicalOrContext* TinyParser::logicalOr() {
  LogicalOrContext *_localctx = _tracker.createInstance<LogicalOrContext>(_ctx, getState());
  enterRule(_localctx, 32, TinyParser::RuleLogicalOr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(196);
    logicalAnd();
    setState(201);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TinyParser::T__12) {
      setState(197);
      match(TinyParser::T__12);
      setState(198);
      logicalAnd();
      setState(203);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LogicalAndContext ------------------------------------------------------------------

TinyParser::LogicalAndContext::LogicalAndContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TinyParser::EqualityContext *> TinyParser::LogicalAndContext::equality() {
  return getRuleContexts<TinyParser::EqualityContext>();
}

TinyParser::EqualityContext* TinyParser::LogicalAndContext::equality(size_t i) {
  return getRuleContext<TinyParser::EqualityContext>(i);
}


size_t TinyParser::LogicalAndContext::getRuleIndex() const {
  return TinyParser::RuleLogicalAnd;
}


std::any TinyParser::LogicalAndContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitLogicalAnd(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::LogicalAndContext* TinyParser::logicalAnd() {
  LogicalAndContext *_localctx = _tracker.createInstance<LogicalAndContext>(_ctx, getState());
  enterRule(_localctx, 34, TinyParser::RuleLogicalAnd);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(204);
    equality();
    setState(209);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TinyParser::T__13) {
      setState(205);
      match(TinyParser::T__13);
      setState(206);
      equality();
      setState(211);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EqualityContext ------------------------------------------------------------------

TinyParser::EqualityContext::EqualityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TinyParser::ComparisonContext *> TinyParser::EqualityContext::comparison() {
  return getRuleContexts<TinyParser::ComparisonContext>();
}

TinyParser::ComparisonContext* TinyParser::EqualityContext::comparison(size_t i) {
  return getRuleContext<TinyParser::ComparisonContext>(i);
}


size_t TinyParser::EqualityContext::getRuleIndex() const {
  return TinyParser::RuleEquality;
}


std::any TinyParser::EqualityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitEquality(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::EqualityContext* TinyParser::equality() {
  EqualityContext *_localctx = _tracker.createInstance<EqualityContext>(_ctx, getState());
  enterRule(_localctx, 36, TinyParser::RuleEquality);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(212);
    comparison();
    setState(217);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TinyParser::T__14

    || _la == TinyParser::T__15) {
      setState(213);
      _la = _input->LA(1);
      if (!(_la == TinyParser::T__14

      || _la == TinyParser::T__15)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(214);
      comparison();
      setState(219);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComparisonContext ------------------------------------------------------------------

TinyParser::ComparisonContext::ComparisonContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TinyParser::AdditionContext *> TinyParser::ComparisonContext::addition() {
  return getRuleContexts<TinyParser::AdditionContext>();
}

TinyParser::AdditionContext* TinyParser::ComparisonContext::addition(size_t i) {
  return getRuleContext<TinyParser::AdditionContext>(i);
}


size_t TinyParser::ComparisonContext::getRuleIndex() const {
  return TinyParser::RuleComparison;
}


std::any TinyParser::ComparisonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitComparison(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ComparisonContext* TinyParser::comparison() {
  ComparisonContext *_localctx = _tracker.createInstance<ComparisonContext>(_ctx, getState());
  enterRule(_localctx, 38, TinyParser::RuleComparison);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(220);
    addition();
    setState(225);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 1966080) != 0)) {
      setState(221);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 1966080) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(222);
      addition();
      setState(227);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AdditionContext ------------------------------------------------------------------

TinyParser::AdditionContext::AdditionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TinyParser::MultiplicationContext *> TinyParser::AdditionContext::multiplication() {
  return getRuleContexts<TinyParser::MultiplicationContext>();
}

TinyParser::MultiplicationContext* TinyParser::AdditionContext::multiplication(size_t i) {
  return getRuleContext<TinyParser::MultiplicationContext>(i);
}


size_t TinyParser::AdditionContext::getRuleIndex() const {
  return TinyParser::RuleAddition;
}


std::any TinyParser::AdditionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitAddition(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::AdditionContext* TinyParser::addition() {
  AdditionContext *_localctx = _tracker.createInstance<AdditionContext>(_ctx, getState());
  enterRule(_localctx, 40, TinyParser::RuleAddition);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(228);
    multiplication();
    setState(233);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TinyParser::T__20

    || _la == TinyParser::T__21) {
      setState(229);
      _la = _input->LA(1);
      if (!(_la == TinyParser::T__20

      || _la == TinyParser::T__21)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(230);
      multiplication();
      setState(235);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MultiplicationContext ------------------------------------------------------------------

TinyParser::MultiplicationContext::MultiplicationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TinyParser::UnaryContext *> TinyParser::MultiplicationContext::unary() {
  return getRuleContexts<TinyParser::UnaryContext>();
}

TinyParser::UnaryContext* TinyParser::MultiplicationContext::unary(size_t i) {
  return getRuleContext<TinyParser::UnaryContext>(i);
}


size_t TinyParser::MultiplicationContext::getRuleIndex() const {
  return TinyParser::RuleMultiplication;
}


std::any TinyParser::MultiplicationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitMultiplication(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::MultiplicationContext* TinyParser::multiplication() {
  MultiplicationContext *_localctx = _tracker.createInstance<MultiplicationContext>(_ctx, getState());
  enterRule(_localctx, 42, TinyParser::RuleMultiplication);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(236);
    unary();
    setState(241);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 58720256) != 0)) {
      setState(237);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & 58720256) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(238);
      unary();
      setState(243);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryContext ------------------------------------------------------------------

TinyParser::UnaryContext::UnaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TinyParser::UnaryContext* TinyParser::UnaryContext::unary() {
  return getRuleContext<TinyParser::UnaryContext>(0);
}

TinyParser::PostfixContext* TinyParser::UnaryContext::postfix() {
  return getRuleContext<TinyParser::PostfixContext>(0);
}


size_t TinyParser::UnaryContext::getRuleIndex() const {
  return TinyParser::RuleUnary;
}


std::any TinyParser::UnaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitUnary(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::UnaryContext* TinyParser::unary() {
  UnaryContext *_localctx = _tracker.createInstance<UnaryContext>(_ctx, getState());
  enterRule(_localctx, 44, TinyParser::RuleUnary);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(247);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TinyParser::T__21:
      case TinyParser::T__25: {
        enterOuterAlt(_localctx, 1);
        setState(244);
        _la = _input->LA(1);
        if (!(_la == TinyParser::T__21

        || _la == TinyParser::T__25)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        setState(245);
        unary();
        break;
      }

      case TinyParser::T__0:
      case TinyParser::T__7:
      case TinyParser::TRUE:
      case TinyParser::FALSE:
      case TinyParser::INT_LIT:
      case TinyParser::FLOAT_LIT:
      case TinyParser::STRING_LIT:
      case TinyParser::IDENT: {
        enterOuterAlt(_localctx, 2);
        setState(246);
        postfix();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PostfixContext ------------------------------------------------------------------

TinyParser::PostfixContext::PostfixContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TinyParser::PrimaryContext* TinyParser::PostfixContext::primary() {
  return getRuleContext<TinyParser::PrimaryContext>(0);
}

std::vector<TinyParser::ExpressionContext *> TinyParser::PostfixContext::expression() {
  return getRuleContexts<TinyParser::ExpressionContext>();
}

TinyParser::ExpressionContext* TinyParser::PostfixContext::expression(size_t i) {
  return getRuleContext<TinyParser::ExpressionContext>(i);
}

std::vector<TinyParser::ExpressionListContext *> TinyParser::PostfixContext::expressionList() {
  return getRuleContexts<TinyParser::ExpressionListContext>();
}

TinyParser::ExpressionListContext* TinyParser::PostfixContext::expressionList(size_t i) {
  return getRuleContext<TinyParser::ExpressionListContext>(i);
}


size_t TinyParser::PostfixContext::getRuleIndex() const {
  return TinyParser::RulePostfix;
}


std::any TinyParser::PostfixContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitPostfix(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::PostfixContext* TinyParser::postfix() {
  PostfixContext *_localctx = _tracker.createInstance<PostfixContext>(_ctx, getState());
  enterRule(_localctx, 46, TinyParser::RulePostfix);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(249);
    primary();
    setState(261);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TinyParser::T__0

    || _la == TinyParser::T__7) {
      setState(259);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case TinyParser::T__0: {
          setState(250);
          match(TinyParser::T__0);
          setState(252);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & 132353783496962) != 0)) {
            setState(251);
            expressionList();
          }
          setState(254);
          match(TinyParser::T__1);
          break;
        }

        case TinyParser::T__7: {
          setState(255);
          match(TinyParser::T__7);
          setState(256);
          expression();
          setState(257);
          match(TinyParser::T__8);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(263);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PrimaryContext ------------------------------------------------------------------

TinyParser::PrimaryContext::PrimaryContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::PrimaryContext::INT_LIT() {
  return getToken(TinyParser::INT_LIT, 0);
}

tree::TerminalNode* TinyParser::PrimaryContext::FLOAT_LIT() {
  return getToken(TinyParser::FLOAT_LIT, 0);
}

tree::TerminalNode* TinyParser::PrimaryContext::STRING_LIT() {
  return getToken(TinyParser::STRING_LIT, 0);
}

tree::TerminalNode* TinyParser::PrimaryContext::TRUE() {
  return getToken(TinyParser::TRUE, 0);
}

tree::TerminalNode* TinyParser::PrimaryContext::FALSE() {
  return getToken(TinyParser::FALSE, 0);
}

tree::TerminalNode* TinyParser::PrimaryContext::IDENT() {
  return getToken(TinyParser::IDENT, 0);
}

TinyParser::ArrayLiteralContext* TinyParser::PrimaryContext::arrayLiteral() {
  return getRuleContext<TinyParser::ArrayLiteralContext>(0);
}

TinyParser::ExpressionContext* TinyParser::PrimaryContext::expression() {
  return getRuleContext<TinyParser::ExpressionContext>(0);
}


size_t TinyParser::PrimaryContext::getRuleIndex() const {
  return TinyParser::RulePrimary;
}


std::any TinyParser::PrimaryContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitPrimary(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::PrimaryContext* TinyParser::primary() {
  PrimaryContext *_localctx = _tracker.createInstance<PrimaryContext>(_ctx, getState());
  enterRule(_localctx, 48, TinyParser::RulePrimary);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(275);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TinyParser::INT_LIT: {
        enterOuterAlt(_localctx, 1);
        setState(264);
        match(TinyParser::INT_LIT);
        break;
      }

      case TinyParser::FLOAT_LIT: {
        enterOuterAlt(_localctx, 2);
        setState(265);
        match(TinyParser::FLOAT_LIT);
        break;
      }

      case TinyParser::STRING_LIT: {
        enterOuterAlt(_localctx, 3);
        setState(266);
        match(TinyParser::STRING_LIT);
        break;
      }

      case TinyParser::TRUE: {
        enterOuterAlt(_localctx, 4);
        setState(267);
        match(TinyParser::TRUE);
        break;
      }

      case TinyParser::FALSE: {
        enterOuterAlt(_localctx, 5);
        setState(268);
        match(TinyParser::FALSE);
        break;
      }

      case TinyParser::IDENT: {
        enterOuterAlt(_localctx, 6);
        setState(269);
        match(TinyParser::IDENT);
        break;
      }

      case TinyParser::T__7: {
        enterOuterAlt(_localctx, 7);
        setState(270);
        arrayLiteral();
        break;
      }

      case TinyParser::T__0: {
        enterOuterAlt(_localctx, 8);
        setState(271);
        match(TinyParser::T__0);
        setState(272);
        expression();
        setState(273);
        match(TinyParser::T__1);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayLiteralContext ------------------------------------------------------------------

TinyParser::ArrayLiteralContext::ArrayLiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

TinyParser::ExpressionListContext* TinyParser::ArrayLiteralContext::expressionList() {
  return getRuleContext<TinyParser::ExpressionListContext>(0);
}


size_t TinyParser::ArrayLiteralContext::getRuleIndex() const {
  return TinyParser::RuleArrayLiteral;
}


std::any TinyParser::ArrayLiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitArrayLiteral(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ArrayLiteralContext* TinyParser::arrayLiteral() {
  ArrayLiteralContext *_localctx = _tracker.createInstance<ArrayLiteralContext>(_ctx, getState());
  enterRule(_localctx, 50, TinyParser::RuleArrayLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(277);
    match(TinyParser::T__7);
    setState(279);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 132353783496962) != 0)) {
      setState(278);
      expressionList();
    }
    setState(281);
    match(TinyParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionListContext ------------------------------------------------------------------

TinyParser::ExpressionListContext::ExpressionListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<TinyParser::ExpressionContext *> TinyParser::ExpressionListContext::expression() {
  return getRuleContexts<TinyParser::ExpressionContext>();
}

TinyParser::ExpressionContext* TinyParser::ExpressionListContext::expression(size_t i) {
  return getRuleContext<TinyParser::ExpressionContext>(i);
}


size_t TinyParser::ExpressionListContext::getRuleIndex() const {
  return TinyParser::RuleExpressionList;
}


std::any TinyParser::ExpressionListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitExpressionList(this);
  else
    return visitor->visitChildren(this);
}

TinyParser::ExpressionListContext* TinyParser::expressionList() {
  ExpressionListContext *_localctx = _tracker.createInstance<ExpressionListContext>(_ctx, getState());
  enterRule(_localctx, 52, TinyParser::RuleExpressionList);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(283);
    expression();
    setState(288);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == TinyParser::T__3) {
      setState(284);
      match(TinyParser::T__3);
      setState(285);
      expression();
      setState(290);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeSpecContext ------------------------------------------------------------------

TinyParser::TypeSpecContext::TypeSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* TinyParser::TypeSpecContext::INT_T() {
  return getToken(TinyParser::INT_T, 0);
}

tree::TerminalNode* TinyParser::TypeSpecContext::FLOAT_T() {
  return getToken(TinyParser::FLOAT_T, 0);
}

tree::TerminalNode* TinyParser::TypeSpecContext::BOOL_T() {
  return getToken(TinyParser::BOOL_T, 0);
}

tree::TerminalNode* TinyParser::TypeSpecContext::STRING_T() {
  return getToken(TinyParser::STRING_T, 0);
}

TinyParser::TypeSpecContext* TinyParser::TypeSpecContext::typeSpec() {
  return getRuleContext<TinyParser::TypeSpecContext>(0);
}

tree::TerminalNode* TinyParser::TypeSpecContext::INT_LIT() {
  return getToken(TinyParser::INT_LIT, 0);
}


size_t TinyParser::TypeSpecContext::getRuleIndex() const {
  return TinyParser::RuleTypeSpec;
}


std::any TinyParser::TypeSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<TinyVisitor*>(visitor))
    return parserVisitor->visitTypeSpec(this);
  else
    return visitor->visitChildren(this);
}


TinyParser::TypeSpecContext* TinyParser::typeSpec() {
   return typeSpec(0);
}

TinyParser::TypeSpecContext* TinyParser::typeSpec(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  TinyParser::TypeSpecContext *_localctx = _tracker.createInstance<TypeSpecContext>(_ctx, parentState);
  TinyParser::TypeSpecContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 54;
  enterRecursionRule(_localctx, 54, TinyParser::RuleTypeSpec, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(296);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case TinyParser::INT_T: {
        setState(292);
        match(TinyParser::INT_T);
        break;
      }

      case TinyParser::FLOAT_T: {
        setState(293);
        match(TinyParser::FLOAT_T);
        break;
      }

      case TinyParser::BOOL_T: {
        setState(294);
        match(TinyParser::BOOL_T);
        break;
      }

      case TinyParser::STRING_T: {
        setState(295);
        match(TinyParser::STRING_T);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(304);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<TypeSpecContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleTypeSpec);
        setState(298);

        if (!(precpred(_ctx, 1))) throw FailedPredicateException(this, "precpred(_ctx, 1)");
        setState(299);
        match(TinyParser::T__7);
        setState(300);
        match(TinyParser::INT_LIT);
        setState(301);
        match(TinyParser::T__8); 
      }
      setState(306);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 29, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool TinyParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 27: return typeSpecSempred(antlrcpp::downCast<TypeSpecContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool TinyParser::typeSpecSempred(TypeSpecContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 1);

  default:
    break;
  }
  return true;
}

void TinyParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  tinyParserInitialize();
#else
  ::antlr4::internal::call_once(tinyParserOnceFlag, tinyParserInitialize);
#endif
}
