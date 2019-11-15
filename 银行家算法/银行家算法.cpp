
# include<stdio.h>
# include<vector>
# include<map> 
# include<cstring> 
# include <cstdlib>
# include <Windows.h>

using namespace std;
int time = 0;				// ȫ����ʼʱ��
const int INITIAL_PROCESS_COUNT = 10;	// ��ʼ�������̸���  (�п��ܴ���ʧ��)
const int SLEEP_TIME = 100;			// ʱ��Ƭ��С 
const int TIME_LIMIT = 100;		// ʱ�価ͷ
const int N = 3;					// ��Դ���� 

const int PID_LOWWER_BOUND = 1;			// �û�����ID�½� 
const int PID_UPPER_BOUND = 10000;		// �û�����ID�Ͻ� 

const int INITIAL_RESOURCE_LOWWER_BOUND = 5000;	// ϵͳ��ʼ��ʱ�ĸ�����Դ�ĸ����½� 
const int INITIAL_RESOURCE_UPPER_BOUND = 10000;	// ϵͳ��ʼ��ʱ�ĸ�����Դ�ĸ����Ͻ� 
const int MAXRESOURCEGROUP_LOWWER_BOUND = 500;	// ���̵����������Դ����½� 
const int MAXRESOURCEGROUP_UPPER_BOUND = 1000; // ���̵����������Դ����Ͻ� 

const int PROBABILITY_EXIT = 100;	// ����ִ���˳�����ĸ��ʣ���BASE_OF_PROBABILITYΪ�� 
const int PROBABILITY_OCCUPY = 1000;	// ����ִ����Դ����ĸ��ʣ���BASE_OF_PROBABILITYΪ�� 
const int PROBABILITY_RELEASE = 100;	// ����ִ����Դ�ͷŵĸ��ʣ���BASE_OF_PROBABILITYΪ�� 
const int BASE_OF_PROBABILITY = 10000;

// ������ָ��� 
const int ASKFOROCCUPY = 0;		// ����ռ��
const int ASKFORRELEASE = 1;	// �����ͷ�
const int ASKFOREXIT	= 2;	// ������ֹ (����ȷ��) 
const int OCCUPY_AGREE = 8;			// ͬ����Դ�������� 
const int OCCUPY_DISAGREE = 9;			// �ܾ���Դ�������� 
const int RELEASE_AGREE = 10;			// ͬ����Դ�ͷ����� 
const int RELEASE_DISAGREE = 11;			// �ܾ���Դ�ͷ����� 



// ���� [low,high]����� 
int getRandom(int low,int high){
	return rand()%(high + 1- low) + low; 
}

// ������� (-1: ���� 0: ��ʾ���ʲ����㣬1����ʾ��������) 
int randomProbability(int numberator, int denominator){
	if (denominator <=0 || numberator<0)return -1;
	int ramdomA = getRandom(1,denominator);
	
	if (ramdomA>numberator){
		return 0;
	}
	return 1;
}


// �ϲ��ַ��� 
char* mergeStrings(char* prefix,char* content){
	char* target = new char[strlen(prefix) + strlen(content) + 1];
	strcpy(target, prefix);
	strcat(target,content);

    return target;
}



class Resource{
	
	// �ж���Դ���Ƿ�ȫΪ0
	public:static bool isAllZero(int* firstResourceGroup){
		for (int i=0;i<N;i++){
			if (firstResourceGroup[i]!=0)return false;
		}
		return true;
	}
	
	// ����һ����Դ�鷵�� 
	public:static int* copyResourceGroup(int* firstResourceGroup){
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = firstResourceGroup[i];
		}
		
