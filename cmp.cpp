#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

main()
{
	char c[10] = "";
	gets(c);
	printf("%d", strcmp(c, "yes"));
}