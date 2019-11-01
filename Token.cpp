#include "StdAfx.h"
#include ".\token.h"
#include "ctype.h"
#include "wAsm.h"

Token::Token(void)
{
	tok=0;
	*token='\0';
}

Token::~Token(void)
{
}

void Token::gettoken(char** src)
{
	char *p;
	p=token;
	while(iswhite(**src))
		(*src)++;
	if(isalpha(**src) || **src=='_'){
		*p = **src;
		p++, ++(*src);
		while(isalnum(**src) || **src=='_'){
			*p = **src;
			p++, ++(*src);
		}
		*p='\0';
		tok=IS_SYMBOL;
	}
	else if(isnum(**src) || **src=='-' && isnum(*(*src+1))){
		*p = **src;
		p++, ++(*src);
		if(**src=='x' || **src=='X'){
			// hex number
			*p = **src;
			p++, ++(*src);
			while(ishexnum(**src)){
				*p = **src;
				p++, ++(*src);
			}
			*p='\0';
			tok=IS_NUMERIC;
			var_t = VAR_INT;
			var.i = aHexToInt(token);
		}
		else {
			while(isnum(**src)){
				*p = **src;
				p++, ++(*src);
			}
			if(**src=='.'){
				*p = **src;
				p++, ++(*src);
				while(isnum(**src)){
					*p = **src;
					p++, ++(*src);
				}
				*p='\0';
				tok=IS_NUMERIC;
				var_t = VAR_DOUBLE;
				var.f = atof(token);
			}
			else {
				*p='\0';
				tok=IS_NUMERIC;
				var_t = VAR_INT;
				var.i = atoi(token);
			}
		}
	}
	else if(isop(**src)){
		*p = **src;
		p++, ++(*src);
		*p='\0';
		tok=IS_OP;	//asm langauge no this item.
	}
	else if(isnote(**src)){
		*p = **src;
		p++, ++(*src);
		*p='\0';
		tok=IS_NOTE;
	}
	else if(**src=='$'){
		int i;
		*p = **src;
		p++, ++(*src);
		for(i=0;i<3 && !iswhite(**src) && !isop(**src);i++){
			*p = **src;
			p++, ++(*src);
		}
		*p='\0';
		tok=IS_REG;
	}
	else if(**src=='\''){
		*p = **src;
		p++, ++(*src);
		var.i = (int)(**src);
		*p = **src;
		p++, ++(*src);
		*p = **src;
		p++, ++(*src);
		*p='\0';
		tok=IS_NUMERIC;
		var_t = VAR_CHAR;
	}
	else if(**src=='\"'){
		int i;
		*p = **src;
		p++, ++(*src);
		for(i=0;i<127 && **src!='\"' && **src;i++){
            var_str[i]= **src;
			++(*src);
		}
		var_str[i]='\0';
		*p = **src;
		p++, ++(*src);
		*p='\0';
		tok=IS_STRING;
		var_t = VAR_CHAR;
	}
	else {
		*p='\0';
		tok=IS_UNKNOW;
	}

	if(tok==IS_SYMBOL){
		if(iskeyword(token)){
			if(asm_cmd<EXIT)
				tok=IS_PSEUDOINSTRUCTION;
			else
				tok=IS_KEYWORD;
		}
	}
}

int Token::iswhite(char ch)
{
	if(ch==' '||ch=='\t' || ch=='\r' || ch=='\n')
		return 1;
	return 0;
}

int Token::isnum(char ch)
{	
	return ch>='0' && ch<='9';
}

int Token::iskeyword(char *t)
{
	asm_cmd=wAsm::Cmd_StrToBin(t);
	return asm_cmd!=-1;
}

int Token::isop(char ch)
{
	if(ch==',')
		return 1;
	return 0;
}

int Token::isnote(char ch)
{
	return ch=='#' || ch==';';
}

// ²ì¿´ÊÇ·ñÊÇ ¼Ä´æÆ÷
int Token::isreg(char* t)
{
	return wAsm::isreg(t);
}

int Token::ishexnum(char ch)
{
	return isnum(ch) || ch>='A' && ch<='F' || ch>='a' && ch<='f';
}

int Token::aHexToInt(char* t)
{
	int i;
	t++, t++;
	i=0;
	while(*t){
		if(isnum(*t))
			i=i*16 + (*t)-'0';
		else if(*t>='A' && *t<='F')
			i=i*16 + (*t)-'A'+10;
		else if(*t>='a' && *t<='f')
			i=i*16 + (*t)-'a'+10;
		t++;
	}
	return i;
}