		return resourceGroup;
	}
	
	// ����һ����Դ�飬����Դ������firstResourceGroup
	public:static int* generateSubsetOfResourceGroup(int* firstResourceGroup){
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = getRandom(0,firstResourceGroup[i]);
			//printf("%d %d ----\n",resourceGroup[i],firstResourceGroup[i]);
		}
		
		return resourceGroup;
	}
	
	
	// �жϵ�һ����Դ���Ƿ��ݵڶ�����Դ��(0: ������Ҫ��  1������Ҫ��)
	public:static int resourceGroupCompare(int* firstResourceGroup,int* secondResourceGroup){
		for (int i=0;i<N;i++){
			if (secondResourceGroup[i]>firstResourceGroup[i]){
				return 0;
			}
		}
		return 1;
	} 
	
	// �ж���Դ���Ƿ�Ϸ�(������Դ���Ƿ�С��0�ж�)(0: ������Ҫ��  1������Ҫ��)
	public:static int resourceGroupJudge(int* resourceGroup){
		for (int i=0;i<N;i++){
			if (resourceGroup[i]<0){
				return 0;
			}
		}
		return 1;
	} 
	
	
	
	// ��Դ���  (��һ����Դ���ȥ�ڶ�����Դ��) 
	public:static int* exeptResourceGroup(int* firstResourceGroup,int* secondResourceGroup){
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = firstResourceGroup[i] - secondResourceGroup[i];
		}
		return resourceGroup;
	}
	
	// ��Դ�ϲ�  (��һ����Դ����ϵڶ�����Դ��) 
	public:static int* mergeResourceGroup(int* firstResourceGroup,int* secondResourceGroup){
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = firstResourceGroup[i] + secondResourceGroup[i];
		}
		return resourceGroup;
		
	}
	
	// չʾ��Դ  
	public:static void showResourceGroup(char* prefix,int* resourceGroup){
	
		printf(prefix);
		for (int i=0;i<N;i++){
			printf("%d ",resourceGroup[i]);
		}
		printf("\n");
	}
	
	// �������һ����Դ�� 
	public:static int* randomResourceGroup(int low,int high){
		
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = getRandom(low,high);
		}
		return resourceGroup; 
	}
}; 





class Message{
	int senderPid;		// 0: ��ʾ����ϵͳ����   ����Ϊ�û����� 
	int receiverPid;	// 0: ��ʾ����ϵͳ����   ����Ϊ�û����� 
	int instruction;	// 0: ����ռ����Դ(��Ҫȷ��)  	1: �����ͷ���Դ(����ȷ��)		--> �û���������ָ��
						// 8: ͬ��ռ��(OCCUPY_AGREE) 	9: �ܾ�ռ��(OCCUPY_DISAGREE) 	--> ����ϵͳ�Ļظ�ָ�� 
						// 10:ͬ���ͷ�(RELEASE_AGREE)   11:�ܾ��ͷ�(RELEASE_DISAGREE) 	--> ����ϵͳ�Ļظ�ָ�� 
	int* resourceGroup;	// ��Դ��
	int* maxNeedResourceGroup;	// �����Դ�� (�ñ���ֻ���û�����������) 
	
	public: Message(int senderPid,int receiverPid,int instruction,int* resourceGroup,int* maxNeedResourceGroup){
		this->receiverPid = receiverPid;
		this->senderPid = senderPid;
		this->instruction = instruction;
		this->resourceGroup = resourceGroup;
		this->maxNeedResourceGroup = maxNeedResourceGroup;
	}
	
	public:Message(int senderPid,int receiverPid,int instruction,int* resourceGroup){
		this->receiverPid = receiverPid;
		this->senderPid = senderPid;
		this->instruction = instruction;
		this->resourceGroup = resourceGroup;
	}
	
	
	public: int* getMaxNeedResourceGroup(){
		return maxNeedResourceGroup;
	}
	public: int getSenderPid(){
		return senderPid;
	}
	public: int getReceiverPid(){
		return receiverPid;
	}
	
	public: int getInstruction(){
		return instruction;
	}
	public: int* getResourceGroup(){
		return resourceGroup;
	}
	
