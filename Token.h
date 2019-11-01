#pragma once

enum {
	IS_UNKNOW=0,
	IS_KEYWORD,
	IS_SYMBOL,
	IS_NUMERIC,
	IS_STRING,		//�ַ�������,��˫���Ű�Χ��
	IS_NOTE,
	IS_OP,
	IS_REG,
	IS_PSEUDOINSTRUCTION,	//��ࡢαָ��

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
	char	var_str[128];	//ע�⣺������Ե��ַ�������\0��β�ġ�
	int  asm_cmd;

	static int isnum(char ch);
	int iskeyword(char *t);
	static int isop(char ch);
	static int isnote(char ch);
	// �쿴�Ƿ��� �Ĵ���
	int isreg(char* t);
	int ishexnum(char ch);
	int aHexToInt(char* t);
};
