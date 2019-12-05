# include<stdio.h>

int main(){
	FILE* sourceFile;
	// 打开源文件 
	if ((sourceFile = fopen("sourceFileDirectory/sourceFile.md","r"))==NULL){
		printf("源文件打开失败！\n");
		return 0;
	}
	printf(":: 源文件打开成功！\n");
	
	// 打开目标文件，不存在则创建
	FILE* targetFile;
	if ((targetFile = fopen("targetFileDirectory/targetFile.md","w"))==NULL){
		printf("目标文件打开失败！\n");
		return 0;
	}
	printf(":: 目标文件打开成功！\n");
	
	const int bufferSize =  10;	// 缓冲区大小 
	char buffer[bufferSize];	// 在内存中开辟一块缓冲区，存放读取到的文件数据 
	int readCharCount;			// 读取到的字节数 
	while((readCharCount = fread(buffer, sizeof(char),bufferSize,sourceFile))){
		fwrite(buffer,readCharCount,sizeof(char),targetFile);		
	}
	
	printf(":: 备份完成！"); 
	
	// 关闭文件 
	fclose(sourceFile);
	fclose(targetFile);
} 