	// ��Ϣչʾ 
	public:void show(char* prefix){
		
		if (instruction==ASKFOROCCUPY){
			printf(mergeStrings(prefix,"�����߽���ID: %d\n"),senderPid);
			printf(mergeStrings(prefix,"�����߽���ID: %d\n"),receiverPid);
			printf(mergeStrings(prefix,"ָ���: %d (��Դ��������)\n"),instruction);
			Resource::showResourceGroup(mergeStrings(prefix,"���������Դ��: "),maxNeedResourceGroup);
			Resource::showResourceGroup(mergeStrings(prefix,"��������Դ��: "),resourceGroup);
		}
		if (instruction==ASKFORRELEASE){
			printf(mergeStrings(prefix,"�����߽���ID: %d\n"),senderPid);
			printf(mergeStrings(prefix,"�����߽���ID: %d\n"),receiverPid);
			printf(mergeStrings(prefix,"ָ���: %d (��Դ�ͷ�����)\n"),instruction);
		}
		if (instruction==ASKFOREXIT){
			printf(mergeStrings(prefix,"�����߽���ID: %d\n"),senderPid);
			printf(mergeStrings(prefix,"�����߽���ID: %d\n"),receiverPid);
			printf(mergeStrings(prefix,"ָ���: %d (������ֹ����)\n"),instruction);
		}
	}
			 
};


vector<Message> messageQueue;		// ���ϵͳ��Ϣ  (����ϵͳ�ͽ��̶��Ӷ�����ȡ��Ϣ�Լ�����з�����Ϣ) 

class Process {
	
	int pid;
	int* maxNeedResourceGroup;	// ���������Դ�� (��������������ռ�е���Դ����) 	 
	int* nowResourceGroup;		// ������ռ�е���Դ�� (�������ռ�е���Դ����) 
	
	int blockFlag;				// ������־ (0: ������  1: ����)	û���õ�.. 
	vector<Message> processMessageReceiveQueue;		// ������Ϣ���ܶ��� 
	
	public:Process(int pid,int* maxNeedResourceGroup,int* nowResourceGroup,int blockFlag){
		this->pid = pid;
		this->maxNeedResourceGroup = maxNeedResourceGroup;
		this->nowResourceGroup = nowResourceGroup;
		this->blockFlag = blockFlag;
	}
	
	public: int getPid(){
		return pid;
	}
	
	public: int* getNowResourceGroup(){
		return nowResourceGroup;
	}
	
	
	// չʾ���������� 
	public: void showInfo(){
		
		
		printf("	---------pid: %d--------\n",pid);
		
		Resource::showResourceGroup(" 	|	���������Դ��: ",maxNeedResourceGroup);
		Resource::showResourceGroup(" 	|	����ռ����Դ��: ",nowResourceGroup);
		
		printf("	|	������־: %d\n",blockFlag);
		printf("	-----------------------\n");
	}
	
	// �жϽ����Ƿ���������Դ����
	public: bool isFull(){
		return Resource::isAllZero(maxNeedResourceGroup);
	} 
	
	
	// ��������Ҫ������ (ֻ�������ÿһ�������һ����)
	public: void run(){
		 
		
		
		receiveMessage();
		handleMessage();
		
		// �������������Ҫ��������˳�
		if (isFull()){
			askForExit();
			return ; 
		}
		// ��ʾ�������� 
		if (blockFlag==1){
			return ;
		}
		
		
		// �Ը��������ͷ���Դ 
		if (randomProbability(PROBABILITY_RELEASE,BASE_OF_PROBABILITY)==1){
			askForReleaseResource();
			return; 
		}
		
		// �Ը���������Դ 
		if(randomProbability(PROBABILITY_OCCUPY,BASE_OF_PROBABILITY)==1){
			// ������������Դ�������� --- ���ܴ���������� 
			int* requestResourceGroup = Resource::generateSubsetOfResourceGroup(this->maxNeedResourceGroup);
			askForOccupyResource(requestResourceGroup); 
			return; 
		}
		
		
	}
	
	// ������Ϣ 
	void sendMessage(Message message){
		messageQueue.push_back(message);
	};
	
