// comet.cpp: implementation of the comet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "comet.h"
#include "MapSymItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

comet::comet()
{

}

comet::~comet()
{

}

void comet::init()
{
	int i;
	for(i=0;i<sizeof(r)/sizeof(r[0]);i++)
		r[i]=0;
	r[GR4]=0x7fff;
}

INT16 comet::runOne()
{
	INT16 pc;
	INT16 cmd,GR,ADR,XR;
	UINT16 *p1,*p2;
	int   i;
	char  line[64];
	pc=r[PC];

	getramcmd((char*)&ram[pc],&cmd,&GR,&ADR,&XR);
	pc+=2;

	if(XR!=0x0f){
		ADR += r[XR];
	}

	switch(cmd){
	case IN:
		pc=r[PC];
		GR=ram[pc+1];
		XR=ram[pc+2];
		printf("input %d :",&ram[XR]-&ram[GR]);
		scanf("%s",line);
		for(i=0;i<strlen(line);i++)
			ram[GR+i]=line[i];
		ram[XR]=i;
		pc+=3;
		break;
	case OUT:
		printf("output:");
		pc=r[PC];
		GR=ram[pc+1];
		XR=ram[pc+2];
		for(i=0;i<ram[XR];i++)
			putchar(ram[GR+i]);
		pc+=3;
		break;

	case LD:
		if(AddrAvailability(ADR))
			r[GR]=ram[ADR];
		break;
	case ST:
		if(AddrAvailability(ADR))
			ram[ADR]=r[GR];
		break;
	case LEA:
		r[GR]=ADR;
		SetFR(r[GR]);
		break;
	case ADD:
		if(AddrAvailability(ADR))
			r[GR]=r[GR]+ram[ADR];
		SetFR(r[GR]);
		break;
	case SUB:
		if(AddrAvailability(ADR))
			r[GR]=r[GR]-ram[ADR];
		SetFR(r[GR]);
		break;
	case AND:
		if(AddrAvailability(ADR))
			r[GR]=r[GR] & ram[ADR];
		SetFR(r[GR]);
		break;
	case OR:
		if(AddrAvailability(ADR))
			r[GR]=r[GR] | ram[ADR];
		SetFR(r[GR]);
		break;
	case EOR:
		if(AddrAvailability(ADR))
			r[GR]=r[GR] ^ ram[ADR];
		SetFR(r[GR]);
		break;
	case CPA:
		if(AddrAvailability(ADR)){
			INT16 tmp;
			tmp=r[GR]-ram[ADR];
			SetFR(tmp);
		}
		break;
	case CPL:
		{
			p1=(UINT16*)&r[GR];
			p2=(UINT16*)&ram[ADR];
			if(*p1 > *p2)		r[FR]=0;
			else if(*p1==*p2)	r[FR]=1;
			else			r[FR]=2;
		}
		break;
	case SLA:		
		r[GR]<<=ADR;
		SetFR(r[GR]);
		break;
	case SRA:
		r[GR]>>=ADR;
		SetFR(r[GR]);
		break;
	case SLL:
		p1=(UINT16*)&r[GR];
		p2=(UINT16*)&ADR;
		*p1 <<= *p2;
		SetFR(r[GR]);
		break;
	case SRL:
		p1=(UINT16*)&r[GR];
		p2=(UINT16*)&ADR;
		*p1 >>= *p2;
		SetFR(r[GR]);
		break;
	case JMP:
		pc=ADR;
		break;
	case JPZ:
		if(r[FR]==0 || r[FR]==1)
			pc=ADR;
		break;
	case JMI:
		if(r[FR]==2)
			pc=ADR;
		break;
	case JNZ:
		if(r[FR]==0 || r[FR]==2)
			pc=ADR;
		break;
	case JZE:
		if(r[FR]==1)
			pc=ADR;
		break;
	case PUSH:
		r[GR4]--;
		if(AddrAvailability(r[GR4])){
			ram[r[GR4]]=ADR;
		}
		break;
	case POP:
		if(AddrAvailability(r[GR4])){
			r[GR]=ram[r[GR4]];
		}
		r[GR4]++;
		break;
	case CALL:
		if(AddrAvailability(ADR)){
			r[GR4]--;
			ram[r[GR4]]=pc;
			pc=ADR;
		}
		break;
	case RET:
		if(AddrAvailability(r[GR4])){
			pc=ram[r[GR4]];
		}
		r[GR4]++;
		break;
	case HALT:
	default:
		cmd=HALT;
		break;
	}
	r[PC] = pc;
	return cmd;
}

