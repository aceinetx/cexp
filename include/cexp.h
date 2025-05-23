#ifndef CEXP_H
#define CEXP_H

/* CONFIG */
#define CEXP_STACK_SIZE 128
#define CEXP_MAX_TOKENS 128
#define CEXP_ERROR_LEN 64

typedef double cexp_number;

typedef struct Cexp_Token {
	enum { CEXP_NONE, CEXP_NUMBER, CEXP_OPERATOR, CEXP_LPAREN, CEXP_RPAREN } type;
	cexp_number value;
	char ch;
} Cexp_Token;

typedef struct Cexp_Lexer {
	char* expression;
	char* p;
} Cexp_Lexer;

typedef struct Cexp_Result {
	char error[CEXP_ERROR_LEN];
	cexp_number value;
} Cexp_Result;

Cexp_Lexer cexp_lexer_new(char* expression);
void cexp_lexer_delete(Cexp_Lexer* lexer);
Cexp_Token cexp_lexer_next(Cexp_Lexer* this);

Cexp_Result eval_raw(Cexp_Lexer* lexer);

/* quick eval: constructs Cexp_Lexer for you */
Cexp_Result eval(char* expression);

double cexp_pow(double base, int exp);

#endif
