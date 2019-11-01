// MapSymItem.cpp: implementation of the MapSymItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MapSymItem.h"

MapSymItem *MapSymItem::first=0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MapSymItem::MapSymItem()
{
	next=NULL;
}

MapSymItem::~MapSymItem()
{

}

void MapSymItem::AddNew(char *name, int addr)
{
	MapSymItem *tmp;
	tmp= new MapSymItem();
	strcpy(tmp->id_name, name);
	tmp->addr = addr;
	tmp->next = first;
	first = tmp;
}

void MapSymItem::load_map_from_file(char *fn)
{
	FILE *fp;
	char buf[128];
	int  ok,addr;

	fp=fopen(fn,"r");
	if(!fp)
		return ;

	while(!feof(fp)){
		buf[0]=0;
		ok=fscanf(fp,"%s %d",buf,&addr);
		if(ok==2){
			MapSymItem::AddNew(buf,addr);
		}
	}

	fclose(fp);
}

int MapSymItem::IdAddr(char *name)
{
	MapSymItem *t;
	for(t=first;t;t=t->next)
		if(!strcmp(name,t->id_name))
			return t->addr;
	return -1;
}
