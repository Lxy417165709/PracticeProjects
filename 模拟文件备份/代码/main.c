# include<stdio.h>

int main(){
	FILE* sourceFile;
	// ��Դ�ļ� 
	if ((sourceFile = fopen("sourceFileDirectory/sourceFile.md","r"))==NULL){
		printf("Դ�ļ���ʧ�ܣ�\n");
		return 0;
	}
	printf(":: Դ�ļ��򿪳ɹ���\n");
	
	// ��Ŀ���ļ����������򴴽�
	FILE* targetFile;
	if ((targetFile = fopen("targetFileDirectory/targetFile.md","w"))==NULL){
		printf("Ŀ���ļ���ʧ�ܣ�\n");
		return 0;
	}
	printf(":: Ŀ���ļ��򿪳ɹ���\n");
	
	const int bufferSize =  10;	// ��������С 
	char buffer[bufferSize];	// ���ڴ��п���һ�黺��������Ŷ�ȡ�����ļ����� 
	int readCharCount;			// ��ȡ�����ֽ��� 
	while((readCharCount = fread(buffer, sizeof(char),bufferSize,sourceFile))){
		fwrite(buffer,readCharCount,sizeof(char),targetFile);		
	}
	
	printf(":: ������ɣ�"); 
	
	// �ر��ļ� 
	fclose(sourceFile);
	fclose(targetFile);
} 
