
# include<stdio.h>
# include<vector>
# include<map> 
# include<cstring> 
# include <cstdlib>
# include <Windows.h>

using namespace std;
int time = 0;				// 全局起始时间
const int INITIAL_PROCESS_COUNT = 10;	// 初始创建进程个数  (有可能创建失败)
const int SLEEP_TIME = 100;			// 时间片大小 
const int TIME_LIMIT = 100;		// 时间尽头
const int N = 3;					// 资源种类 

const int PID_LOWWER_BOUND = 1;			// 用户进程ID下界 
const int PID_UPPER_BOUND = 10000;		// 用户进程ID上界 

const int INITIAL_RESOURCE_LOWWER_BOUND = 5000;	// 系统初始化时的各类资源的个数下界 
const int INITIAL_RESOURCE_UPPER_BOUND = 10000;	// 系统初始化时的各类资源的个数上界 
const int MAXRESOURCEGROUP_LOWWER_BOUND = 500;	// 进程的最大需求资源组的下界 
const int MAXRESOURCEGROUP_UPPER_BOUND = 1000; // 进程的最大需求资源组的上界 

const int PROBABILITY_EXIT = 100;	// 进程执行退出任务的概率，以BASE_OF_PROBABILITY为底 
const int PROBABILITY_OCCUPY = 1000;	// 进程执行资源申请的概率，以BASE_OF_PROBABILITY为底 
const int PROBABILITY_RELEASE = 100;	// 进程执行资源释放的概率，以BASE_OF_PROBABILITY为底 
const int BASE_OF_PROBABILITY = 10000;

// 以下是指令号 
const int ASKFOROCCUPY = 0;		// 请求占有
const int ASKFORRELEASE = 1;	// 请求释放
const int ASKFOREXIT	= 2;	// 请求终止 (无需确认) 
const int OCCUPY_AGREE = 8;			// 同意资源申请请求 
const int OCCUPY_DISAGREE = 9;			// 拒绝资源申请请求 
const int RELEASE_AGREE = 10;			// 同意资源释放请求 
const int RELEASE_DISAGREE = 11;			// 拒绝资源释放请求 



// 返回 [low,high]随机数 
int getRandom(int low,int high){
	return rand()%(high + 1- low) + low; 
}

// 随机概率 (-1: 出错， 0: 表示概率不满足，1：表示概率满足) 
int randomProbability(int numberator, int denominator){
	if (denominator <=0 || numberator<0)return -1;
	int ramdomA = getRandom(1,denominator);
	
	if (ramdomA>numberator){
		return 0;
	}
	return 1;
}


// 合并字符串 
char* mergeStrings(char* prefix,char* content){
	char* target = new char[strlen(prefix) + strlen(content) + 1];
	strcpy(target, prefix);
	strcat(target,content);

    return target;
}



class Resource{
	
	// 判断资源组是否全为0
	public:static bool isAllZero(int* firstResourceGroup){
		for (int i=0;i<N;i++){
			if (firstResourceGroup[i]!=0)return false;
		}
		return true;
	}
	
	// 拷贝一个资源组返回 
	public:static int* copyResourceGroup(int* firstResourceGroup){
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = firstResourceGroup[i];
		}
		
