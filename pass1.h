#pragma once
#include "TokenLine.h"

class Pass1
{
public:
	void HandleLable(TokenLine *lt);
	Pass1(void);
	~Pass1(void);
	void handleLine(TokenLine* tl,FILE *fp);
	void handlePGLine(TokenLine* tl,FILE *fp);
	int addr_code;
	int addr_data;
	void init(void);
	void handleCMDLine(TokenLine* tl);
	void recountGVAddr(void);
};
