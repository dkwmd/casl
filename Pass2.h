#pragma once

#include "TokenLine.h"
#include "comet.h"

class Pass2
{
	void setramcmd(char *rm, INT16 cmd, INT16 GR, INT16 ADR, INT16 XR);
public:
	Pass2(void);
	~Pass2(void);
	void HandleLable(TokenLine *tl);
	void handleLine(TokenLine* tl, FILE* fp);
	int handleCMDLine2(TokenLine* tl, FILE* fp);
	void handleCMDLine(TokenLine* tl, FILE* fp);
	void handlePGLine(TokenLine* tl,FILE *fp);
	void MathIR_gen(Token* t,Token* cmd,FILE *fp);
	int all_error;
	void MathDoubleR_gen(Token* t, Token* cmd, FILE* fp);
	int line_n;
	int MathSymbol(Token *t);
};