		return resourceGroup;
	}
	
	// 生成一个资源组，其资源不大于firstResourceGroup
	public:static int* generateSubsetOfResourceGroup(int* firstResourceGroup){
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = getRandom(0,firstResourceGroup[i]);
			//printf("%d %d ----\n",resourceGroup[i],firstResourceGroup[i]);
		}
		
		return resourceGroup;
	}
	
	
	// 判断第一个资源组是否恒≥第二个资源组(0: 不符合要求  1：符合要求)
	public:static int resourceGroupCompare(int* firstResourceGroup,int* secondResourceGroup){
		for (int i=0;i<N;i++){
			if (secondResourceGroup[i]>firstResourceGroup[i]){
				return 0;
			}
		}
		return 1;
	} 
	
	// 判断资源组是否合法(根据资源数是否小于0判断)(0: 不符合要求  1：符合要求)
	public:static int resourceGroupJudge(int* resourceGroup){
		for (int i=0;i<N;i++){
			if (resourceGroup[i]<0){
				return 0;
			}
		}
		return 1;
	} 
	
	
	
	// 资源相差  (第一个资源组减去第二个资源组) 
	public:static int* exeptResourceGroup(int* firstResourceGroup,int* secondResourceGroup){
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = firstResourceGroup[i] - secondResourceGroup[i];
		}
		return resourceGroup;
	}
	
	// 资源合并  (第一个资源组加上第二个资源组) 
	public:static int* mergeResourceGroup(int* firstResourceGroup,int* secondResourceGroup){
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = firstResourceGroup[i] + secondResourceGroup[i];
		}
		return resourceGroup;
		
	}
	
	// 展示资源  
	public:static void showResourceGroup(char* prefix,int* resourceGroup){
	
		printf(prefix);
		for (int i=0;i<N;i++){
			printf("%d ",resourceGroup[i]);
		}
		printf("\n");
	}
	
	// 随机生成一个资源组 
	public:static int* randomResourceGroup(int low,int high){
		
		int* resourceGroup = new int[N];
		for (int i=0;i<N;i++){
			resourceGroup[i] = getRandom(low,high);
		}
		return resourceGroup; 
	}
}; 





class Message{
	int senderPid;		// 0: 表示操作系统进程   否则为用户进程 
	int receiverPid;	// 0: 表示操作系统进程   否则为用户进程 
	int instruction;	// 0: 请求占有资源(需要确认)  	1: 请求释放资源(无需确认)		--> 用户进的请求指令
						// 8: 同意占有(OCCUPY_AGREE) 	9: 拒绝占有(OCCUPY_DISAGREE) 	--> 操作系统的回复指令 
						// 10:同意释放(RELEASE_AGREE)   11:拒绝释放(RELEASE_DISAGREE) 	--> 操作系统的回复指令 
	int* resourceGroup;	// 资源组
	int* maxNeedResourceGroup;	// 最大资源组 (该变量只对用户进程有意义) 
	
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
	
	// 信息展示 
	public:void show(char* prefix){
		
		if (instruction==ASKFOROCCUPY){
			printf(mergeStrings(prefix,"发送者进程ID: %d\n"),senderPid);
			printf(mergeStrings(prefix,"接受者进程ID: %d\n"),receiverPid);
			printf(mergeStrings(prefix,"指令号: %d (资源申请请求)\n"),instruction);
			Resource::showResourceGroup(mergeStrings(prefix,"最大需求资源组: "),maxNeedResourceGroup);
			Resource::showResourceGroup(mergeStrings(prefix,"请求获得资源组: "),resourceGroup);
		}
		if (instruction==ASKFORRELEASE){
			printf(mergeStrings(prefix,"发送者进程ID: %d\n"),senderPid);
			printf(mergeStrings(prefix,"接受者进程ID: %d\n"),receiverPid);
			printf(mergeStrings(prefix,"指令号: %d (资源释放请求)\n"),instruction);
		}
		if (instruction==ASKFOREXIT){
			printf(mergeStrings(prefix,"发送者进程ID: %d\n"),senderPid);
			printf(mergeStrings(prefix,"接受者进程ID: %d\n"),receiverPid);
			printf(mergeStrings(prefix,"指令号: %d (进程终止请求)\n"),instruction);
		}
	}
			 
};


vector<Message> messageQueue;		// 存放系统消息  (操作系统和进程都从队列中取消息以及向队列发送消息) 

class Process {
	
	int pid;
	int* maxNeedResourceGroup;	// 最大需求资源组 (代表进程最多所需占有的资源向量) 	 
	int* nowResourceGroup;		// 现在所占有的资源组 (代表进程占有的资源向量) 
	
