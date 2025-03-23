#include <cexp.h>
#include <stdio.h>
#include <string.h>

int main() {
	printf("%f\n", eval("69.123 / (1 + 2)"));
	printf("%f\n", eval("69 / (1 + 2)"));
	printf("%f\n", eval("0.1 + 0.2"));
	printf("%f\n", eval("(69 / (1 + 2)) * (1*3)"));
	return 0;
}
