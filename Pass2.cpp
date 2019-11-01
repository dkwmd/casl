#include "StdAfx.h"
#include "pass2.h"
#include "wAsm.h"
#include "SymbolTL.h"

extern SymbolTL stl;

Pass2::Pass2(void)
: all_error(0)
, line_n(0)
{
}

Pass2::~Pass2(void)
{
}

void Pass2::HandleLable(TokenLine *tl)
{
	int i;
	if(tl->line_item[0].tok==IS_SYMBOL){
		//肯定是标号
	}
	else {
		//添加标号记号
		for(i=tl->line_num-1;i>=0;i--)
			tl->line_item[i+1]=tl->line_item[i];

		tl->line_item[0].tok=IS_SYMBOL;
		tl->line_item[0].token[0]='\0';		

		tl->line_num++;
	}
}

void Pass2::handleLine(TokenLine* tl, FILE* fp)
{
		if((*tl).line_num>0){
			HandleLable(tl);
			if((*tl).line_item[1].tok==IS_PSEUDOINSTRUCTION)
				handlePGLine(tl,fp);
			else
				handleCMDLine(tl,fp);
		}
}

void Pass2::setramcmd(char *rm, INT16 cmd, INT16 GR, INT16 ADR, INT16 XR)
{
	rm[0]=cmd;
	rm[1]= GR<<4 & XR;
	*((INT16*)(rm+2))=ADR;
}

int Pass2::handleCMDLine2(TokenLine* tl, FILE* fp)
{
	unsigned char	cmd;
	int		xr;
	short	ADDR;
	unsigned char GR,XR;

	cmd= tl->line_item[1].asm_cmd;
	switch(cmd){
	case JMP:
	case JPZ:
	case JMI:
	case JNZ:
	case JZE:
	case PUSH:
	case CALL:
		GR=0x0f;
		if(tl->line_num >= 4){
			XR = xr = wAsm::getXRegNum(tl->line_item[3].token,1);

			if(xr<0){
				printf("unknow register '%s' at line %d\n"
					,tl->line_item[3].token
					,line_n);
				return 0;
			}
		}
		else
			XR=0x0f;

		if(tl->line_item[2].tok==IS_SYMBOL){
			ADDR = MathSymbol(&tl->line_item[2]);
		}
		else if(tl->line_item[2].tok==IS_NUMERIC)
			ADDR = tl->line_item[2].var.i;
		else
			printf("syntax error at line %d.\n",line_n);

		fwrite(&cmd,sizeof(cmd),1,fp);
		XR = GR<<4 | XR;
		fwrite(&XR,sizeof(XR),1,fp);
		fwrite(&ADDR,sizeof(ADDR),1,fp);
		return 1;
	}
	return 0;
}

void Pass2::handleCMDLine(TokenLine* tl, FILE* fp)
{
	unsigned char	cmd;
	int		reg,gr,xr;
	short	ADDR;
	unsigned char GR,XR;

	if(tl->line_item[1].tok!=IS_KEYWORD){
		if(tl->line_item[1].tok!=IS_NOTE)
			printf("error '%s' at line %d.\n"
				,tl->line_item[1].token
				,line_n);
		return ;
	}
	cmd= tl->line_item[1].asm_cmd;

	if(handleCMDLine2(tl,fp))
		return ;

	XR=GR=0x0f;
	ADDR=0;
	
	if(tl->line_num>2){
		reg = wAsm::isreg(tl->line_item[2].token);
		GR = gr = wAsm::getXRegNum(tl->line_item[2].token,reg);
		if(gr<0){
			printf("unknow register '%s' at line %d\n"
				,tl->line_item[2].token
				,line_n);
			return ;
		}

		if(tl->line_num>3){
			if(tl->line_item[3].tok==IS_SYMBOL){
				ADDR = MathSymbol(&tl->line_item[3]);
			}
			else if(tl->line_item[3].tok==IS_NUMERIC)
				ADDR = tl->line_item[3].var.i;
			else
				printf("syntax error at line %d.\n",line_n);
		}

		if(tl->line_num >= 5){
			XR = xr = wAsm::getXRegNum(tl->line_item[4].token,reg);

			if(xr<0){
				printf("unknow register '%s' at line %d\n"
					,tl->line_item[4].token
					,line_n);
				return ;
			}
		}
	}

	fwrite(&cmd,sizeof(cmd),1,fp);
	XR = GR<<4 | XR;
	fwrite(&XR,sizeof(XR),1,fp);
	fwrite(&ADDR,sizeof(ADDR),1,fp);

	if(cmd==HALT){
		//halt
		fwrite(&cmd,1,1,fp);
	}
}

void Pass2::handlePGLine(TokenLine* tl,FILE *fp)
{
	unsigned char	cmd;
	short	ADDR;

	cmd= tl->line_item[1].asm_cmd;

	switch(cmd){
	case IN://由于是宏指令，所以实际上是函数调用，并且使用堆栈
			ADDR = stl.findSym(tl->line_item[2].token);
			ADDR = stl.si[ADDR].addr;			
			fwrite(&cmd,sizeof(cmd),1,fp);
			fwrite(&cmd,sizeof(cmd),1,fp);
			fwrite(&ADDR,sizeof(ADDR),1,fp);
			ADDR = stl.findSym(tl->line_item[3].token);
			ADDR = stl.si[ADDR].addr;			
			fwrite(&ADDR,sizeof(ADDR),1,fp);
		return ;
	case OUT://由于是宏指令，所以实际上是函数调用，并且使用堆栈
			ADDR = stl.findSym(tl->line_item[2].token);
			ADDR = stl.si[ADDR].addr;			
			fwrite(&cmd,sizeof(cmd),1,fp);
			fwrite(&cmd,sizeof(cmd),1,fp);
			fwrite(&ADDR,sizeof(ADDR),1,fp);
			ADDR = stl.findSym(tl->line_item[3].token);
			ADDR = stl.si[ADDR].addr;			
			fwrite(&ADDR,sizeof(ADDR),1,fp);
		return ;
	}
}

void Pass2::MathIR_gen(Token* t,Token* cmd,FILE *fp)
{
	char B;

	if(wAsm::isreg(t->token)){
		B = wAsm::getXRegNum(t->token,1);
		fwrite(&B,sizeof(B),1,fp);
	}
	else {
		B = -1;
		fwrite(&B,sizeof(B),1,fp);
		printf("command %s args %s error, match a Register. \n",
			cmd->token,t->token);
		all_error++;
	}
}

void Pass2::MathDoubleR_gen(Token* t, Token* cmd, FILE* fp)
{
	char B;

	if(wAsm::isreg(t->token)){
		B = wAsm::getXRegNum(t->token,3);
		fwrite(&B,sizeof(B),1,fp);
	}
	else {
		B = -1;
		fwrite(&B,sizeof(B),1,fp);
		printf("Instruction %s params %s error, match a Register. \n",
			cmd->token,t->token);
		all_error++;
	}
}

int Pass2::MathSymbol(Token *t)
{
	int ADDR;
	ADDR=0;
	if(t->tok==IS_SYMBOL){
		ADDR = stl.findSym(t->token);
		if(ADDR>=0){
			ADDR = stl.si[ADDR].addr;
		}
		else{
			printf("syntax error : not found symbol %s at line %d\n",
				t->token,line_n);
			all_error++;
		}
	}
	return ADDR;
}