	// ��ϵͳ���Ͷ����н�����Ϣ 
	void receiveMessage(){
		vector<Message>::iterator it;
		for (it=messageQueue.begin();it!=messageQueue.end();){
			
			int rcvPid = it->getReceiverPid();
			if (rcvPid == pid){
				
				// ���д�ϵͳ��Ϣ������ȡ�����͸��Լ�����Ϣ 
				//printf("	���� (pid: %d) �յ���һ����Ϣ!\n",this->pid);
				this->processMessageReceiveQueue.push_back(*it);
				messageQueue.erase(it);
			}else{
				it++;
			}
		}
		
	};
	
	// ������Ϣ 
	void handleMessage(){

		vector<Message>::iterator it;
		for (it=processMessageReceiveQueue.begin();it!=processMessageReceiveQueue.end();it++){
			
			// ����Դռ������Ļظ� 
			if (it->getInstruction() == OCCUPY_AGREE){
				// ϵͳͬ������̽��������״̬ 
				changeBlockState(0); 
				
				// ռ����Դ������������������� 
				occupyResource(it->getResourceGroup());
				reduceMaxNeedResource(it->getResourceGroup()); 
				
				printf("		[ ���� (pid: %d) ] �������Դ�ѷ���!\n",this->pid);
			}
			if (it->getInstruction() == OCCUPY_DISAGREE){ 
				printf("		[ ���� (pid: %d) ] �������Դ����ʧ��!\n",this->pid);
			}
			
			
			if (it->getInstruction() == RELEASE_AGREE){
				// ϵͳͬ������̽��������״̬ 
				changeBlockState(0); 
				
				// �ͷ���Դ
				releaseResource(it->getResourceGroup());
				 
				printf("		[ ���� (pid: %d) ] �������Դ���ͷ�!\n",this->pid);
			} 
			
			if (it->getInstruction() == RELEASE_DISAGREE){
				
				printf("		[ ���� (pid: %d) ] �������Դ�ͷ�ʧ��!\n",this->pid);
			} 
			
		}
		
		// �����Ϣ���� 
		processMessageReceiveQueue.clear();	 
	}
	
	// �ı�����״̬ (0: ��������� 1: ��������) 
	void changeBlockState(int value){
		this->blockFlag = value;	 
	} 
	
	// ���������˳�  (����ȷ��) 
	void askForExit(){
		Message message(this->pid,0,ASKFOREXIT,this->nowResourceGroup,maxNeedResourceGroup);
		sendMessage(message);
		
		// ���� 
		this->changeBlockState(1);
	};
	
	// ����������Դռ��  (��Ҫȷ��)
	void askForOccupyResource(int* resourceGroup){
		// ��������Դ��  (������������Դ) 
		int* requestResourceGroup = Resource::generateSubsetOfResourceGroup(this->maxNeedResourceGroup);
		
		Message message(this->pid,0,ASKFOROCCUPY,requestResourceGroup,this->maxNeedResourceGroup);
		sendMessage(message);
		
		// ���� (�ȴ��ظ�) 
		this->changeBlockState(1);
	}
	
	// ����������Դ�ͷ�  (��Ҫȷ��) 
	void askForReleaseResource(){
		// ��������Դ��  (�����������ͷ���Դ) 
		int* releaseResourceGroup = Resource::generateSubsetOfResourceGroup(nowResourceGroup);
		
		// �����ͷ���Ϣ 
		Message message(this->pid,0,ASKFORRELEASE,releaseResourceGroup,maxNeedResourceGroup);
		sendMessage(message);
		
		// ���� (�ȴ��ظ�) 
		this->changeBlockState(1);
	}
	
	// ռ����Դ 
	void occupyResource(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			nowResourceGroup[i]+=deltaResourceGroup[i];
		}
	};
	
	
	// ����������������� 
	void reduceMaxNeedResource(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			maxNeedResourceGroup[i]-=deltaResourceGroup[i];
		}
	} 
	
	// �ͷ���Դ 
	void releaseResource(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			nowResourceGroup[i]-=deltaResourceGroup[i];
		}
	};

};

vector<Process> processQueue;		// ϵͳ���̶��� 
map<int,int> livingPids;			// ϵͳ�Ѿ����ڵ�pid 


 
// ����ϵͳ 
class OperateSystem{
	