	int blockFlag;				// 阻塞标志 (0: 非阻塞  1: 阻塞)	没有用到.. 
	vector<Message> processMessageReceiveQueue;		// 进程消息接受队列 
	
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
	
	
	// 展示进程上下文 
	public: void showInfo(){
		
		
		printf("	---------pid: %d--------\n",pid);
		
		Resource::showResourceGroup(" 	|	最大需求资源组: ",maxNeedResourceGroup);
		Resource::showResourceGroup(" 	|	现在占有资源组: ",nowResourceGroup);
		
		printf("	|	阻塞标志: %d\n",blockFlag);
		printf("	-----------------------\n");
	}
	
	// 判断进程是否满足了资源需求
	public: bool isFull(){
		return Resource::isAllZero(maxNeedResourceGroup);
	} 
	
	
	// 进程运行要做的事 (只允许进程每一次最多做一件事)
	public: void run(){
		 
		
		
		receiveMessage();
		handleMessage();
		
		// 如果进程满足了要求，则进程退出
		if (isFull()){
			askForExit();
			return ; 
		}
		// 表示进程阻塞 
		if (blockFlag==1){
			return ;
		}
		
		
		// 以概率请求释放资源 
		if (randomProbability(PROBABILITY_RELEASE,BASE_OF_PROBABILITY)==1){
			askForReleaseResource();
			return; 
		}
		
		// 以概率申请资源 
		if(randomProbability(PROBABILITY_OCCUPY,BASE_OF_PROBABILITY)==1){
			// 这里限制了资源的请求数 --- 不能大于最大需求 
			int* requestResourceGroup = Resource::generateSubsetOfResourceGroup(this->maxNeedResourceGroup);
			askForOccupyResource(requestResourceGroup); 
			return; 
		}
		
		
	}
	
	// 发送消息 
	void sendMessage(Message message){
		messageQueue.push_back(message);
	};
	
	// 从系统发送队列中接受信息 
	void receiveMessage(){
		vector<Message>::iterator it;
		for (it=messageQueue.begin();it!=messageQueue.end();){
			
			int rcvPid = it->getReceiverPid();
			if (rcvPid == pid){
				
				// 进行从系统消息队列中取出发送给自己的消息 
				//printf("	进程 (pid: %d) 收到了一条信息!\n",this->pid);
				this->processMessageReceiveQueue.push_back(*it);
				messageQueue.erase(it);
			}else{
				it++;
			}
		}
		
	};
	
	// 处理信息 
	void handleMessage(){

		vector<Message>::iterator it;
		for (it=processMessageReceiveQueue.begin();it!=processMessageReceiveQueue.end();it++){
			
			// 对资源占有请求的回复 
			if (it->getInstruction() == OCCUPY_AGREE){
				// 系统同意则进程进入非阻塞状态 
				changeBlockState(0); 
				
				// 占有资源，并且最大需求量减少 
				occupyResource(it->getResourceGroup());
				reduceMaxNeedResource(it->getResourceGroup()); 
				
				printf("		[ 进程 (pid: %d) ] 请求的资源已分配!\n",this->pid);
			}
			if (it->getInstruction() == OCCUPY_DISAGREE){ 
				printf("		[ 进程 (pid: %d) ] 请求的资源分配失败!\n",this->pid);
			}
			
			
			if (it->getInstruction() == RELEASE_AGREE){
				// 系统同意则进程进入非阻塞状态 
				changeBlockState(0); 
				
				// 释放资源
				releaseResource(it->getResourceGroup());
				 
				printf("		[ 进程 (pid: %d) ] 请求的资源已释放!\n",this->pid);
			} 
			
			if (it->getInstruction() == RELEASE_DISAGREE){
				
				printf("		[ 进程 (pid: %d) ] 请求的资源释放失败!\n",this->pid);
			} 
			
		}
		
		// 清空消息队列 
		processMessageReceiveQueue.clear();	 
	}
	
	// 改变阻塞状态 (0: 解除阻塞， 1: 进入阻塞) 
	void changeBlockState(int value){
		this->blockFlag = value;	 
	} 
	
	// 进程申请退出  (无需确认) 
	void askForExit(){
		Message message(this->pid,0,ASKFOREXIT,this->nowResourceGroup,maxNeedResourceGroup);
		sendMessage(message);
		
		// 阻塞 
		this->changeBlockState(1);
	};
	
