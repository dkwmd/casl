#include "StdAfx.h"
#include ".\tokenline.h"

TokenLine::TokenLine(void)
{
}

TokenLine::~TokenLine(void)
{
}

int TokenLine::analyse_line(char* line)
{
	while(Token::iswhite(*line))
		line++;
	handleVLine(line);
	FormatLine();
	return 0;
}

void TokenLine::handleVLine(char *line)
{
	Token token;

	line_num=0;
	token.gettoken(&line);
	while(token.tok && line_num<5){
		if(token.token[0]!=','){
			line_item[line_num]=token;
			token.gettoken(&line);
			line_num++;
		}
		else 
			token.gettoken(&line);
	}
}

void TokenLine::FormatLine()
{
	//注意标号问题
}
