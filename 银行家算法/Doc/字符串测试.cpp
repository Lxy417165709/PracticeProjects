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
	总结
	1. 上面的函数实现了两个char*的合并 
*/