	// 进程申请资源占用  (需要确认)
	void askForOccupyResource(int* resourceGroup){
		// 生成子资源组  (在这是请求资源) 
		int* requestResourceGroup = Resource::generateSubsetOfResourceGroup(this->maxNeedResourceGroup);
		
		Message message(this->pid,0,ASKFOROCCUPY,requestResourceGroup,this->maxNeedResourceGroup);
		sendMessage(message);
		
		// 阻塞 (等待回复) 
		this->changeBlockState(1);
	}
	
	// 进程申请资源释放  (需要确认) 
	void askForReleaseResource(){
		// 生成子资源组  (在这是请求释放资源) 
		int* releaseResourceGroup = Resource::generateSubsetOfResourceGroup(nowResourceGroup);
		
		// 发送释放信息 
		Message message(this->pid,0,ASKFORRELEASE,releaseResourceGroup,maxNeedResourceGroup);
		sendMessage(message);
		
		// 阻塞 (等待回复) 
		this->changeBlockState(1);
	}
	
	// 占用资源 
	void occupyResource(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			nowResourceGroup[i]+=deltaResourceGroup[i];
		}
	};
	
	
	// 减少最大允许需求量 
	void reduceMaxNeedResource(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			maxNeedResourceGroup[i]-=deltaResourceGroup[i];
		}
	} 
	
	// 释放资源 
	void releaseResource(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			nowResourceGroup[i]-=deltaResourceGroup[i];
		}
	};

};

vector<Process> processQueue;		// 系统进程队列 
map<int,int> livingPids;			// 系统已经存在的pid 


 
// 操作系统 
class OperateSystem{
	
	int pid;	// 系统进程ID 
	int* initialResourceGroup;	// 系统初始资源 
	int* systemResourceGroup;	// 资源组 (代表系统所拥有的资源)
	vector<Message> systemMessageReceiveQueue;		// 进程消息接受队列
	int initialProcessCount;	// 初始进程数量	
	/*
		int time = 0;				// 全局起始时间
		const int INITIAL_PROCESS_COUNT = 10;	// 初始创建进程个数  (有可能创建失败)
		const int SLEEP_TIME = 100;			// 时间片大小 
		const int TIME_LIMIT = 5;		// 时间尽头
		const int N = 3;					// 资源种类 
		
		const int PID_LOWWER_BOUND = 1;			// 用户进程ID下界 
		const int PID_UPPER_BOUND = 10000;		// 用户进程ID上界 
		
		const int INITIAL_RESOURCE_LOWWER_BOUND = 5000;	// 系统初始化时的各类资源的个数下界 
		const int INITIAL_RESOURCE_UPPER_BOUND = 10000;	// 系统初始化时的各类资源的个数上界 
		const int MAXRESOURCEGROUP_LOWWER_BOUND = 500;	// 进程的最大需求资源组的下界 
		const int MAXRESOURCEGROUP_UPPER_BOUND = 1000;  // 进程的最大需求资源组的上界 
		
		const int PROBABILITY_EXIT = 100;	// 进程执行退出任务的概率，以BASE_OF_PROBABILITY为底 
		const int PROBABILITY_OCCUPY = 1000;	// 进程执行资源申请的概率，以BASE_OF_PROBABILITY为底 
		const int PROBABILITY_RELEASE = 100;	// 进程执行资源释放的概率，以BASE_OF_PROBABILITY为底 
		const int BASE_OF_PROBABILITY = 10000;
	*/
	public:OperateSystem(int initialProcessCount){
		this->pid = 0;
		this->clear();
		
		// 初始化输出
		printf("------------------------------------\n") ;
		printf("|初始化资源		(操作系统)\n");
		printf("|	种类数为 %d\n",N);
		printf("|	每类资源的个数空间为: [%d, %d]\n",INITIAL_RESOURCE_LOWWER_BOUND,INITIAL_RESOURCE_UPPER_BOUND);
		printf("------------------------------------\n") ;
		printf("------------------------------------\n") ;
		printf("|初始化资源		(用户进程)\n");
		printf("|	种类数为 %d\n",N);
		printf("|	最大需求资源组的个数空间为: [%d, %d]\n",MAXRESOURCEGROUP_LOWWER_BOUND,MAXRESOURCEGROUP_UPPER_BOUND);
		printf("------------------------------------\n") ;
		
		// 初始化资源 
		initialResourceGroup = Resource::randomResourceGroup(INITIAL_RESOURCE_LOWWER_BOUND,INITIAL_RESOURCE_UPPER_BOUND);
		systemResourceGroup = Resource::copyResourceGroup(initialResourceGroup);
		this->initialProcessCount = initialProcessCount;
		// 以下正在测试 
		generateProcesses(initialProcessCount);
		showAllProcessInfo();

		
	}
	
