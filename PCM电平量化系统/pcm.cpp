# include<stdio.h> 
# include<string>
# include<iostream>
using namespace std;


class PCM{

	private: int allBits; // �ܹ���bits
	private: int maxValue;// �������ֵ 
	private: int dlmBits; // ������bits
	private: int dnmBits; // ������bits 
	private: int* dlArr;	 // �������� [0,16,32,64...] ��Ŷ�����ʵ��ƽ
	private: int* dnArr;	 // �������� [1,1,2,4...]	 ��Ŷ���������� 
	private: int size;	 // ���� (�̶�Ϊ8)
	private: bool runningFlag;	// false������ true���� 
	
	// �ж��Ƿ��������� 
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
		// ע���쳣���� 
		this->allBits = allBits;
		this->dnmBits = dnmBits;// ע��1λ����λ 
		this->dlmBits = allBits - this->dnmBits - 1;
		
		this->size = 8;
		
		this->dlArr = new int[size];
		this->dnArr = new int[size];
		
		
		this->maxValue = (1<<(allBits+dnmBits-1))-1;	// allBits��bug! 
		initArr();
		
		showSystemInfo();
		
	}
	
	public: void showSystemInfo(){
		
		printf("PCMϵͳ\n");
		printf("	������Χ�� [%d, +%d]\n",-maxValue,maxValue);
		printf("	��bitλ���� %d\n",allBits);
		printf("	����bitλ���� %d\n",dlmBits);
		printf("	����bitλ���� %d\n",dnmBits);
		
		printDlArr(); 
		printDnArr(); 
		printf("----------------------------------------------------------------------------------------------\n");
	}
	
	
	// ����
	public: int quanification(int n,string &mazi){
		if (abs(n)>this->maxValue){
			return -1;	// ���� 
		}
		int flag=1;	// ����λ 
		if (n<0){
			flag = 0;
			n = -n;
		}
		
		int dlNumber = binarySearch(n);	// ��Ӧ����ʼ��ƽλ�� 
		int dnNumber =  (n-this->dlArr[dlNumber])/this->dnArr[dlNumber];	// ��Ӧ�Ķ��ں� 
		int ans = this->dlArr[dlNumber] + dnNumber*dnArr[dlNumber];	//  
		ans = (flag==0?-ans:ans);
		
		mazi=toBinary(flag,1) + " " + toBinary(dlNumber,dlmBits) + " " + toBinary(dnNumber,dnmBits);
		return ans;
	}
	

	
	
	
	
	// ��ʼ��������ƽ���������������� 
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
	// ���������ƽ���� 
	private: void printDlArr(){
		printf("	����������ƽ�� [");
		for (int i=0;i<size;i++){
			if (i!=size-1){
				printf("%d,\t",dlArr[i]);
			}else{
				printf("%d]\n",dlArr[i]);
			}
		}
	}
	// �������������� 
	private: void printDnArr(){
		printf("	������������� [");
		for (int i=0;i<size;i++){
			if (i!=size-1){
				printf("%d,\t",dnArr[i]);
			}else{
				printf("%d]\n",dnArr[i]);
			}
		}
	}
	
	// 10����ת�������ַ��� ,bits��λ�� 
	private: string toBinary(int target,int bits){
		string ans = "";
		for(int i=0;i<bits;i++){
		
			ans = char(target%2+'0') + ans;
			target /=2;
		}
		return ans;
	}
		
	// ���أ���dlArr���ҵ����һ��С�ڵ���target�������±� 
	// �涨targetҪ���ڵ���0������᷵��-1 
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
		printf("PCM����ϵͳ����bitλ���Լ�������bitλ��(�ո�ֿ����س�ȷ��)��");
		scanf("%d %d",&allBits,&dnmBits);
		system = PCM(allBits,dnmBits);
		
		if (system.runningJudge()==false){
			printf("ϵͳ����ʧ�ܣ�����������ϵͳ����!\n");
		}
		
	}while (system.runningJudge()==false);
	
	while(true){
		
		int n;
		printf("��������Ҫ�����ĵ�ƽ��");
		scanf("%d",&n);
		string mazi;
		int ans = system.quanification(n,mazi);
		if (ans == -1){
			printf("	������ĵ�ƽ����\n"); 
		}else{
			printf("	[ ��ƽ(%d) ]������Ϊ [ ��ƽ(%d) ], ",n,ans);
			cout<<"����Ϊ: "<<mazi<<"!"<<endl;
		}
	}
} 