	int pid;	// ϵͳ����ID 
	int* initialResourceGroup;	// ϵͳ��ʼ��Դ 
	int* systemResourceGroup;	// ��Դ�� (����ϵͳ��ӵ�е���Դ)
	vector<Message> systemMessageReceiveQueue;		// ������Ϣ���ܶ���
	int initialProcessCount;	// ��ʼ��������	
	/*
		int time = 0;				// ȫ����ʼʱ��
		const int INITIAL_PROCESS_COUNT = 10;	// ��ʼ�������̸���  (�п��ܴ���ʧ��)
		const int SLEEP_TIME = 100;			// ʱ��Ƭ��С 
		const int TIME_LIMIT = 5;		// ʱ�価ͷ
		const int N = 3;					// ��Դ���� 
		
		const int PID_LOWWER_BOUND = 1;			// �û�����ID�½� 
		const int PID_UPPER_BOUND = 10000;		// �û�����ID�Ͻ� 
		
		const int INITIAL_RESOURCE_LOWWER_BOUND = 5000;	// ϵͳ��ʼ��ʱ�ĸ�����Դ�ĸ����½� 
		const int INITIAL_RESOURCE_UPPER_BOUND = 10000;	// ϵͳ��ʼ��ʱ�ĸ�����Դ�ĸ����Ͻ� 
		const int MAXRESOURCEGROUP_LOWWER_BOUND = 500;	// ���̵����������Դ����½� 
		const int MAXRESOURCEGROUP_UPPER_BOUND = 1000;  // ���̵����������Դ����Ͻ� 
		
		const int PROBABILITY_EXIT = 100;	// ����ִ���˳�����ĸ��ʣ���BASE_OF_PROBABILITYΪ�� 
		const int PROBABILITY_OCCUPY = 1000;	// ����ִ����Դ����ĸ��ʣ���BASE_OF_PROBABILITYΪ�� 
		const int PROBABILITY_RELEASE = 100;	// ����ִ����Դ�ͷŵĸ��ʣ���BASE_OF_PROBABILITYΪ�� 
		const int BASE_OF_PROBABILITY = 10000;
	*/
	public:OperateSystem(int initialProcessCount){
		this->pid = 0;
		this->clear();
		
		// ��ʼ�����
		printf("------------------------------------\n") ;
		printf("|��ʼ����Դ		(����ϵͳ)\n");
		printf("|	������Ϊ %d\n",N);
		printf("|	ÿ����Դ�ĸ����ռ�Ϊ: [%d, %d]\n",INITIAL_RESOURCE_LOWWER_BOUND,INITIAL_RESOURCE_UPPER_BOUND);
		printf("------------------------------------\n") ;
		printf("------------------------------------\n") ;
		printf("|��ʼ����Դ		(�û�����)\n");
		printf("|	������Ϊ %d\n",N);
		printf("|	���������Դ��ĸ����ռ�Ϊ: [%d, %d]\n",MAXRESOURCEGROUP_LOWWER_BOUND,MAXRESOURCEGROUP_UPPER_BOUND);
		printf("------------------------------------\n") ;
		
		// ��ʼ����Դ 
		initialResourceGroup = Resource::randomResourceGroup(INITIAL_RESOURCE_LOWWER_BOUND,INITIAL_RESOURCE_UPPER_BOUND);
		systemResourceGroup = Resource::copyResourceGroup(initialResourceGroup);
		this->initialProcessCount = initialProcessCount;
		// �������ڲ��� 
		generateProcesses(initialProcessCount);
		showAllProcessInfo();

		
	}
	
