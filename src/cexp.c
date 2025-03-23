#include <cexp.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define cexp_bool char
#define true 1
#define false 0

typedef struct {
	cexp_number items[CEXP_STACK_SIZE];
	size_t top;
} NumStack;

typedef struct {
	char items[CEXP_STACK_SIZE];
	size_t top;
} CharStack;

static void cexp_init_num_stack(NumStack *s) {
	s->top = -1;
}

static void cexp_push_num(NumStack *s, cexp_number value) {
	s->items[++(s->top)] = value;
}

static cexp_number cexp_pop_num(NumStack *s) {
	return s->items[(s->top)--];
}

static void cexp_init_char_stack(CharStack *s) {
	s->top = -1;
}

static void cexp_push_char(CharStack *s, char value) {
	s->items[++(s->top)] = value;
}

static char cexp_pop_char(CharStack *s) {
	return s->items[(s->top)--];
}

static char cexp_peek_char(CharStack *s) {
	return s->items[s->top];
}

static bool cexp_is_empty_char(CharStack *s) {
	return s->top == (size_t)-1;
}

static cexp_bool cexp_is_digit(char c) {
	return (c >= '0' && c <= '9');
}
static cexp_bool cexp_is_operator(char c) {
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

static Cexp_Result cexp_result_new() {
	Cexp_Result result;
	memset(result.error, 0, sizeof(result.error));
	result.value = 0;
	return result;
}

static Cexp_Result perform_operation(cexp_number operand1, cexp_number operand2, char operator) {
	Cexp_Result result = cexp_result_new();

	switch (operator) {
	case '+':
		result.value = operand1 + operand2;
		break;
	case '-':
		result.value = operand1 - operand2;
		break;
	case '*':
		result.value = operand1 * operand2;
		break;
	case '/':
		if (operand2 == 0) {
			snprintf(result.error, sizeof(result.error), "Division by 0: \"%f / %f\"", (double)operand1, (double)operand2);
			break;
		}
		result.value = operand1 / operand2;
		break;
	}
	return result;
}

Cexp_Lexer cexp_lexer_new(char *expression) {
	Cexp_Lexer lexer;
	lexer.expression = expression;
	lexer.p = lexer.expression;

	return lexer;
}

void cexp_lexer_delete(Cexp_Lexer *lexer) {
	/* nothing */
}

static Cexp_Token cexp_lexer_number(Cexp_Lexer *this) {
	Cexp_Token token;
	cexp_number left_part, right_part;
	size_t right_part_length;
	cexp_bool float_part;

	token.type = CEXP_NUMBER;
	token.value = 0;

	left_part = 0;
	right_part = 0;
	right_part_length = 0;

	float_part = false;

	while (*this->p) {
		cexp_number *part;

		if (*this->p == '.') {
			float_part = true;
			this->p++;
			continue;
		} else if (!cexp_is_digit(*this->p))
			break;

		part = &left_part;
		if (float_part) {
			part = &right_part;
			right_part_length++;
		}

		*part *= 10;
		*part += *this->p - '0';

		this->p++;
	}

	token.value = (left_part + (right_part / (cexp_number)pow(10, right_part_length)));

	return token;
}

Cexp_Token cexp_lexer_next(Cexp_Lexer *this) {
	Cexp_Token token;

	while (*this->p) {
		if (cexp_is_digit(*this->p)) {
			token = cexp_lexer_number(this);
			return token;
		} else if (cexp_is_operator(*this->p)) {
			token.type = CEXP_OPERATOR;
			token.ch = *this->p;
			this->p++;
			return token;
		} else if (*this->p == '(') {
			token.type = CEXP_LPAREN;
			token.ch = *this->p;
			this->p++;
			return token;
		} else if (*this->p == ')') {
			token.type = CEXP_RPAREN;
			token.ch = *this->p;
			this->p++;
			return token;
		}

		this->p++;
	}

	token.value = 0;
	token.type = CEXP_NONE;
	return token;
}

Cexp_Result eval_raw(Cexp_Lexer *lexer) {
	NumStack values;
	CharStack operators;
	Cexp_Token token;
	Cexp_Result op_res, result;

	cexp_init_num_stack(&values);
	cexp_init_char_stack(&operators);

	token = cexp_lexer_next(lexer);
	while (token.type != CEXP_NONE) {
		if (token.type == CEXP_NUMBER) {
			cexp_push_num(&values, token.value);
		} else if (token.type == CEXP_OPERATOR) {
			while (!cexp_is_empty_char(&operators) && cexp_is_operator(cexp_peek_char(&operators)) && ((token.ch == '+' || token.ch == '-') || ((token.ch == '*' || token.ch == '/') && (cexp_peek_char(&operators) == '*' || cexp_peek_char(&operators) == '/')))) {
				cexp_number operand2, operand1;

				operand2 = cexp_pop_num(&values);
				operand1 = cexp_pop_num(&values);

				op_res = perform_operation(operand1, operand2, cexp_pop_char(&operators));

				if (*op_res.error) {
					return op_res;
				}

				cexp_push_num(&values, op_res.value);
			}
			cexp_push_char(&operators, token.ch);
		} else if (token.type == CEXP_LPAREN) {
			cexp_push_char(&operators, token.ch);
		} else if (token.type == CEXP_RPAREN) {
			while (!cexp_is_empty_char(&operators) && cexp_peek_char(&operators) != '(') {
				cexp_number operand2, operand1;

				operand2 = cexp_pop_num(&values);
				operand1 = cexp_pop_num(&values);

				op_res = perform_operation(operand1, operand2, cexp_pop_char(&operators));

				if (*op_res.error) {
					return op_res;
				}

				cexp_push_num(&values, op_res.value);
			}
			cexp_pop_char(&operators);
		}
		token = cexp_lexer_next(lexer);
	}

	while (!cexp_is_empty_char(&operators)) {
		cexp_number operand2, operand1;

		operand2 = cexp_pop_num(&values);
		operand1 = cexp_pop_num(&values);

		op_res = perform_operation(operand1, operand2, cexp_pop_char(&operators));

		if (*op_res.error) {
			return op_res;
		}

		cexp_push_num(&values, op_res.value);
	}

	result = cexp_result_new();
	result.value = cexp_pop_num(&values);

	return result;
}

Cexp_Result eval(char *expression) {
	Cexp_Lexer lexer;
	Cexp_Result result = cexp_result_new();

	lexer = cexp_lexer_new(expression);

	result = eval_raw(&lexer);

	cexp_lexer_delete(&lexer);

	return result;
}
