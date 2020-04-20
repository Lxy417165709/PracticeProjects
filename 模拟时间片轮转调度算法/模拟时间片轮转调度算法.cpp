# include<stdio.h>
# include<string>
# include<cstring>
# include<iostream>
# include<stdlib.h>
# include<windows.h>
# include <ctime>
using namespace std;

enum stateType {WAITING,RUNNING,FINISHED};	// 进程状态类型

// 整数转字符串 
string toString(int a){
	string s = "";
	do{
		s = char(a%10+'0') + s; 
		a/=10;
	}while(a!=0);
	return s;
}

// 产生[l, r]的随机数(失败返回-1) 
int getRandom(int l,int r){
	if(l>r)return -1;
	return rand()%(r-l+1)+l;
	
} 

// PCB结构 
struct PCB{
public:
	string name;			// 进程名 
	int timeOfComming;		// 到达时间 
	int timeOfExecuting;	// 运行时间
	char state;		// 进程状态
	PCB* next;				// 链接指针 
	
public:
	PCB(string name,int timeOfComming,int timeOfExecuting,char state,PCB* next){
		this->name = name;
		this->timeOfComming = timeOfComming;
		this->timeOfExecuting = timeOfExecuting;
		this->state = state;
		this->next = next;
	}
	PCB(const PCB &pcb){
		this->name = pcb.name;
		this->timeOfComming = pcb.timeOfComming;
		this->timeOfExecuting = pcb.timeOfExecuting;
		this->state = pcb.state;
		this->next = pcb.next;
	}
	
	void show(){
		printf("\t\t------------------\n");
		printf("\t\t| 进程名: %s\n",name.data());
		printf("\t\t| 到达时间: %d\n",timeOfComming);
		printf("\t\t| 剩余运行时间: %d\n",timeOfExecuting);
		printf("\t\t| 进程状态: %c\n",state);
		printf("\t\t------------------\n");
		
	}
	
	void finish(){
		this->state = 'C';
	}
	void run(){
		this->state = 'R';
	}
	void wait(){
		this->state = 'W';
	}
	
};

// 进程PCB链表(单向链表) 
class ListOfPCB{
	PCB* dummyHead;		// 哑头节点 
	PCB* tail;			// 指向位于尾部的进程(初始时指向哑头) 
	int length;			// 链表长度 
public:
	int size(){
		return length;
	}
	PCB* front(){
		return dummyHead->next;
	}
	
	ListOfPCB(){
		this->dummyHead = new PCB("",-1,-1,'W',NULL); 
		this->tail = this->dummyHead;
		this->length = 0;
	}

	// 加入PCB 
	void push(PCB* p){
		PCB* copyP = new PCB(*p);
		this->tail->next = copyP;
		this->tail = this->tail->next; 
		length++;
	}
	
	// 删除PCB
	bool remove(PCB* p){
		if (p==NULL){
			return false;
		} 
		PCB* cur = dummyHead;
		while(cur!=NULL && cur->next != p){
			cur = cur->next;
		}
		if (cur==NULL){
			return false;
		}
		cur->next = cur->next->next; 
		// 当删除点为尾节点时，尾节点需要调整
		if (p==tail){
			tail = cur;	
		}
		this->length--;
		return true; 
	}
	
	void show(){
		PCB* cur = dummyHead;
		printf("\t\t进程PCB链表长度: %d\n",length);
		while(cur->next!=NULL){
			cur->next->show();
			cur = cur->next;
		}
	}
	
	
};


// 进程调度链表(单向循环链表) 
class LoopListOfPCB{

	PCB* dummyHead;		// 哑头节点 
	PCB* tail;			// 指向位于尾部的进程(初始时指向哑头) 
	int length;			// 链表长度 
public:
	int size(){
		return length;
	}
	PCB* front(){
		return dummyHead->next;
	}
	LoopListOfPCB(){
		this->dummyHead = new PCB("",-1,-1,'W',NULL); 
		this->tail = this->dummyHead;
		this->length = 0;
	}
	
	// 加入PCB 
	void push(PCB* p){
		if(p==NULL){
			return;
		}
		PCB* copyP = new PCB(*p);
		this->tail->next = copyP;
		copyP->next = dummyHead->next;
		this->tail = this->tail->next; 
		length++;
	}
	
	// 删除PCB 
	bool remove(PCB* p){
		if (p==NULL){
			return false;
		}
		
		if (p == dummyHead->next){
			// 表示只有一个元素 
			if (length == 1){
				dummyHead->next = NULL;
				tail = dummyHead;
			}else{
				// 表示不只有一个元素 
				tail->next = p->next;
				dummyHead->next = tail->next;
			}
		}else{
			PCB* cur = dummyHead;
			
			for(int i=0;i<length;i++){
				if (cur->next==p){
					
					break;
				}
				cur = cur->next;
			}		
			// 表示不存在链表中p节点 
			if (cur->next!=p){
				return false;
			} 
			cur->next = cur->next->next;
			
			// 当删除点为尾节点时，尾节点需要调整
			if(p==tail){
				tail = cur;
			}
		}
		
		length--;
		return true;	
	}
	void show(){
		PCB* cur = dummyHead;
		printf("\t\t进程调度链表长度: %d\n",length);
		for(int i=0;i<length;i++){
			cur->next->show();
			cur = cur->next;
		}		
	}
	
	
};


