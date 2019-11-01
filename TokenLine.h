#pragma once
#include "token.h"

class TokenLine
{
public:
	void FormatLine();
	TokenLine(void);
	~TokenLine(void);
	int analyse_line(char* line);
	void handlePLine(char* line);
	void handleVLine(char* line);

	Token line_item[5];
	char line_num;
};
