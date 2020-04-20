# include<stdio.h>
# include<string>
# include<cstring>
# include<iostream>
# include<stdlib.h>
# include<windows.h>
# include <ctime>
using namespace std;

enum stateType {WAITING,RUNNING,FINISHED};	// ����״̬����

// ����ת�ַ��� 
string toString(int a){
	string s = "";
	do{
		s = char(a%10+'0') + s; 
		a/=10;
	}while(a!=0);
	return s;
}

// ����[l, r]�������(ʧ�ܷ���-1) 
int getRandom(int l,int r){
	if(l>r)return -1;
	return rand()%(r-l+1)+l;
	
} 

// PCB�ṹ 
struct PCB{
public:
	string name;			// ������ 
	int timeOfComming;		// ����ʱ�� 
	int timeOfExecuting;	// ����ʱ��
	char state;		// ����״̬
	PCB* next;				// ����ָ�� 
	
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
		printf("\t\t| ������: %s\n",name.data());
		printf("\t\t| ����ʱ��: %d\n",timeOfComming);
		printf("\t\t| ʣ������ʱ��: %d\n",timeOfExecuting);
		printf("\t\t| ����״̬: %c\n",state);
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

// ����PCB����(��������) 
class ListOfPCB{
	PCB* dummyHead;		// ��ͷ�ڵ� 
	PCB* tail;			// ָ��λ��β���Ľ���(��ʼʱָ����ͷ) 
	int length;			// ������ 
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

	// ����PCB 
	void push(PCB* p){
		PCB* copyP = new PCB(*p);
		this->tail->next = copyP;
		this->tail = this->tail->next; 
		length++;
	}
	
	// ɾ��PCB
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
		// ��ɾ����Ϊβ�ڵ�ʱ��β�ڵ���Ҫ����
		if (p==tail){
			tail = cur;	
		}
		this->length--;
		return true; 
	}
	
	void show(){
		PCB* cur = dummyHead;
		printf("\t\t����PCB������: %d\n",length);
		while(cur->next!=NULL){
			cur->next->show();
			cur = cur->next;
		}
	}
	
	
};


// ���̵�������(����ѭ������) 
class LoopListOfPCB{

	PCB* dummyHead;		// ��ͷ�ڵ� 
	PCB* tail;			// ָ��λ��β���Ľ���(��ʼʱָ����ͷ) 
	int length;			// ������ 
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
	
	// ����PCB 
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
	
	// ɾ��PCB 
	bool remove(PCB* p){
		if (p==NULL){
			return false;
		}
		
		if (p == dummyHead->next){
			// ��ʾֻ��һ��Ԫ�� 
			if (length == 1){
				dummyHead->next = NULL;
				tail = dummyHead;
			}else{
				// ��ʾ��ֻ��һ��Ԫ�� 
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
			// ��ʾ������������p�ڵ� 
			if (cur->next!=p){
				return false;
			} 
			cur->next = cur->next->next;
			
			// ��ɾ����Ϊβ�ڵ�ʱ��β�ڵ���Ҫ����
			if(p==tail){
				tail = cur;
			}
		}
		
		length--;
		return true;	
	}
	void show(){
		PCB* cur = dummyHead;
		printf("\t\t���̵���������: %d\n",length);
		for(int i=0;i<length;i++){
			cur->next->show();
			cur = cur->next;
		}		
	}
	
	
};


// ϵͳ (ִ�е���) 
class System{

	ListOfPCB pcbList;			// �������� 
	LoopListOfPCB dispatchList;	// ���̵������� 
	int currentTime;			// ��ǰʱ��
	PCB* runningPCB;
public:
	System(){
		currentTime = 0;
		runningPCB = NULL;
	}
	
	// ��ӽ���
	void addProcess(PCB* p){
		this->pcbList.push(p);
	}
	
	
	 
	// �ѵ�ǰ�ܽ��е��ȵĽ��̼�����ȶ��� 
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
	
