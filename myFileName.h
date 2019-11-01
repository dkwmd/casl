// myFileName.h: interface for the myFileName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYFILENAME_H__DA816362_19AE_4386_B628_3FC1ACD4C4ED__INCLUDED_)
#define AFX_MYFILENAME_H__DA816362_19AE_4386_B628_3FC1ACD4C4ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class myFileName  
{
public:
	void cutPathStr(char *pathfn,char *fn,char *path);
	void makeNewExt(char *dest,char *ext);
	void SeeNormal(char *fname);
	int HasDotExt(char *fname);
	myFileName();
	virtual ~myFileName();

	char mainfn[99];
	char extfn[20];
	char nonpathfn[64];
	char path[64];
};

#endif // !defined(AFX_MYFILENAME_H__DA816362_19AE_4386_B628_3FC1ACD4C4ED__INCLUDED_)
