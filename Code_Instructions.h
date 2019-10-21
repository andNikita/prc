/*
0 - 9 - registers
10 - 30- commands
31 - 50 - commands with using registers
91 - 98 - commands with using std_in/out
99 - end
*/
#define FPUSH 11
#define FPUSH_R 31
#define FPOP 12
#define FPOP_R 32
#define FADD 13
#define FSUB 14
#define FMUL 15
#define FDIV 16
#define FSQRT 17
#define FSIN 18
#define FCOS 19
#define FIN 91
#define FOUT 92
#define FEND 99
#define AX 0 // номер регистра - номер элемента в массиве регистров
#define BX 1
#define CX 2
#define DX 3