	// ��ʼִ��ʱ��Ƭ��ת���� 
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
				printf("\t\t����(%s)�ɹ�����1��ʱ�䵥Ԫ��\n",runningPCB->name.data());
				runningPCB->wait();
				
				if (runningPCB->timeOfExecuting==0){
					runningPCB->finish();
				}
				runningPCB = runningPCB->next;
				// ɾ������ɵ��ȵĽ��� 
				while (dispatchList.size()!=0 && runningPCB->timeOfExecuting==0){
					dispatchList.remove(runningPCB);
					printf("\t\t����(%s)���н������ѱ��޳������ȶ��С�\n",runningPCB->name.data());
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
	 
	// չʾ��ǰʱ�� 
	void showTime(){
	 	printf("\n\n\n\t:: ��ǰʱ��: %d\n",currentTime);
	}
	// չʾ���̵������� 
	void showListOfDispatching(){
		dispatchList.show();
	} 
	// չʾ�������� 
	void showListOfPCB(){
		this->pcbList.show();
	} 
};



int INIT_PROCESS_COUNT = 50; 					// ��ʼ����������
string PROCESS_NAME_PREFIX = "process_";		// ������ǰ׺ 
int PROCESS_COMMING_TIME_LOW = 1;				// ���̵���ʱ���½� 
int PROCESS_COMMING_TIME_HIGH = 50;				// ���̵���ʱ���Ͻ�
int PROCESS_EXECUTING_TIME_LOW = 0;				// ��������ʱ���½� 
int PROCESS_EXECUTING_TIME_HIGH = 20;			// ��������ʱ���Ͻ�
int SLEEP_TIME = 0;								// ÿ�ε��Ⱥ����ߵ�ʱ�� 


// �������룬����Ƿ����������롣 
void getCin(string tip,int &num){
	while(true){
		printf("%s",tip.data());
		scanf("%d",&num);
		if(num<=0){
			printf("������������������\n");
		}else{
			break;
		}
	}
}


int main(){

	srand((int)time(0));

	getCin("��������̸���: ",INIT_PROCESS_COUNT);
	getCin("��������̵���ʱ���½�: ",PROCESS_COMMING_TIME_LOW);
	getCin("��������̵���ʱ���Ͻ�: ",PROCESS_COMMING_TIME_HIGH);
	getCin("���������ִ��ʱ���½�: ",PROCESS_EXECUTING_TIME_LOW);
	getCin("���������ִ��ʱ���Ͻ�: ",PROCESS_EXECUTING_TIME_HIGH);
	printf("\n��Ϣ����ɹ���ϵͳ��ʼ����...\n\n");

	System system;
	for(int i=0;i<INIT_PROCESS_COUNT;i++){
		string name = PROCESS_NAME_PREFIX + toString(i);
		int commingTime =  getRandom(PROCESS_COMMING_TIME_LOW, PROCESS_COMMING_TIME_HIGH);
		int executingTime =  getRandom(PROCESS_EXECUTING_TIME_LOW, PROCESS_EXECUTING_TIME_HIGH);
		
		if (executingTime==-1){
			printf("��Ϣ���������������ִ��ʱ�����½硣\n");
			return 1;
		}
		if (commingTime ==-1){
			printf("��Ϣ��������������̵���ʱ�����½硣\n"); 
			return 1;
		}
		
		PCB* p = new PCB(name,commingTime,executingTime,'W',NULL);
		system.addProcess(p);
	}
	printf("ϵͳ��ʼ���ɹ��� \n");
	printf("\n\t�����ǽ��̶�����Ϣ��\n\n"); 
	system.showListOfPCB();
	
	getCin("\n������ÿ�ε��Ⱥ����ߵ�ʱ�䣬�Ա��ڹ۲졣(��λ:����): ",SLEEP_TIME);
	int flag = 1;
	getCin("����'1'�Ϳ��Կ�ʼ������: ",flag);
	printf("\n-------------------- ��ʼ���� ---------------------\n");
	system.beginDispatching(SLEEP_TIME);

}

