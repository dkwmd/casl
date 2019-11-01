#pragma once

#define	II_MAX_DB	256

struct SymbolItem {
	char	ident[64];
	unsigned int	addr;
	char	fun_or_gvar;
	int		array_num;	//��ά����Ļ�����������һά���顣
};

class SymbolTL
{
public:
	SymbolTL(void);
	~SymbolTL(void);

	SymbolItem	si[II_MAX_DB];
	int si_num;	
	void init(void);
	void addOne(SymbolItem * item);
	int findSym(char* name);
	SymbolItem* getSym(char* s);
};
