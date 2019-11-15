# include<stdio.h> 
# include<string>
# include<iostream>
using namespace std;


class PCM{

	private: int allBits; // 总共的bits
	private: int maxValue;// 最大量化值 
	private: int dlmBits; // 段落码bits
	private: int dnmBits; // 段内码bits 
	private: int* dlArr;	 // 段落数组 [0,16,32,64...] 存放段落其实电平
	private: int* dnArr;	 // 段内数组 [1,1,2,4...]	 存放段内量化间隔 
	private: int size;	 // 长度 (固定为8)
	private: bool runningFlag;	// false不正常 true正常 
	
	// 判断是否正常运行 
	public: bool runningJudge(){
		return runningFlag;
	} 
	
	public: PCM(){
		runningFlag = false;
	}
	public: PCM(int allBits,int dnmBits){
		
		if (dnmBits>=allBits || allBits<=1 || dnmBits<=0 || allBits>=32){
			runningFlag = false;
			return ;
		}
		
		runningFlag = true;
		// 注意异常处理 
		this->allBits = allBits;
		this->dnmBits = dnmBits;// 注意1位符号位 
		this->dlmBits = allBits - this->dnmBits - 1;
		
		this->size = 8;
		
		this->dlArr = new int[size];
		this->dnArr = new int[size];
		
		
		this->maxValue = (1<<(allBits+dnmBits-1))-1;	// allBits有bug! 
		initArr();
		
		showSystemInfo();
		
	}
	
	public: void showSystemInfo(){
		
		printf("PCM系统\n");
		printf("	量化范围： [%d, +%d]\n",-maxValue,maxValue);
		printf("	总bit位数： %d\n",allBits);
		printf("	段落bit位数： %d\n",dlmBits);
		printf("	段内bit位数： %d\n",dnmBits);
		
		printDlArr(); 
		printDnArr(); 
		printf("----------------------------------------------------------------------------------------------\n");
	}
	
	
	// 量化
	public: int quanification(int n,string &mazi){
		if (abs(n)>this->maxValue){
			return -1;	// 出错 
		}
		int flag=1;	// 符号位 
		if (n<0){
			flag = 0;
			n = -n;
		}
		
		int dlNumber = binarySearch(n);	// 对应的起始电平位置 
		int dnNumber =  (n-this->dlArr[dlNumber])/this->dnArr[dlNumber];	// 对应的段内号 
		int ans = this->dlArr[dlNumber] + dnNumber*dnArr[dlNumber];	//  
		ans = (flag==0?-ans:ans);
		
		mazi=toBinary(flag,1) + " " + toBinary(dlNumber,dlmBits) + " " + toBinary(dnNumber,dnmBits);
		return ans;
	}
	

	
	
	
	
	// 初始化量化天平数组和量化间隔数组 
	private: void initArr(){
		dlArr[0] = 0;
		dlArr[1] = 1<<(allBits+dnmBits-8);
		for (int i=2;i<size;i++){
			dlArr[i] = dlArr[i-1]*2;
		}
		
		dnArr[0] = 1;
		dnArr[1] = dlArr[1]/(1<<dnmBits);
		for (int i=2;i<size;i++){
			dnArr[i] = dnArr[i-1]*2;
		}
		
	}
	// 输出量化天平数组 
	private: void printDlArr(){
		printf("	段落量化天平： [");
		for (int i=0;i<size;i++){
			if (i!=size-1){
				printf("%d,\t",dlArr[i]);
			}else{
				printf("%d]\n",dlArr[i]);
			}
		}
	}
	// 输出量化间隔数组 
	private: void printDnArr(){
		printf("	段内量化间隔： [");
		for (int i=0;i<size;i++){
			if (i!=size-1){
				printf("%d,\t",dnArr[i]);
			}else{
				printf("%d]\n",dnArr[i]);
			}
		}
	}
	
	// 10进制转二进制字符串 ,bits是位数 
	private: string toBinary(int target,int bits){
		string ans = "";
		for(int i=0;i<bits;i++){
		
			ans = char(target%2+'0') + ans;
			target /=2;
		}
		return ans;
	}
		
	// 返回：在dlArr中找到最后一个小于等于target的数的下标 
	// 规定target要大于等于0，否则会返回-1 
	private: int binarySearch(int target){
		int l=0;
		int r=size-1;
		while(l<=r){
			int mid = (l+r)/2;
			if (dlArr[mid]==target){
				l = mid + 1;
			}else{
				if(dlArr[mid]>target){
					r = mid - 1;
				}else{
					l = mid + 1;
				}
			}
		}
		return r; 
	}

	private: int abs(int a){
		if (a>0){
			return a;
		}
		return -a;
	}

};

int main(){
	PCM system;
	
	do{
		int allBits,dnmBits;
		printf("PCM编码系统的总bit位数以及段内码bit位数(空格分开，回车确定)：");
		scanf("%d %d",&allBits,&dnmBits);
		system = PCM(allBits,dnmBits);
		
		if (system.runningJudge()==false){
			printf("系统创建失败，请重新输入系统设置!\n");
		}
		
	}while (system.runningJudge()==false);
	
	while(true){
		
		int n;
		printf("请输入你要量化的电平：");
		scanf("%d",&n);
		string mazi;
		int ans = system.quanification(n,mazi);
		if (ans == -1){
			printf("	你输入的电平有误！\n"); 
		}else{
			printf("	[ 电平(%d) ]被量化为 [ 电平(%d) ], ",n,ans);
			cout<<"码字为: "<<mazi<<"!"<<endl;
		}
	}
} 