	// 操作系统运行所需要做的事 
	public:void run(){
		vector<Process>::iterator it;
		printf("\n\n");
		for (time = 0; time < TIME_LIMIT; time++){
			printf("(时间: %d)\n",time); 
			
			// 接受消息并处理，处理后输出操作系统的上下文 
			receiveMessage();
			handleMessage();
			showSystemInfo();
			
			// 运行进程 
			for (it=processQueue.begin();it!=processQueue.end();it++){
				it->run();
			}
			
			
			printf("\n"); 
			
			// 判断系统是否运行正常
			int flag = systemTest();
			if (flag==2){
				printf(" ::: 系统工作顺利完成！\n"); 
				break;
			}
			if (flag==1){
				printf(" ::: 系统运行正常！\n"); 
			}else{
				printf(" ::: 系统运行出错！  错误代码: %d\n",flag); 
				break; 
			} 
			printf("\n\n"); 
			Sleep(SLEEP_TIME); 	// 休眠，防止程序执行过快 
		} 

	};
	
	// 判断系统是否运行正常(不存在资源丢失及资源无中生有) 
	// 返回值：-2：表示都存在，-1：表示资源丢失，0：表示资源无中生有，1：表示正常，2：表示工作都完成了 
	int systemTest(){
		
		vector<Process>::iterator it;
		int* tmpResourceGroup = new int[N]{0};	// 临时资源组 
		for (it=processQueue.begin();it!=processQueue.end();it++){
			tmpResourceGroup = Resource::mergeResourceGroup(tmpResourceGroup,it->getNowResourceGroup()); 
		}
		tmpResourceGroup = Resource::mergeResourceGroup(tmpResourceGroup,systemResourceGroup); 
		tmpResourceGroup = Resource::exeptResourceGroup(initialResourceGroup,tmpResourceGroup);
		
		// 标志位 
		int lossFlag=0;
		int addFlag=0;
		
		// 开始检查
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
		// 返回值
		return flag;
	}
	
	
	
	
	// 进程生成 (初始化时使用)
	void generateProcesses(int initialProcessCount){
		printf("*************************************************\n"); 
		printf("*  操作系统开始创建进程 (创建个数: %d)\n",INITIAL_PROCESS_COUNT); 
		for (int i=1;i<=initialProcessCount;i++){
			int flag = 0;
			int pid = getRandom(PID_LOWWER_BOUND,PID_UPPER_BOUND);
			
			flag = createProcess(pid,Resource::randomResourceGroup(MAXRESOURCEGROUP_LOWWER_BOUND,MAXRESOURCEGROUP_UPPER_BOUND),new int[N]{0},0);
			
			if (flag==1){
				printf("*	pid: %d - 进程, 创建成功!\n",pid);
			}else{
				printf("*	pid: %d - 进程, 创建失败!  (错误代号: %d)\n", pid, flag);
			}
		} 
		printf("*************************************************\n"); 
	} 
	

