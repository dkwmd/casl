// myFileName.cpp: implementation of the myFileName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "myFileName.h"
#include "stdlib.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

myFileName::myFileName()
{

}

myFileName::~myFileName()
{

}

int myFileName::HasDotExt(char *fname)
{
	int i,s;
	s=strlen(fname);
	for(i=s-1;i>=0;i--){
		if(fname[i]=='\\')
			break;
		if(fname[i]=='.')
			return i;
	}
	return 0;
}

void myFileName::SeeNormal(char *fname)
{
	int i,s;
	
	s=HasDotExt(fname);
	if(!s){
		strcpy(mainfn,fname);
		extfn[0]='\0';
		return ;
	}
	for(i=0;i<s;i++)
		mainfn[i]=fname[i];
	mainfn[i]='\0';
	for(s=0;fname[i];i++)
		extfn[s++]=fname[i];
	extfn[s]='\0';

	cutPathStr(mainfn,nonpathfn,path);
}

void myFileName::makeNewExt(char *dest, char *ext)
{
	strcpy(dest,mainfn);
	strcat(dest,ext);
}

void myFileName::cutPathStr(char *pathfn, char *fn,char *path)
{
	char *last,*p;

	p=pathfn;
	last=p;
	do{
		if(*p=='\\')
			last=p+1;
	}while(*p++);

	for(p=last;*p;)
		*fn++ = *p++;
	*fn='\0';

	for(p=pathfn;*p && p<last;p++)
		*path++ =  *p;
	*path='\0';
}
