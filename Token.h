#pragma once

enum {
	IS_UNKNOW=0,
	IS_KEYWORD,
	IS_SYMBOL,
	IS_NUMERIC,
	IS_STRING,		//字符串常量,被双引号包围的
	IS_NOTE,
	IS_OP,
	IS_REG,
	IS_PSEUDOINSTRUCTION,	//汇编、伪指令

	VAR_CHAR,
	VAR_INT,
	VAR_DOUBLE,
};

class Token
{
public:
	Token(void);
	~Token(void);
	void gettoken(char** src);
	static int iswhite(char ch);

	char token[64];
	int  tok;
	int  var_t;
	union {
		int		i;
		double	f;
	}var;
	char	var_str[128];	//注意：汇编语言的字符串不是\0结尾的。
	int  asm_cmd;

	static int isnum(char ch);
	int iskeyword(char *t);
	static int isop(char ch);
	static int isnote(char ch);
	// 察看是否是 寄存器
	int isreg(char* t);
	int ishexnum(char ch);
	int aHexToInt(char* t);
};
