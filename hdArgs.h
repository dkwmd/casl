#pragma once

class hdArgs
{
public:
	hdArgs(void);
	~hdArgs(void);
	void setArgs(int args, char ** argv);
	void analyse(int args, char ** argv);
	void print_help(void);

	char HecCodeFileName[128];
	int  is_ok;
	int	 isdebug;
	int ok(void);
};
