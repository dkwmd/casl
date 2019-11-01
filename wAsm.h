#pragma once


#define	GR0		0
#define	GR1		1
#define	GR2		2
#define	GR3		3
#define	GR4		4

#define	PC		5
#define	FR		6

//////////////////////////////////////////////////////////
enum	{
	START,
	END,
	IN,
	OUT,
	DC,
	DS,
	EXIT,
	LD,
	ST,
	LEA,
	ADD,
	SUB,
	AND,
	OR,
	EOR,
	CPA,
	CPL,
	SLA,
	SRA,
	SLL,
	SRL,
	JMP,
	JPZ,
	JMI,
	JNZ,
	JZE,
	PUSH,
	POP,
	CALL,
	RET,
	HALT,	/* �������ģʽ�£�halt���������У�����ģʽ�£����н����˳�������� */
};

#define BAD		-1	/* not an instruction */


class wAsm
{
public:
	static char *R_STR[];
	static char *I_Set[];

	wAsm(void);
	~wAsm(void);
	static int InstructionLen(int cmd);	//ָ��ռ�ö��ٸ���(16bit)
	static int Cmd_StrToBin(char* StrCmd);
	static char* Cmd_BinToStr(int cmdbin);
	static int isreg(char* str);
	static int getXRegNum(char* str,int type);
};
