// ����ʹ��Ӧ�ó��������ɵ� 
// Ӧ�ó�����Ŀ������Ŀ�ļ���

#include "stdafx.h"
#include "hdArgs.h"
#include "comet.h"

hdArgs ar;
comet vm;

void load_bin_code()
{
	int i;
	unsigned char *RAM;
	unsigned char code[]={
		LEA,		0x0f,	1,0x00,
		LEA,		0x3f,	0,0xf0,
		LEA,		0x03,	0,0x00,
		LEA,		0x13,	0,0x00,
		SRL,		0x3f,	1,0x00,
		SLL,		0x3f,	1,0x00,
		SLA,		0x0f,	1,0x00,
		SRA,		0x1f,	1,0x00,
		HALT,
	};

	RAM=(unsigned char*)vm.ram;
	for(i=0;i<sizeof(code);i++)
		RAM[i]=code[i];
}

void loadFromFile()
{
	unsigned char *RAM;
	FILE *fp;
	fp=fopen(ar.HecCodeFileName,"rb");
	if(!fp)
		return ;
	int i;
	i=0;
	RAM=(unsigned char*)vm.ram;
	while(!feof(fp)){
		RAM[i]=fgetc(fp);
		i++;
	}
	fclose(fp);
}

int _tmain(int args,char *argv[])
{
    // TODO: �������Լ��Ĵ����滻�����ʾ�����롣
	if(args==1){
		ar.print_help();

		printf("�������ô��롣\n");
		load_bin_code();

		vm.init();
		vm.run(0);
	}
	else {
		ar.setArgs(args,argv);		
		if(ar.ok()){
			vm.init();
			loadFromFile();
			vm.run(ar.isdebug);
		}
	}
	return 0;
}