	// 分配合法性判断
	// 参数 
	// requestResourceGroup：进程申请的资源组
	// maxNeedResourceGroup: 进程最多需要的资源组
	// 返回值
	// (-3: 请求的资源数 > 进程所能请求的最大资源数) 
	// (-2: 请求的资源数 >系统的资源数) 
	// (-1: 请求的资源数 < 0) 
	// (0:  请求进程的最大需求数 > 系统的资源数) 
	// (1:  符合分配要求)  
	int resourceGroupRequestJudge(int* requestResourceGroup,int* maxNeedResourceGroup){
		
		// 分配的资源数不能大于系统的资源数
		int condition0 = Resource::resourceGroupCompare(maxNeedResourceGroup,requestResourceGroup);
		if (condition0!=1){
			return -3;
		}
		
		// 分配的资源数不能大于系统的资源数
		int condition1 = Resource::resourceGroupCompare(systemResourceGroup,requestResourceGroup);
		if (condition1!=1){
			return -2;
		}
		
		// 分配的资源数不能小于0
		int condition2 = Resource::resourceGroupJudge(requestResourceGroup);
		if (condition2!=1){
			return -1;
		}
		
		// 进程的最大需求数不能大于系统的资源数
		int condition3 =  Resource::resourceGroupCompare(systemResourceGroup,maxNeedResourceGroup);
		if (condition3!=1){
			return 0;
		}
		
		return 1; 
	} 
	
	
	
	
	
	// 创建进程  (-1: 表示资源请求非法  0: 表示进程存在，1: 创建成功)
	int createProcess(int pid,int* maxNeedResourceGroup,int* nowResourceGroup,int blockFlag){
		if (livingPids[pid]!=0){
			return 0;
		}
		// 获得所需要资源组 
		int* needResourceGroup = Resource::exeptResourceGroup(maxNeedResourceGroup,nowResourceGroup);
		
		// 是否允许申请 
		if (resourceGroupRequestJudge(needResourceGroup,maxNeedResourceGroup) != 1){
			return -1;
		}

		// 进行进程创建 
		Process process(pid,maxNeedResourceGroup,nowResourceGroup,blockFlag);
		
		// 系统资源分配 
		systemResourceGroupAllocate(nowResourceGroup);
		
		// 加入进程队列, 标识进程号占用 
		processQueue.push_back(process);
		livingPids[pid] = 1;
		
		return 1;
	} 
	
	// 删除进程 (0: 不存在这样的进程, 1: 表示删除成功) 
	int deleteProcess(int pid){
		vector<Process>::iterator it;
		for (it = processQueue.begin(); it != processQueue.end();it++ ){
			if (it->getPid()==pid){
				
				int* resourceGroup = it->getNowResourceGroup();
				
				// 系统资源回收 
				systemResourceGroupRecover(resourceGroup);
				
				// 删除进程信息 
				processQueue.erase(it);
				livingPids[pid] = 0;
				
				return 1;
			} 
		}
		return 0;
		
	} 
	
	// 从系统发送队列中接受信息 
	void receiveMessage(){
		vector<Message>::iterator it;
		for (it=messageQueue.begin();it!=messageQueue.end();){
			
			int rcvPid = it->getReceiverPid();
			if (rcvPid == pid){
				// 进行从系统消息发送队列中取回信息 
				printf("操作系统 (pid: %d) 收到了一条消息!\n",this->pid);
				printf("		---------------- 消息内容 ---------------\n");
				it->show("		| "); 
				printf("		-----------------------------------------\n");
				this->systemMessageReceiveQueue.push_back(*it);
				messageQueue.erase(it);
			}else{
				it++;
			}
		}
		
	};
	
