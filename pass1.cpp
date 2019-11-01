#include "StdAfx.h"
#include "pass1.h"
#include "pass2.h"
#include "SymbolTL.h"
#include "wAsm.h"
#include "comet.h"

extern SymbolTL stl;

/*
	casl 汇编语言规范：
	标号：	指令	操作数，。。。
	[空格]	指令	。。。
	空格	end
	标号	dc		。。。
	标号	ds		。。。
	由此可以得出结论，没有标号的语句必须空出一个字符在行前。
*/

Pass1::Pass1(void)
: addr_code(0)
{
	init();
}

Pass1::~Pass1(void)
{
}

void Pass1::HandleLable(TokenLine *tl)
{
	SymbolItem item;
	int i;

	if(tl->line_item[0].tok==IS_SYMBOL){
		if(tl->line_item[1].tok!=IS_PSEUDOINSTRUCTION){
			//肯定是标号
			item.addr = addr_code;
			item.fun_or_gvar = 2;
			strcpy(item.ident,tl->line_item[0].token);
			item.array_num = 0;	//标号
			stl.addOne(&item);
		}
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

void Pass1::handleLine(TokenLine* tl,FILE *fp)
{
	if((*tl).line_num>0){
		HandleLable(tl);
		if((*tl).line_item[1].tok==IS_NOTE)
			return ;
		if((*tl).line_item[1].tok==IS_PSEUDOINSTRUCTION)
			handlePGLine(tl,fp);
		else
			handleCMDLine(tl);
	}
}

int MathSymbol(Token *t)
{
	int ADDR;
	ADDR=0;
	if(t->tok==IS_SYMBOL){
		ADDR = stl.findSym(t->token);
		if(ADDR>=0){
			ADDR = stl.si[ADDR].addr;
		}
		else{
			printf("syntax error : not found symbol %s \n",
				t->token);
		}
	}
	return ADDR;
}

void Pass1::handlePGLine(TokenLine* tl,FILE *fp)
{
	SymbolItem item;
	INT16 W;
	int i;

	switch(tl->line_item[1].asm_cmd){
	case DC:
		//肯定是标号
		if(tl->line_item[0].token[0]){
			item.addr = addr_data;
			item.fun_or_gvar = 0;
			strcpy(item.ident,tl->line_item[0].token);
			item.array_num = 0;	//标号
			stl.addOne(&item);
		}
		if(tl->line_item[2].tok==IS_SYMBOL)
			W=MathSymbol(&tl->line_item[2]);
		else
			W=(INT16)tl->line_item[2].var.i;
		fwrite(&W,sizeof(INT16),1,fp); 
		addr_data++;
		break;
	case DS:
		//肯定是标号
		if(tl->line_item[0].token[0]){
			item.addr = addr_data;
			item.fun_or_gvar = 0;
			strcpy(item.ident,tl->line_item[0].token);
			item.array_num = tl->line_item[2].var.i;	//标号
			stl.addOne(&item);
		}
		W='?';
		for(i=0;i<tl->line_item[2].var.i;i++)
			fwrite(&W,sizeof(INT16),1,fp); 
		addr_data+=tl->line_item[2].var.i;
		break;
	case START:
		break;
	case END:
		break;
	case IN:
	case OUT:
		addr_code+=wAsm::InstructionLen(tl->line_item[1].asm_cmd);
		break;
	}
}

void Pass1::init(void)
{
	addr_code=0;
	addr_data=0;
	stl.init();
}

void Pass1::handleCMDLine(TokenLine* tl)
{
	int cmd;
	cmd= tl->line_item[0].asm_cmd;
	addr_code+= wAsm::InstructionLen(cmd);
}

void Pass1::recountGVAddr(void)
{
	int i;
	for(i=0;i<stl.si_num;i++){
		if(stl.si[i].fun_or_gvar==0)
			stl.si[i].addr += addr_code;		
	}
}
