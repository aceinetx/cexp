#include <cexp.h>
#include <stdio.h>

void do_expression(char* expression) {
	Cexp_Result result;
	result = eval(expression);

	if (*result.error) {
		printf("%s\n", result.error);
		return;
	}

	printf("%f\n", result.value);
}

int main(void) {
	do_expression("69.123 / (1 + 2)");
	do_expression("69 / (1 + 2)");
	do_expression("0.1 + 0.2");
	do_expression("(69 / (1 + 2)) * (1 * 3)");
	do_expression("(69 / (3 - 2 - 1))");
	return 0;
}
