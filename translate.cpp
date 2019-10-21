#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "st.h"
#include "Code_Instructions.h"


/*struct command{
	int *instr;
	int n;
};

typedef struct command command_t;*/

void translate(char* inst, command_t* buf);
long seek_size(FILE* f_in);
void command_init(command_t* buf, FILE* f_in);
int findNumb(char* inst);
void write(int x, command_t* buf);
void write_buf(command_t* buf);

int main(int argc, char** argv)
{
	FILE* f_in = fopen("instructions", "r");
	assert(f_in);
	FILE* f_tr = fopen("code.bin", "wb");
	assert(f_tr);
	char inst[SIZE_INSTRUCT + 10] = "";
	command_t buf ={};
	command_init (&buf, f_in);

	while(!feof(f_in))
	{
		if (!fgets(inst, SIZE_INSTRUCT, f_in))
		{
			printf("no read fgets\n");
			ferror(f_in);
			if (feof(f_in))
				printf("был достигнут конец файла");
			break;
		}
		//i = 0;
		//printf("%s\n", inst);
		translate(inst, &buf);
		if (strcmp(inst, "end\n") == 0)
		{
			break;
		}
	}
	//write_buf(&buf);
	fwrite((const void*) buf.instr, sizeof(int), buf.n, f_tr);
}

void write_buf(command_t* buf)
{
	for(int i = 0; i < buf->n; i++)
	{
		printf("%d ", buf->instr[i]);
	}
}

void command_init(command_t* buf, FILE* f_in)
{
	long size = seek_size(f_in);
	buf->instr = (int*) calloc(size, sizeof(int));
	buf->n = 0;
}

long seek_size(FILE* f_in)
{
    fseek(f_in, 0, SEEK_END);    //вынести в отдел функ
    long size_byte = ftell(f_in);
    fseek(f_in, 0, SEEK_SET);
    return size_byte;
}

int findNumb(char* inst)
{
	char numb[SIZE_NUMB] = "";
	int j = 0;
	for (int i = 0; i < SIZE_INSTRUCT; i++)
	{
		if(isdigit(inst[i]))
			numb[j++] = inst[i];
	} 
	return atoi(numb);
}

void write(int x, command_t* buf)
{
	assert(buf);
	buf->instr[buf->n++] = x;
}

void translate(char *inst,command_t *buf)
{
	elem_t x = 0;
	if (strncmp(inst, "push ax", 7) == 0)
	{
		write(FPUSH_R, buf);
		write(AX, buf);
	}
	else if (strncmp(inst, "push bx", 7) == 0)
	{
		write(FPUSH_R, buf);
		write(BX, buf);
	}
	else if (strncmp(inst, "push cx", 7) == 0) 
	{
		write(FPUSH_R, buf);
		write(CX, buf);
	}
	else if (strncmp(inst, "push dx", 7) == 0)
	{
		write(FPUSH_R, buf);
		write(DX, buf);
	}
	else if (strncmp(inst, "push", 4) == 0)
	{
		write(FPUSH, buf);
		x = findNumb(inst);
		write(x, buf);
	}
		if (strncmp(inst, "pop ax", 6) == 0)
	{
		write(FPOP_R, buf);
		write(AX, buf);
	}
	else if (strncmp(inst, "pop bx", 6) == 0)
	{
		write(FPOP_R, buf);
		write(BX, buf);
	}
	else if (strncmp(inst, "pop cx", 6) == 0) 
	{
		write(FPOP_R, buf);
		write(CX, buf);
	}
	else if (strncmp(inst, "pop dx", 6) == 0)
	{
		write(FPOP_R, buf);
		write(DX, buf);
	}
	else if (strncmp(inst, "pop", 3) == 0)
	{
		write(FPOP, buf);
	}
	if (strcmp(inst, "add\n") == 0)
	{
		write(FADD, buf);
	}
	if (strcmp(inst, "sub\n") == 0)
	{
		write(FSUB, buf);
	}
	if (strcmp(inst, "mul\n") == 0)
	{
		write(FMUL, buf);
	}
	if (strcmp(inst, "div\n") == 0)
	{
		write(FDIV, buf);
	}
	if (strcmp(inst, "sqrt\n") == 0)
	{
		write(FSQRT, buf);
	}
	if (strcmp(inst, "sin\n") == 0)
	{
		write(FSIN, buf);
	}
	if (strcmp(inst, "cos\n") == 0)
	{
		write(FCOS, buf);
	}
	if (strcmp(inst, "in\n") == 0)
	{
		write(FIN, buf);
	}
	if (strcmp(inst, "out\n") == 0)
	{
		write(FOUT, buf);
	}
	if (strcmp(inst, "end\n") == 0)
	{
		write(FEND, buf);
	}
}