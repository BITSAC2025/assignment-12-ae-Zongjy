#include "stdbool.h"
extern void svf_assert(bool);
int main() {
	int a = 10;
	int b = 2;
	if (b == 2)
		a++;
	else
		a--;
	svf_assert(a == 11);
	return 0;
}
