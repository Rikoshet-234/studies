#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>


void str_shift_spaces(char *str, size_t len)
{
	char *i, *j;
	for (j = i = str + len - 1; i != str - 1; i--, j -= *j != ' ') {
		*j = *i;
	}

	for (; j != str - 1; j--) {
		*j = ' ';
	}
}


int main(void)
{
	char t[] = "Ala ma kota";
	str_shift_spaces(t, strlen(t));
	printf("%s\n", t);

	return 0;
}
