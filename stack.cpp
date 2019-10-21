/*
Nikita Oskolkov
Stack must contain numbers

in stack_t:
errcode == 0 => 0 errors
in function write funcname to stack ok
fname = "...\n";

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "st.h"

#define NoLastVersion 1

int count_dump;
#define C_D 1

int stack_dump(stack_t *st)
{
	assert(st);
	FILE* fdump = fopen("st_dump", "a");
	if (fdump == NULL){
		perror("in stack_ok file doesn't open");
		return ISERR;
	} else
	{
#if C_D
		if(count_dump++ == 0)
		fprintf(fdump, "\n~~~~~~~~~~~~~~~~~~~~~~~~~IT IS NEW DUUUUUMP~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif
		fprintf(fdump, "stackDump \"%s\" [%p] (%s)\n ", st->name, st, stack_ok(st, "dump")?"ok":"no ok");
		fprintf(fdump, "\t{\n\tsize = %d\n\tdata[%lld] = %p\n", st->size, st->maxsize - 1, &st->data[st->maxsize - 1]);
		fprintf(fdump, "\t\t{\n");
		for (int i = 0; i < st->maxsize - 1; i++)
		{
			fprintf(fdump,"\t\t\t%c[%d] = %f%s\n",(i < st->size) ? '*' : ' ', i, st->data[i], (i < st->size)?"":" (poison?)") ;
		}
		fprintf(fdump, "\t\t}\n");
		fprintf(fdump, "\t\tcanary1 = %lld [%p];\n", st->canary1, &st->canary1);
		fprintf(fdump, "\t\thash = %lld;\n", st->hash);
		fprintf(fdump, "\t\tcanary2 = %lld [%p];\n", st->canary2, &st->canary2);
		fprintf(fdump, "\t}\n\n");
		fclose(fdump);
		return NOERR;
	}
}

void del_file()
{
	printf("\nхотите ли вы удалить файл \"stack_ok\"? да - 1, нет - 0\n");
	int ok = 0;
	scanf("%d", &ok);
	if (ok)
		if (remove("stack_ok") == -1)
			perror("stack_ok didn't delete");
	printf("\nхотите ли вы удалить файл \"st_dump\"? да - 1, нет - 0\n");
	scanf("%d", &ok);
	if (ok)
		if (remove("st_dump") == -1)
			perror("st_dump didn't delete");

}

long long f_hash(stack_t* st)
{
	assert(st);
	//char fname[] = "f_hash\n";
	long long hash = st->canary1*31 + st->canary2*13;
	int i = 0;

	for (i = 0; i < st->size; i++)
	{
		hash += st->data[i]*(i + 3);
	}

	return hash;
}

int stack_ok(stack_t *st,char* fname)
{
	assert(st);
	int n_err = 0;
	FILE* file_ok = fopen("stack_ok", "a");
	if (file_ok == NULL){
		perror("in stack_ok file doesn't open");
		return  NOERR;
	}
	else 
	{
		fputs(fname, file_ok);
		if (st->canary1 != st->canary2 && st->canary1 != CANARY)
		{
				fputs("canarys were changed\n", file_ok);
				n_err++;
		}
		if (f_hash(st) != st->hash)
		{
			fputs("hash was changed\n", file_ok);
			n_err++;
		}
		if (st->size > st->maxsize)
		{
			fputs("problem with stack's size\n", file_ok);
			n_err++;
		}
		if (st->hash != f_hash(st))//dada == null
		{
			fputs("stack's hash != f_hash\n", file_ok);
			n_err++;
		}
		if (st->data == NULL)
		{
			fputs("st->data == NULL\n", file_ok);
			n_err++;
		}
		if (!n_err)
		{
			fputs("no errors\n", file_ok);
			fclose(file_ok);
			return NOERR;
		}
	}
	return ISERR;
}



void push(stack_t* st, elem_t elem)
{
#if NoLastVersion
	assert(st);
	char fname[] = "push\n";
	stack_ok(st, fname);
#endif
	if (st->size >= st->maxsize)
		stack_change(st, ST_MORE);
	st->data[st->size++] = elem;
#if NoLastVersion
	st->hash = f_hash(st);
#endif
}

elem_t pop(stack_t* st)
{
#if NoLastVersion
	assert(st);
	char fname[] = "pop\n";
	stack_ok(st, fname);
#endif
	elem_t elem = st->data[--st->size];
#if NoLastVersion
	st->hash = f_hash(st);
#endif
	if(st->size < (st->maxsize / FACTOR - 100))
		stack_change(st, ST_LESS);
	return elem;
}

void stack_change(stack_t *st, int flag)
{
#if NoLastVersion
	assert(st);
#endif
	if (flag)
		st->maxsize = st->maxsize * FACTOR;
	else
	{
		st->maxsize = (st->maxsize / FACTOR);
	}	
	st->data = (elem_t*) realloc(st->data, sizeof(elem_t) * st->maxsize);
#if NoLastVersion
	if (!st->data)
		perror("ralloc didn't give memory");
#endif
}

void stack_init(stack_t* st)
{
#if NoLastVersion
	assert(st);
#endif
	st->data = (elem_t*) calloc(INIT_SIZE, sizeof(elem_t));
	st->reg = (int*) calloc(COUNT_R, sizeof(int));
#if NoLastVersion
	assert(st->data);
#endif
	st->maxsize = INIT_SIZE;
	st->size = 0;
	st->canary1 = CANARY;
	st->canary2 = CANARY;
#if NoLastVersion
	st->hash = f_hash(st);
#if C_D
	count_dump = 0;
#endif
#endif
}

void stack_distruct(stack_t* st)
{
#if NoLastVersion
	assert(st);
#endif
	int i = 0;
	for (i = 0; i < st->size; i++)
		st->data[i] = POIZON;
	st->canary1 = POIZON;
	st->canary2 = POIZON;
	st->size = POIZON;
	st->maxsize = POIZON;
	st->hash = POIZON;
	free(st->data);
	free(st->reg);
}