	// ����ϵͳ��������Ҫ������ 
	public:void run(){
		vector<Process>::iterator it;
		printf("\n\n");
		for (time = 0; time < TIME_LIMIT; time++){
			printf("(ʱ��: %d)\n",time); 
			
			// ������Ϣ������������������ϵͳ�������� 
			receiveMessage();
			handleMessage();
			showSystemInfo();
			
			// ���н��� 
			for (it=processQueue.begin();it!=processQueue.end();it++){
				it->run();
			}
			
			
			printf("\n"); 
			
			// �ж�ϵͳ�Ƿ���������
			int flag = systemTest();
			if (flag==2){
				printf(" ::: ϵͳ����˳����ɣ�\n"); 
				break;
			}
			if (flag==1){
				printf(" ::: ϵͳ����������\n"); 
			}else{
				printf(" ::: ϵͳ���г���  �������: %d\n",flag); 
				break; 
			} 
			printf("\n\n"); 
			Sleep(SLEEP_TIME); 	// ���ߣ���ֹ����ִ�й��� 
		} 

	};
	
	// �ж�ϵͳ�Ƿ���������(��������Դ��ʧ����Դ��������) 
	// ����ֵ��-2����ʾ�����ڣ�-1����ʾ��Դ��ʧ��0����ʾ��Դ�������У�1����ʾ������2����ʾ����������� 
	int systemTest(){
		
		vector<Process>::iterator it;
		int* tmpResourceGroup = new int[N]{0};	// ��ʱ��Դ�� 
		for (it=processQueue.begin();it!=processQueue.end();it++){
			tmpResourceGroup = Resource::mergeResourceGroup(tmpResourceGroup,it->getNowResourceGroup()); 
		}
		tmpResourceGroup = Resource::mergeResourceGroup(tmpResourceGroup,systemResourceGroup); 
		tmpResourceGroup = Resource::exeptResourceGroup(initialResourceGroup,tmpResourceGroup);
		
		// ��־λ 
		int lossFlag=0;
		int addFlag=0;
		
		// ��ʼ���
		for (int i=0;i<N;i++){
			if (tmpResourceGroup[i]!=0){
				if(tmpResourceGroup[i]<0){
					addFlag = 1;
				}else{
					lossFlag = 1;
				} 
			}
		} 
		int flag = 1 - addFlag - 2*lossFlag; 
		if (flag == 1){
			if (processQueue.size()==0){
				flag = 2;
			}
		} 
		// ����ֵ
		return flag;
	}
	
	
	
	
	// �������� (��ʼ��ʱʹ��)
	void generateProcesses(int initialProcessCount){
		printf("*************************************************\n"); 
		printf("*  ����ϵͳ��ʼ�������� (��������: %d)\n",INITIAL_PROCESS_COUNT); 
		for (int i=1;i<=initialProcessCount;i++){
			int flag = 0;
			int pid = getRandom(PID_LOWWER_BOUND,PID_UPPER_BOUND);
			
			flag = createProcess(pid,Resource::randomResourceGroup(MAXRESOURCEGROUP_LOWWER_BOUND,MAXRESOURCEGROUP_UPPER_BOUND),new int[N]{0},0);
			
			if (flag==1){
				printf("*	pid: %d - ����, �����ɹ�!\n",pid);
			}else{
				printf("*	pid: %d - ����, ����ʧ��!  (�������: %d)\n", pid, flag);
			}
		} 
		printf("*************************************************\n"); 
	} 
	

	// ����Ϸ����ж�
	// ���� 
	// requestResourceGroup�������������Դ��
	// maxNeedResourceGroup: ���������Ҫ����Դ��
	// ����ֵ
	// (-3: �������Դ�� > ������������������Դ��) 
	// (-2: �������Դ�� >ϵͳ����Դ��) 
	// (-1: �������Դ�� < 0) 
	// (0:  ������̵���������� > ϵͳ����Դ��) 
	// (1:  ���Ϸ���Ҫ��)  
	int resourceGroupRequestJudge(int* requestResourceGroup,int* maxNeedResourceGroup){
		
		// �������Դ�����ܴ���ϵͳ����Դ��
		int condition0 = Resource::resourceGroupCompare(maxNeedResourceGroup,requestResourceGroup);
		if (condition0!=1){
			return -3;
		}
		
		// �������Դ�����ܴ���ϵͳ����Դ��
		int condition1 = Resource::resourceGroupCompare(systemResourceGroup,requestResourceGroup);
		if (condition1!=1){
			return -2;
		}
		
		// �������Դ������С��0
		int condition2 = Resource::resourceGroupJudge(requestResourceGroup);
		if (condition2!=1){
			return -1;
		}
		
		// ���̵�������������ܴ���ϵͳ����Դ��
		int condition3 =  Resource::resourceGroupCompare(systemResourceGroup,maxNeedResourceGroup);
		if (condition3!=1){
			return 0;
		}
		
		return 1; 
	} 
	
	
	
	
	
