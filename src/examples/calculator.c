#include <cexp.h>
#include <stdio.h>
#include <string.h>

void do_expression(char *expression) {
	Cexp_Result result;
	result = eval(expression);

	if (*result.error) {
		printf("%s\n", result.error);
		return;
	}

	printf("%f\n", result.value);
}

int main() {
	for (;;) {
		char buf[255];

		printf(">>> ");
		fgets(buf, sizeof(buf), stdin);

		if (strcmp(buf, "exit") == 10)
			break;

		do_expression(buf);
	}
	return 0;
}