// 系统 (执行调度) 
class System{

	ListOfPCB pcbList;			// 进程链表 
	LoopListOfPCB dispatchList;	// 进程调度链表 
	int currentTime;			// 当前时间
	PCB* runningPCB;
public:
	System(){
		currentTime = 0;
		runningPCB = NULL;
	}
	
	// 添加进程
	void addProcess(PCB* p){
		this->pcbList.push(p);
	}
	
	
	 
	// 把当前能进行调度的进程加入调度队列 
	void lengthenDispatchList(){
	 	PCB* currentPCB = pcbList.front();
	 	while(currentPCB!=NULL){

	 		PCB* next = currentPCB->next;
			if (currentPCB->timeOfComming <= currentTime){

				pcbList.remove(currentPCB);

				if(currentPCB->timeOfExecuting>0) dispatchList.push(currentPCB);

			}
			currentPCB = next;
		}
		
	}
	
	// 开始执行时间片轮转调度 
	void beginDispatching(int sleepTime){
	 	
	 	do{
	 		currentTime++;
	 		lengthenDispatchList();

	 		showTime();
			showListOfDispatching();
			
	 		if (dispatchList.size()!=0){
	 			if (runningPCB == NULL){
					runningPCB = dispatchList.front();
				}
				runningPCB->run();
				runningPCB->timeOfExecuting--;
				printf("\t\t进程(%s)成功运行1个时间单元。\n",runningPCB->name.data());
				runningPCB->wait();
				
				if (runningPCB->timeOfExecuting==0){
					runningPCB->finish();
				}
				runningPCB = runningPCB->next;
				// 删除已完成调度的进程 
				while (dispatchList.size()!=0 && runningPCB->timeOfExecuting==0){
					dispatchList.remove(runningPCB);
					printf("\t\t进程(%s)运行结束，已被剔除出调度队列。\n",runningPCB->name.data());
					runningPCB = runningPCB->next;
				}
				
				if(dispatchList.size()==0){
					runningPCB = NULL;
				}

			}
			Sleep(sleepTime);
		}while(dispatchList.size()!=0 || pcbList.size() !=0);
		 
		currentTime++;
		showTime();
		showListOfDispatching();
	}
	 
	// 展示当前时间 
	void showTime(){
	 	printf("\n\n\n\t:: 当前时间: %d\n",currentTime);
	}
	// 展示进程调度链表 
	void showListOfDispatching(){
		dispatchList.show();
	} 
	// 展示进程链表 
	void showListOfPCB(){
		this->pcbList.show();
	} 
};



int INIT_PROCESS_COUNT = 50; 					// 初始化进程数量
string PROCESS_NAME_PREFIX = "process_";		// 进程名前缀 
int PROCESS_COMMING_TIME_LOW = 1;				// 进程到达时间下界 
int PROCESS_COMMING_TIME_HIGH = 50;				// 进程到达时间上界
int PROCESS_EXECUTING_TIME_LOW = 0;				// 进程运行时间下界 
int PROCESS_EXECUTING_TIME_HIGH = 20;			// 进程运行时间上界
int SLEEP_TIME = 0;								// 每次调度后休眠的时间 


// 接收输入，如果非法则重新输入。 
void getCin(string tip,int &num){
	while(true){
		printf("%s",tip.data());
		scanf("%d",&num);
		if(num<=0){
			printf("输入有误，请重新输入\n");
		}else{
			break;
		}
	}
}


int main(){

	srand((int)time(0));

	getCin("请输入进程个数: ",INIT_PROCESS_COUNT);
	getCin("请输入进程到达时间下界: ",PROCESS_COMMING_TIME_LOW);
	getCin("请输入进程到达时间上界: ",PROCESS_COMMING_TIME_HIGH);
	getCin("请输入进程执行时间下界: ",PROCESS_EXECUTING_TIME_LOW);
	getCin("请输入进程执行时间上界: ",PROCESS_EXECUTING_TIME_HIGH);
	printf("\n信息输入成功！系统初始化中...\n\n");

	System system;
	for(int i=0;i<INIT_PROCESS_COUNT;i++){
		string name = PROCESS_NAME_PREFIX + toString(i);
		int commingTime =  getRandom(PROCESS_COMMING_TIME_LOW, PROCESS_COMMING_TIME_HIGH);
		int executingTime =  getRandom(PROCESS_EXECUTING_TIME_LOW, PROCESS_EXECUTING_TIME_HIGH);
		
		if (executingTime==-1){
			printf("信息输入有误，请检查进程执行时间上下界。\n");
			return 1;
		}
		if (commingTime ==-1){
			printf("信息输入有误，请检查进程到达时间上下界。\n"); 
			return 1;
		}
		
		PCB* p = new PCB(name,commingTime,executingTime,'W',NULL);
		system.addProcess(p);
	}
	printf("系统初始化成功！ \n");
	printf("\n\t以下是进程队列信息：\n\n"); 
	system.showListOfPCB();
	
	getCin("\n请输入每次调度后休眠的时间，以便于观察。(单位:毫秒): ",SLEEP_TIME);
	int flag = 1;
	getCin("输入'1'就可以开始调度了: ",flag);
	printf("\n-------------------- 开始调度 ---------------------\n");
	system.beginDispatching(SLEEP_TIME);

}

