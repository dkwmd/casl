#include "StdAfx.h"
#include ".\hdargs.h"
#include "myFileName.h"
#include <stdio.h>
#include <string.h>
#include "MapSymItem.h"

hdArgs::hdArgs(void)
{
}

hdArgs::~hdArgs(void)
{
}

void hdArgs::setArgs(int args, char ** argv)
{
	analyse(args,argv);
}

void hdArgs::analyse(int args, char ** argv)
{
	int i;
	HecCodeFileName[0]='\0';
	isdebug=0;
	for(i=1;i<args;i++){
		if(*argv[i]=='-'){
			switch(argv[i][1]){
			case 'd':
				isdebug=1;
				break;
			case '?':
			default:
				print_help();
				is_ok=0;
				return ;
			}
		}
		else { //filename
			myFileName fn;

			fn.SeeNormal(argv[i]);
			fn.makeNewExt(HecCodeFileName,".map");
			MapSymItem::load_map_from_file(HecCodeFileName);
			fn.makeNewExt(HecCodeFileName,".run");
		}
	}
	if(HecCodeFileName[0])
		is_ok=1;
}

void hdArgs::print_help(void)
{
	char *msg[]={
		" CaslVm  [-d] filename",
		"      -d     debug mode",
		"      -?     this message is help you for hec vm!",
		"   filename  .run file",
		""
	};
	int i;
	for(i=0;*msg[i];i++)
		printf("%s\n",msg[i]);
}

int hdArgs::ok(void)
{
	return is_ok;
}
