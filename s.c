#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

main()
{
	char c[10] = "";
	fgets(c, 10, stdin);
	printf("%d", strcmp(c, "yes\n"));
}