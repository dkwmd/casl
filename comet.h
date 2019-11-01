// comet.h: interface for the comet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMET_H__8FB6D59B_F72A_4AD5_910E_BD7F0E55FD3F__INCLUDED_)
#define AFX_COMET_H__8FB6D59B_F72A_4AD5_910E_BD7F0E55FD3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define INT16	short int
#define UINT16	unsigned short int

#include "wAsm.h"

class comet  
{
public:
	int AddrAvailability(INT16 addr);
	void SetFR(INT16 rr);
	void printCmd(int start_pc,int num);
	void run(int runMode);
	INT16 runOne();
	void init();
	comet();
	virtual ~comet();

	INT16  r[7];	//GR0 ~ GR4
					//PC and FR

	INT16	ram[65546];
protected:
	void debugCmd();
	int  printCmd2(int pc);
	void getramcmd(char *rm,INT16 *cmd,INT16 *GR,INT16 *ADR,INT16 *XR);
	int		debugMode;

	int		overPc;
};

#endif // !defined(AFX_COMET_H__8FB6D59B_F72A_4AD5_910E_BD7F0E55FD3F__INCLUDED_)
