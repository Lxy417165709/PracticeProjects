# include<cstring>
# include<cstdio>
# include<iostream>
using namespace std;

char* mergeStrings(char* prefix,char* content){
	char* target = new char[strlen(prefix) + strlen(content) + 1];
	strcpy(target, prefix);
	strcat(target,content);

    return target;
}
int main(){
	printf(mergeStrings("123","456"));
}

/*
	�ܽ�
	1. ����ĺ���ʵ��������char*�ĺϲ� 
*/
