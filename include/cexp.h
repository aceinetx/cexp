#include <stdbool.h>

/* CONFIG */
#define CEXP_STACK_SIZE 128
#define CEXP_MAX_TOKENS 128

typedef double cexp_number;

typedef struct Cexp_Token {
  enum { CEXP_NONE, CEXP_NUMBER, CEXP_OPERATOR, CEXP_LPAREN, CEXP_RPAREN } type;
  cexp_number value;
  char ch;
} Cexp_Token;

typedef struct Cexp_Lexer {
  char *expression;
  char *p;
} Cexp_Lexer;

Cexp_Lexer *cexp_lexer_new(char *expression);
void cexp_lexer_delete(Cexp_Lexer *lexer);
Cexp_Token cexp_lexer_next(Cexp_Lexer *this);

cexp_number eval_raw(Cexp_Lexer *lexer);

/* quick eval: constructs Cexp_Lexer for you */
cexp_number eval(char *expression);