	// ��������  (-1: ��ʾ��Դ����Ƿ�  0: ��ʾ���̴��ڣ�1: �����ɹ�)
	int createProcess(int pid,int* maxNeedResourceGroup,int* nowResourceGroup,int blockFlag){
		if (livingPids[pid]!=0){
			return 0;
		}
		// �������Ҫ��Դ�� 
		int* needResourceGroup = Resource::exeptResourceGroup(maxNeedResourceGroup,nowResourceGroup);
		
		// �Ƿ��������� 
		if (resourceGroupRequestJudge(needResourceGroup,maxNeedResourceGroup) != 1){
			return -1;
		}

		// ���н��̴��� 
		Process process(pid,maxNeedResourceGroup,nowResourceGroup,blockFlag);
		
		// ϵͳ��Դ���� 
		systemResourceGroupAllocate(nowResourceGroup);
		
		// ������̶���, ��ʶ���̺�ռ�� 
		processQueue.push_back(process);
		livingPids[pid] = 1;
		
		return 1;
	} 
	
	// ɾ������ (0: �����������Ľ���, 1: ��ʾɾ���ɹ�) 
	int deleteProcess(int pid){
		vector<Process>::iterator it;
		for (it = processQueue.begin(); it != processQueue.end();it++ ){
			if (it->getPid()==pid){
				
				int* resourceGroup = it->getNowResourceGroup();
				
				// ϵͳ��Դ���� 
				systemResourceGroupRecover(resourceGroup);
				
				// ɾ��������Ϣ 
				processQueue.erase(it);
				livingPids[pid] = 0;
				
				return 1;
			} 
		}
		return 0;
		
	} 
	
	// ��ϵͳ���Ͷ����н�����Ϣ 
	void receiveMessage(){
		vector<Message>::iterator it;
		for (it=messageQueue.begin();it!=messageQueue.end();){
			
			int rcvPid = it->getReceiverPid();
			if (rcvPid == pid){
				// ���д�ϵͳ��Ϣ���Ͷ�����ȡ����Ϣ 
				printf("����ϵͳ (pid: %d) �յ���һ����Ϣ!\n",this->pid);
				printf("		---------------- ��Ϣ���� ---------------\n");
				it->show("		| "); 
				printf("		-----------------------------------------\n");
				this->systemMessageReceiveQueue.push_back(*it);
				messageQueue.erase(it);
			}else{
				it++;
			}
		}
		
	};
	
