// ============================================================================
// Tiny Language — ANTLR4 Grammar
// A simple, statically-typed language for practicing ANTLR + C++ + LLVM
//
// Features exercised:
//   - Scalar types: int, float, bool, string
//   - Mutable & immutable bindings (let / var)
//   - Arithmetic, comparison, logical operators
//   - Control flow: if/else, while, for
//   - Functions with parameters & return types
//   - Lambda expressions / closures (first-class functions)
//   - Arrays (fixed-size)
//   - Print built-in
//   - Single-line & block comments
//
// Build:
//   antlr4 -Dlanguage=Cpp Tiny.g4
//   # produces TinyLexer.{h,cpp} and TinyParser.{h,cpp}
// ============================================================================

grammar Tiny;

// ── Top-Level ───────────────────────────────────────────────────────────────

program
    : declaration* EOF
    ;

declaration
    : functionDecl
    | statement
    ;

// ── Functions ───────────────────────────────────────────────────────────────

functionDecl
    : 'fn' IDENT '(' paramList? ')' ('->' typeSpec)? block
    ;

paramList
    : param (',' param)*
    ;

param
    : IDENT ':' typeSpec
    ;

// ── Statements ──────────────────────────────────────────────────────────────

statement
    : varDecl ';'
    | assignment ';'
    | returnStmt ';'
    | printStmt ';'
    | ifStmt
    | whileStmt
    | forStmt
    | exprStmt ';'
    | block
    ;

varDecl
    : 'let' IDENT (':' typeSpec)? '=' expression          // immutable
    | 'var' IDENT (':' typeSpec)? '=' expression           // mutable
    ;

assignment
    : IDENT '=' expression
    | IDENT '[' expression ']' '=' expression              // array element
    ;

returnStmt
    : 'return' expression?
    ;

printStmt
    : 'print' '(' expressionList? ')'
    ;

ifStmt
    : 'if' '(' expression ')' block ('else' (ifStmt | block))?
    ;

whileStmt
    : 'while' '(' expression ')' block
    ;

forStmt
    : 'for' '(' IDENT 'in' expression '..' expression ')' block
    ;

exprStmt
    : expression
    ;

block
    : '{' statement* '}'
    ;

// ── Expressions (precedence climbing) ───────────────────────────────────────

expression
    : logicalOr
    ;

logicalOr
    : logicalAnd ('||' logicalAnd)*
    ;

logicalAnd
    : equality ('&&' equality)*
    ;

equality
    : comparison (('==' | '!=') comparison)*
    ;

comparison
    : addition (('<' | '<=' | '>' | '>=') addition)*
    ;

addition
    : multiplication (('+' | '-') multiplication)*
    ;

multiplication
    : unary (('*' | '/' | '%') unary)*
    ;

unary
    : ('-' | '!') unary
    | postfix
    ;

postfix
    : primary ( '(' expressionList? ')'                     // function call
              | '[' expression ']'                           // array index
              )*
    ;

primary
    : INT_LIT
    | FLOAT_LIT
    | STRING_LIT
    | 'true'
    | 'false'
    | IDENT
    | arrayLiteral
    | lambdaExpr                                             // fn(x: int) -> int { ... }
    | '(' expression ')'
    ;

// ── Lambda Expression ───────────────────────────────────────────────────────
// Syntax: fn(params) -> returnType { body }
// The 'fn' keyword without a name creates an anonymous function (closure).

lambdaExpr
    : 'fn' '(' paramList? ')' ('->' typeSpec)? block
    ;

arrayLiteral
    : '[' expressionList? ']'
    ;

expressionList
    : expression (',' expression)*
    ;

// ── Types ───────────────────────────────────────────────────────────────────

typeSpec
    : 'int'                                                 // i64
    | 'float'                                               // f64
    | 'bool'                                                // i1
    | 'string'                                              // i8* (ptr)
    | typeSpec '[' INT_LIT ']'                               // fixed array
    | 'fn' '(' typeList? ')' ('->' typeSpec)?                // function type
    ;

typeList
    : typeSpec (',' typeSpec)*
    ;

// ── Lexer Rules ─────────────────────────────────────────────────────────────

// Keywords (listed explicitly so IDENT doesn't capture them)
LET     : 'let';
VAR     : 'var';
FN      : 'fn';
RETURN  : 'return';
IF      : 'if';
ELSE    : 'else';
WHILE   : 'while';
FOR     : 'for';
IN      : 'in';
PRINT   : 'print';
TRUE    : 'true';
FALSE   : 'false';
INT_T   : 'int';
FLOAT_T : 'float';
BOOL_T  : 'bool';
STRING_T: 'string';

// Literals
INT_LIT
    : [0-9]+
    ;

FLOAT_LIT
    : [0-9]+ '.' [0-9]+
    ;

STRING_LIT
    : '"' (~["\\] | '\\' .)* '"'
    ;

// Identifiers
IDENT
    : [a-zA-Z_] [a-zA-Z0-9_]*
    ;

// Operators & Punctuation (handled inline in parser rules)

// Whitespace & Comments — skip
WS          : [ \t\r\n]+       -> skip ;
LINE_COMMENT: '//' ~[\r\n]*    -> skip ;
BLOCK_COMMENT: '/*' .*? '*/'   -> skip ;
