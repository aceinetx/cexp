#include <cexp.h>

double cexp_pow(double base, int exp) {
	double result;
	if (exp == 0) {
		return 1.0; /* Any number to the power of 0 is 1 */
	}
	if (exp < 0) {
		base = 1.0 / base; /* Handle negative exponents */
		exp = -exp;
	}

	result = 1.0;
	while (exp > 0) {
		if (exp % 2 == 1) { /* If exp is odd */
			result *= base;
		}
		base *= base; /* Square the base */
		exp /= 2;			/* Divide exp by 2 */
	}
	return result;
}
