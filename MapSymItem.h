// MapSymItem.h: interface for the MapSymItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPSYMITEM_H__946C3274_BC5A_442C_8CD8_BF375B2ED817__INCLUDED_)
#define AFX_MAPSYMITEM_H__946C3274_BC5A_442C_8CD8_BF375B2ED817__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MapSymItem  
{
public:
	static int IdAddr(char *name);
	static void AddNew(char *name,int addr);
	MapSymItem();
	virtual ~MapSymItem();

	char  id_name[64];
	int		addr;
	MapSymItem *next;

	static MapSymItem *first;
	static void load_map_from_file(char *fn);
};

#endif // !defined(AFX_MAPSYMITEM_H__946C3274_BC5A_442C_8CD8_BF375B2ED817__INCLUDED_)
