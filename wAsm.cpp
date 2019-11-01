#include "StdAfx.h"
#include ".\wasm.h"

/* to get the string version of a register, use  R_STR[$IP]="$IP" */

char *wAsm::R_STR[] = 
{
	"GR0","GR1","GR2","GR3","GR4","PC","FR",
};

char *wAsm::I_Set[] = 
	{
		"START",
		"END",
		"IN",
		"OUT",
		"DC",
		"DS",
		"EXIT",
		"LD",
		"ST",
		"LEA",
		"ADD",
		"SUB",
		"AND",
		"OR",
		"EOR",
		"CPA",
		"CPL",
		"SLA",
		"SRA",
		"SLL",
		"SRL",
		"JMP",
		"JPZ",
		"JMI",
		"JNZ",
		"JZE",
		"PUSH",
		"POP",
		"CALL",
		"RET",
		"HALT",
	};


wAsm::wAsm(void)
{
}

wAsm::~wAsm(void)
{
}

int wAsm::Cmd_StrToBin(char* StrCmd)
{
	int i,s;
	s=sizeof(I_Set)/sizeof(I_Set[0]);
	for(i=0;i<s;i++)
		if(!strcmp(StrCmd,I_Set[i]))
			return i;
	return -1;
}

char* wAsm::Cmd_BinToStr(int cmdbin)
{
	int s;
	s=sizeof(I_Set)/sizeof(I_Set[0]);
	if(cmdbin<s)
		return I_Set[cmdbin];
	return NULL;
}

int wAsm::isreg(char* str)
{
	int i;
	for(i=0;i<sizeof(R_STR)/sizeof(R_STR[0]);i++)
		if(!strcmp(str,R_STR[i]))
			return 1;
	return 0;
}

int wAsm::getXRegNum(char* str,int type)
{
	int i;
	switch(type){
		case 1:
			for(i=0;i<sizeof(R_STR)/sizeof(R_STR[0]);i++)
				if(!strcmp(str,R_STR[i]))
					return i;
			break;
	}
	printf("wAsm::getXRegNum() error , unknow register. %s ,d(%d)\n",str,type);
	return -1;	//never go here !
}

int wAsm::InstructionLen(int cmd)
{
	int l;
	switch(cmd){
	case IN:
	case OUT:
		l=3;
		break;
	default:
		l=2;
		break;
	}
	return l;
}
