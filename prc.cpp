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
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "st.h"
#include "Code_Instructions.h"

//#define LastVersion 0



main(int argc, char **argv)
{
	stack_t st;
	stack_init(&st);
	command_t buf;
	work(&st, &buf);
	//printf("d = %f", st.data[0]);
	//stack_dump(&st);
	stack_distruct(&st);
	del_file();
}

long size_file(FILE* file)
{
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size /= sizeof(int);
}

void buf_init(command_t *buf, long size)
{
	assert(buf);
	if (size <= 0)
		printf("size of file < 0(buf_init)");

	buf->n = 0;
	buf->instr = (int*) calloc(size, sizeof(int));
	assert(buf->instr);
}

void buf_destruct(command_t *buf)
{
	assert(buf);
	assert(buf->instr);
	free(buf->instr);
}

void work (stack_t* st, command_t *buf)
{
	FILE* f_instr = fopen("code.bin", "rb");
	FILE* f_answ = fopen("answer", "w");

	long fsize = size_file(f_instr);
	buf_init(buf, fsize);

	fread((void*)buf->instr, sizeof(int), fsize, f_instr);

	/*for(int i = 0; i < fsize; i++)
	{
		printf("%d ", buf->instr[i]);
	}*/

	for (buf->n = 0; buf->n < fsize; buf->n++)
	{
		func(buf, st);
	}

	fprintf(f_answ, "%f", pop(st));
	buf_destruct(buf);
	fclose(f_answ);
	fclose(f_instr);
}

void func(command_t* buf, stack_t* st)
{
	elem_t x = 0;
	
	switch (buf->instr[buf->n]) {
		case FPUSH:
			push(st, buf->instr[++buf->n]);
			break;
		case FPUSH_R:
			push(st, st->reg[buf->instr[++buf->n]]);
			break;
		case FPOP:
			break;
		case FPOP_R:
			break;
		case FADD:
			push(st, pop(st) + pop(st));
			break;
		case FSUB:
			x = pop(st);
			push(st, pop(st) - x);
			break;
		case FMUL:
			push(st, pop(st) * pop(st));
			break;
		case FDIV:
			x = pop(st);
			push(st, pop(st) / x);
			break;
		case FSQRT:
			push(st, sqrt(pop(st)));
			break;
		case FSIN:
			push(st, sin(pop(st)));
			break;
		case FCOS:
			push(st, cos(pop(st)));
			break;
		case FIN:
			printf("введите данные с клавиатуры\t");
			scanf("%f", &x);
			push(st, x);
			break;
		case FOUT:
			printf("pop = %f\n", pop(st));
			break;
		case FEND:
			break;
		default:
			printf("func get no code of command %d\n", buf->instr[buf->n]);
			break;
	}
	stack_dump(st);
}