	// 处理信息 
	void handleMessage(){

		vector<Message>::iterator it;
		for (it=systemMessageReceiveQueue.begin();it!=systemMessageReceiveQueue.end();){
			
			int messageDeleteFlag = 0;	// 消息删除标志 0为不删除 1为删除 
			
			// 表示进程需要申请资源 
			if (it->getInstruction() == ASKFOROCCUPY){
				int id = it->getSenderPid(); 
				
				// 判断进程是否存在，不存在则不能分配资源 (这其实还可以作为一个函数) 
				if (livingPids[id]==0){
					printf("pid: %d 的进程不存在!\n",id);
					break;  
				} 
				
				int* requestResourceGroup = it->getResourceGroup();
				int* maxNeedResourceGroup = it->getMaxNeedResourceGroup();
				int flag = resourceGroupRequestJudge(requestResourceGroup,maxNeedResourceGroup);
				if (flag == 1){
					
					// 分配资源 (释放操作系统的资源) 
					systemResourceGroupAllocate(requestResourceGroup);
					
					// 向进程发出通知，通知申请的资源可以使用了  
					Message message(0,id,OCCUPY_AGREE,requestResourceGroup);
					sendMessage(message); 
					
					
					printf("操作系统同意了 [ 进程(pid: %d) ] 的资源申请请求!\n",id);
					
					messageDeleteFlag = 1; 
					
				}else{
					// 向进程发出通知，通知资源申请被拒绝 
					Message message(0,id,OCCUPY_DISAGREE,requestResourceGroup);
					sendMessage(message); 
				
					printf("操作系统拒绝了 [ 进程(pid: %d) ] 的资源申请请求!  (错误代号: %d)\n",id,flag);
				}
				
			}
			
			// 表示进程请求释放资源 
			if (it->getInstruction() == ASKFORRELEASE){
				int id = it->getSenderPid();
				int* requestResourceGroup = it->getResourceGroup();
				
				
				// 释放资源 
				systemResourceGroupRecover(requestResourceGroup);
				
				// 向进程发出通知，通知资源释放成功 
				Message message(0,id,RELEASE_AGREE,requestResourceGroup);
				sendMessage(message); 
				
				printf("操作系统成功释放了 [ 进程(pid: %d) ] 的部分资源!\n",id);
				
				messageDeleteFlag = 1; 
			}
			
			// 表示进程要退出了 
			if (it->getInstruction() == ASKFOREXIT){
				int id = it->getSenderPid();
				int flag = deleteProcess(id);
				
				if (flag==1){
					printf("操作系统成功删除 [ 进程(pid: %d) ]，并回收了它的所有资源!\n",id);
					messageDeleteFlag = 1; 
				}else{
					printf("操作系统无法删除 [ 进程(pid: %d) ] !  (错误代号: %d)\n",id,flag);
				}
			}
			
			if (messageDeleteFlag==1){
				systemMessageReceiveQueue.erase(it);   // 同意申请则删除该消息，不同意不删除，推迟到其他时间点处理 
			}else{
				it++;
			}
		}	 
	}
	
	
	// 系统资源回收
	void systemResourceGroupRecover(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			systemResourceGroup[i]+=deltaResourceGroup[i];
		} 
	};
	// 系统资源分配
	void systemResourceGroupAllocate(int* deltaResourceGroup){
		for (int i=0;i<N;i++){
			systemResourceGroup[i]-=deltaResourceGroup[i];
		} 
	};
	
	// 展示所有信息 (系统 + 进程的) 
	void showAllInfo(){
		showSystemInfo();
		showAllProcessInfo(); 
	}
	
	// 展示系统的信息 (包括展示进程的信息) 
	void showSystemInfo(){
		printf("**************** 操作系统信息 ****************\n");
		printf("* 	队列：	(%d个队列)\n",2);
		printf("*		系统消息队列长度：%d\n",messageQueue.size());
		printf("*		进程队列长度：%d\n",processQueue.size());
		printf("* 	资源:	(%d类资源)\n",N);
		Resource::showResourceGroup("*		系统最初拥有的资源: ",this->initialResourceGroup);
		Resource::showResourceGroup("*		系统现在拥有的资源: ",systemResourceGroup);
		printf("**********************************************\n");
	}
	
	// 展示所有进程的信息 
	void showAllProcessInfo(){
		vector<Process>::iterator it;
		for (it = processQueue.begin();it!=processQueue.end();it++){
			it->showInfo();
		}
	}
	
	// 初始化清空 
	void clear(){
		messageQueue.clear();
		processQueue.clear();
		livingPids.clear();
	} 
	// 发送消息 
	void sendMessage(Message message){
		messageQueue.push_back(message);
	};
};


int main(){

	
	OperateSystem operateSystem(INITIAL_PROCESS_COUNT);

	operateSystem.run(); 
} 