	// ������Ϣ 
	void handleMessage(){

		vector<Message>::iterator it;
		for (it=systemMessageReceiveQueue.begin();it!=systemMessageReceiveQueue.end();){
			
			int messageDeleteFlag = 0;	// ��Ϣɾ����־ 0Ϊ��ɾ�� 1Ϊɾ�� 
			
			// ��ʾ������Ҫ������Դ 
			if (it->getInstruction() == ASKFOROCCUPY){
				int id = it->getSenderPid(); 
				
				// �жϽ����Ƿ���ڣ����������ܷ�����Դ (����ʵ��������Ϊһ������) 
				if (livingPids[id]==0){
					printf("pid: %d �Ľ��̲�����!\n",id);
					break;  
				} 
				
				int* requestResourceGroup = it->getResourceGroup();
				int* maxNeedResourceGroup = it->getMaxNeedResourceGroup();
				int flag = resourceGroupRequestJudge(requestResourceGroup,maxNeedResourceGroup);
				if (flag == 1){
					
					// ������Դ (�ͷŲ���ϵͳ����Դ) 
					systemResourceGroupAllocate(requestResourceGroup);
					
					// ����̷���֪ͨ��֪ͨ�������Դ����ʹ����  
					Message message(0,id,OCCUPY_AGREE,requestResourceGroup);
					sendMessage(message); 
					
					
					printf("����ϵͳͬ���� [ ����(pid: %d) ] ����Դ��������!\n",id);
					
					messageDeleteFlag = 1; 
					
				}else{
					// ����̷���֪ͨ��֪ͨ��Դ���뱻�ܾ� 
					Message message(0,id,OCCUPY_DISAGREE,requestResourceGroup);
					sendMessage(message); 
				
					printf("����ϵͳ�ܾ��� [ ����(pid: %d) ] ����Դ��������!  (�������: %d)\n",id,flag);
				}
				
			}
			
			// ��ʾ���������ͷ���Դ 
			if (it->getInstruction() == ASKFORRELEASE){
				int id = it->getSenderPid();
				int* requestResourceGroup = it->getResourceGroup();
				
				
				// �ͷ���Դ 
				systemResourceGroupRecover(requestResourceGroup);
				
				// ����̷���֪ͨ��֪ͨ��Դ�ͷųɹ� 
				Message message(0,id,RELEASE_AGREE,requestResourceGroup);
				sendMessage(message); 
				
				printf("����ϵͳ�ɹ��ͷ��� [ ����(pid: %d) ] �Ĳ�����Դ!\n",id);
				
				messageDeleteFlag = 1; 
			}
			
			// ��ʾ����Ҫ�˳��� 
			if (it->getInstruction() == ASKFOREXIT){
				int id = it->getSenderPid();
				int flag = deleteProcess(id);
				
				if (flag==1){
					printf("����ϵͳ�ɹ�ɾ�� [ ����(pid: %d) ]��������������������Դ!\n",id);
					messageDeleteFlag = 1; 
				}else{
					printf("����ϵͳ�޷�ɾ�� [ ����(pid: %d) ] !  (�������: %d)\n",id,flag);
				}
			}
			
			if (messageDeleteFlag==1){
				systemMessageReceiveQueue.erase(it);   // ͬ��������ɾ������Ϣ����ͬ�ⲻɾ�����Ƴٵ�����ʱ��㴦�� 
			}else{
				it++;
			}
		}	 
	}
	
	
	// ϵͳ��Դ����
	void systemResourceGroupRecover(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			systemResourceGroup[i]+=deltaResourceGroup[i];
		} 
	};
	// ϵͳ��Դ����
	void systemResourceGroupAllocate(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			systemResourceGroup[i]-=deltaResourceGroup[i];
		} 
	};
	
	// չʾ������Ϣ (ϵͳ + ���̵�) 
	void showAllInfo(){
		showSystemInfo();
		showAllProcessInfo(); 
	}
	
	// չʾϵͳ����Ϣ (����չʾ���̵���Ϣ) 
	void showSystemInfo(){
		printf("**************** ����ϵͳ��Ϣ ****************\n");
		printf("* 	���У�	(%d������)\n",2);
		printf("*		ϵͳ��Ϣ���г��ȣ�%d\n",messageQueue.size());
		printf("*		���̶��г��ȣ�%d\n",processQueue.size());
		printf("* 	��Դ:	(%d����Դ)\n",N);
		Resource::showResourceGroup("*		ϵͳ���ӵ�е���Դ: ",this->initialResourceGroup);
		Resource::showResourceGroup("*		ϵͳ����ӵ�е���Դ: ",systemResourceGroup);
		printf("**********************************************\n");
	}
	
	// չʾ���н��̵���Ϣ 
	void showAllProcessInfo(){
		vector<Process>::iterator it;
		for (it = processQueue.begin();it!=processQueue.end();it++){
			it->showInfo();
		}
	}
	
	// ��ʼ����� 
	void clear(){
		messageQueue.clear();
		processQueue.clear();
		livingPids.clear();
	} 
	// ������Ϣ 
	void sendMessage(Message message){
		messageQueue.push_back(message);
	};
};


int main(){

	
	OperateSystem operateSystem(INITIAL_PROCESS_COUNT);

	operateSystem.run(); 
} 
