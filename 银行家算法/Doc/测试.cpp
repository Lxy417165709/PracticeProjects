# include<cstdio>

using namespace std;

int* test(){
	int* x = new int[5];
	x[0] = 1;
	x[1] = 2;
	x[2] = 3;
	x[3] = 4;
	x[4] = 5;
	return x;
}


int main(){
	
	int* x = test();
	printf("%d\n",x[0]);
	printf("%d\n",x[1]);
	printf("%d\n",x[2]);
	printf("%d\n",x[3]);
	printf("%d\n",x[4]);
	
	// 1
	// 2
	// 3
	// 4
	// 5
}

/*
	总结
	1. 函数返回的指针可以访问！ 

*/