void comet::getramcmd(char *rm, INT16 *cmd, INT16 *GR, INT16 *ADR, INT16 *XR)
{
	*cmd=rm[0];
	*GR	=(rm[1]& 0xf0)>>4;
	*XR =rm[1]& 0x0f;
	*ADR=*((INT16*)(rm+2));
}

void comet::run(int runMode)
{
	/* 0:直接运行，1：单步模式，2：执行到模式；3：运行直到结束返回命令行*/
	debugMode=runMode;

	while(1){
		if(debugMode==1){
			printCmd(r[PC],1);
			debugCmd();
		}
		else if(debugMode==2 && r[PC]==overPc){
			printCmd(r[PC],1);
			debugCmd();
		}
		if(runOne()==HALT){
			if(debugMode>0){
				debugCmd();
				break;
			}
			if(debugMode==0)
				break;			
		}
	}
}

void comet::printCmd(int start_pc, int num)
{
	int i;
	int cmd;
	for(i=0;i<num;i++){
		cmd=printCmd2(start_pc);
		start_pc+=wAsm::InstructionLen(cmd);
	}
}

int comet::printCmd2(int pc)
{
	INT16 cmd,GR,ADR,XR;

	getramcmd((char*)&ram[pc],&cmd,&GR,&ADR,&XR);

	printf("%4d: ",pc);
	if(cmd<0 || cmd>HALT){
		printf("\n");
		return cmd;
	}
	printf("%8s ",wAsm::I_Set[cmd]);
	if(cmd==IN || cmd==OUT){
		printf("%3d(0x%X) ",ram[pc+1],ram[pc+1] & 0x0000ffff);
		printf("%3d(0x%X) ",ram[pc+2],ram[pc+2] & 0x0000ffff);
		printf("\n");
		return cmd;
	}
	if(cmd==HALT){
		printf("\n");
		return cmd;
	}
	if(GR!=0x0f)
		printf("%4s,",wAsm::R_STR[GR]);
	printf("%3d(0x%X)",ADR,ADR & 0x0000ffff);
	if(XR!=0x0f)
		printf(",%s",wAsm::R_STR[XR]);
	printf("\n");
	return cmd;
}

void comet::debugCmd()
{
	char line[64];

	if(debugMode==1 || debugMode==2){
		int i;
		for(i=0;i<7;i++){
			printf("%3s=%3d\t",wAsm::R_STR[i],r[i]);
		}
	}
	debugMode=-1;
	do{
		printf("\n-");
		scanf("%s",line);
		switch(line[0]){
		case '?':
			printf("  l:	list asm command\n");
			printf("  d:	view data area\n");
			printf("  r:	view register\n");
			printf("  g:	run \n");
			printf("  q:	quit \n");
			break;
		case 'l':
		case 'L':
			{
				int n;
				n=atoi(&line[1]);
				if(n>0){
					printCmd(r[PC],n);
				}
				else
					printCmd(r[PC],1);
			}
			break;
		case 'd':
		case 'D':
			{
				int n,i,j,lnum;
				char *p;
				n=atoi(&line[1]);	
				if(n==0 && line[1]){
					n=MapSymItem::IdAddr(&line[1]);
					if(n<0){
						printf("can't found symbol %s !\n",&line[1]);
						break;
					}
				}
				lnum=8*3;
				for(i=0;i<lnum;i+=8){
					printf("%4d: ",n+i);
					for(j=0;j<8;j++)
						printf("0x%04X ",(int)(ram[n+i+j] & 0x0000ffff));
					for(p=(char*)&ram[n+i],j=0;j<16;j++,p++)
						if(isalnum(*p))
							printf("%c",*p);
						else	printf(".");
					printf("\n");
				}
			}
			break;
		case 'r':
		case 'R':
			{
				int i;
				for(i=0;i<7;i++){
					printf("%3s=%3d 0x%04X\t",wAsm::R_STR[i],r[i],r[i] & 0x0000ffff);
				}
			}
			break;
		case 't':
		case 'T':
			debugMode=1;
			break;
		case 'g':
		case 'G':
			debugMode=3;
			{
				int n;
				n=atoi(&line[1]);
				if(n>0){
					debugMode=2;
					overPc=n;
				}
			}
			break;
		case 'q':
		case 'Q':
			exit(0);
			break;
		}

		if(debugMode>=0)
			break;
	}while(1);
}

void comet::SetFR(INT16 rr)
{
	if(rr>0)	r[FR]=0;
	else if(rr==0)	r[FR]=1;
	else r[FR]=2;
}

int comet::AddrAvailability(INT16 addr)
{
	int ret;

	if(!(addr>=0 && addr<65536)){
		printf("addr error !\n");
		ret=0;
	}
	else	ret=1;
	return ret;
}
