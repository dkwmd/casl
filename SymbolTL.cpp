#include "StdAfx.h"
#include ".\symboltl.h"

SymbolTL::SymbolTL(void)
{
	init();
}

SymbolTL::~SymbolTL(void)
{
}

void SymbolTL::init(void)
{
	si_num = 0;
}

void SymbolTL::addOne(SymbolItem * item)
{
	si[si_num++]= *item;
}

int SymbolTL::findSym(char* name)
{
	int i;
	for(i=0;i<si_num;i++)
		if(!strcmp(name,si[i].ident))
			return i;
	return -1;
}

SymbolItem* SymbolTL::getSym(char* s)
{
	int i;
	i=findSym(s);
	if(i>=0)
		return &si[i];
	return NULL;
}
