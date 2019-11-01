// hecasm.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SymbolTL.h"
#include "TokenLine.h"
#include "Pass1.h"
#include "Pass2.h"
#include "myFileName.h"

SymbolTL stl;
myFileName fn;

void print_all_symbol(FILE *fp)
{
	int i;
	for(i=0;i<stl.si_num;i++)
		fprintf(fp,"%10s\t%d\n",stl.si[i].ident,stl.si[i].addr);
	fprintf(fp,"all=%d\n",i);
}

void gen_mapfile(char *srcfile)
{
	char filename[64];
	FILE *fp;

	fn.SeeNormal(srcfile);
	fn.makeNewExt(filename,".map");
	fp=fopen(filename,"wt+");
	if(!fp)
		return ;
	print_all_symbol(fp);
	fclose(fp);
}

void take_all_symbol(char *srcfile,char *tempfile)
{
	TokenLine	tl;
	FILE *fp_r,*fp_w;
	char line[64];
	Pass1 pass1;

	fp_r=fopen(srcfile,"rt");
	if(!fp_r)
		return ;
	fp_w=fopen(tempfile,"wb+");
	if(!fp_w){
		fclose(fp_r);
		return ;
	}

	while(!feof(fp_r)){
		if(fgets(line,63,fp_r)){
			tl.analyse_line(line);
			pass1.handleLine(&tl,fp_w);
		}
	}
	fclose(fp_r);
	fclose(fp_w);
	pass1.recountGVAddr();
}

void asm_complier(char *srcfile,char *descfile)
{
	TokenLine	tl;
	FILE *fp_r,*fp_wb;
	char line[64];
	Pass2 pass2;

	fp_r=fopen(srcfile,"rt");
	if(!fp_r)
		return ;
	fp_wb=fopen(descfile,"wb+");
	if(!fp_wb){
		fclose(fp_r);
		return ;
	}

	while(!feof(fp_r)){
		if(fgets(line,63,fp_r)){
			tl.analyse_line(line);
			pass2.line_n++;
			pass2.handleLine(&tl,fp_wb);
		}
	}
	fclose(fp_r);

	//append global value
	fp_r=fopen("temp.tmp","rb");
	if(fp_r){
		while(!feof(fp_r)){
			fputc(fgetc(fp_r),fp_wb);
		}
	}

	fclose(fp_wb);

	if(pass2.all_error>0)
		printf("\t %d error(s) \n",pass2.all_error);
}

void asm_loop(char *srcfile,char *descfile)
{
	take_all_symbol(srcfile,"temp.tmp");
	print_all_symbol(stdout);
	gen_mapfile(srcfile);
	asm_complier(srcfile,descfile);
}

static void printCmdArg()
{
	char *help[]={
		"Command  asm_file [run_file]\n",
		"    asm_file    the HecVM  .casl \n",
		"    run_file    output file, the file can be run on CaslVM. \n",
		"Example:  \n",
		"    casl    test.casl  test.run\n",
	};

	int i,s;
	s=sizeof(help)/sizeof(help[0]);
	for(i=0;i<s;i++){
		printf("%s",help[i]);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc==3)
		asm_loop(argv[1] , argv[2]);
	else if(argc==2){
		char outfile[64];
		fn.SeeNormal(argv[1]);
		fn.makeNewExt(outfile,".run");
		asm_loop(argv[1] , outfile);
	}
	else {
		printCmdArg();
		printf("\n");
	}
	return 0;
}
