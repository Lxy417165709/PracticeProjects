# include<stdio.h>
# include<stdlib.h>
# include<vector>
# include<time.h>
# include<iostream>
# include<map>
# include<queue>
# include<set>
using namespace std;

// 返回[low,high]区间的随机数 
int getRandom(int low,int high){
	return rand()%(high-low+1) + low; 
}

// 获取随机指令集 
vector<int> getRandomCommands(){
	vector<int> commands;
	srand((int)time(0));
	int randNum;
	// 106 * 3 + 2 == 320
	for (int i=1;i<=106;i++){
		randNum = getRandom(0,319);
		commands.push_back(randNum);
		randNum = getRandom(0,randNum);
		commands.push_back(randNum);
		randNum = getRandom(randNum+1,319);
		commands.push_back(randNum);
	}
	randNum = getRandom(0,319);
	commands.push_back(randNum);
	randNum = getRandom(0,randNum);
	commands.push_back(randNum);
	return commands;
}

// 获取顺序指令集
vector<int> getOrderCommands(){
	vector<int> commands;
	for (int i=0;i<=319;i++){
		commands.push_back(i);
	} 
	return commands;
}

// 获取随机指令集 
vector<int> getRandomCommands2(){
	vector<int> commands;
	for (int i=0;i<=319;i++){
		commands.push_back(getRandom(0,319));
	} 
	return commands;
}

// 指令集转页面集
vector<int> commandsToPages(vector<int> commands){
	vector<int> pages;
	for (int i=0;i<commands.size();i++){
		pages.push_back(commands[i]/10);
	}
	return pages;
}

// 展示vector 
void showVector(vector<int> x,int countPerLine){
	for (int i=1;i<=x.size();i++){
		cout<<x[i-1]<<"\t";
		if (i%countPerLine==0){
			cout<<endl;
		}
	}
	if (x.size()%countPerLine!=0){
		cout<<endl;
	}
}



// FIFO算法，返回页面导入次数
// pages: 页面序列
// memoryCapacity: 内存所能容纳的页数 
int FIFO(vector<int> pages,int memoryCapacity){
	map<int,bool> isLiving; // 用于记录页是否在内存中
	queue<int> memoryQueue;	// 用于存放当前内存中的页面
	int times = 0;	// 用于记录页面导入次数
	
	for (int i=0; i<pages.size();i++){
		//printf("第 %d 轮: ",i);
		
		// 如果该页不在内存中 
		if (isLiving[pages[i]]==false){
			
			// 如果内存没有空闲位置 
			if (memoryQueue.size() >= memoryCapacity){
				int oldestPage = memoryQueue.front();
				memoryQueue.pop();
				isLiving[oldestPage]=false;
				//printf("第 %d 页被置换出外存，",oldestPage);
			}
			memoryQueue.push(pages[i]);
			isLiving[pages[i]]=true ;
			times++;
			//printf("第 %d 页换入内存。\n",pages[i]);
		}else{
			//printf("第 %d 页已经存在于内存中。\n",pages[i]);
		}
		
	}
	return times;
	 
} 
 


// 双向链表节点 
struct Node {
	int page;
	Node* pre;
	Node* next;

	Node(int page=-1,Node* pre=NULL,Node* next=NULL){
		this->page = page;
		this->pre = pre;
		this->next = next;
	}
	
};


// 双向链表
class DoubleList{
	Node* dummyHead;	// 哑头
	Node* dummyTail;	// 哑尾
	int size;			// 链表长度 
	public: DoubleList(){
		dummyHead = new Node();
		dummyTail = new Node();
		dummyHead->next = dummyTail;
		dummyTail->pre = dummyHead;
		this->size = 0;
	}
	public: int length(){
		return this->size;
	} 
	// 插入节点 
	public: void push(Node* node){
		// Node newNode(page,dummyTail->pre,dummyTail);
		node->pre  = dummyTail->pre;
		node->next = dummyTail; 
		dummyTail->pre->next = node;
		dummyTail->pre = node;
		size++;
	}
	// 删除节点 
	public: void del(Node* node){
		node->next->pre = node->pre;
		node->pre->next = node->next; 
		size--;
	}
	
	// 更新节点
	public: void update(Node* node){
		this->del(node);
		this->push(node);
	}
	
	// 删除头节点并返回 (链表非空才可以) 
	public: Node* pop(){
		Node* node = dummyHead->next;
		dummyHead->next =  dummyHead->next->next;
		dummyHead->next->pre = dummyHead;
		size--;
		return node;
	}
	
	// 展示链表数据 
	public: void show(){
		Node* tmp = dummyHead->next;
		while (tmp!=dummyTail){
			printf("%d ",tmp->page);
			tmp = tmp->next;
		}
		printf("\n");
	}
	
}; 



// LRU算法，返回页面导入次数
// pages: 页面序列
// memoryCapacity: 内存所能容纳的页数 
int LRU(vector<int> pages,int memoryCapacity){
	DoubleList doubleList;	// 双向链表，用于存放内存页面
	map<int,Node*> pageToNode; // 用于存放页面与双向链表节点的映射关系 (可以O(1)时间内找出page的位置) 
	int times = 0;	// 用于记录页面导入次数
	for(int i=0;i<pages.size();i++){
		// 如果内存不存在该页面
		if (pageToNode[pages[i]]==NULL){
			// 如果内存已满
			if (doubleList.length()==memoryCapacity){
				Node* head = doubleList.pop();
				pageToNode[head->page]=NULL;
			}
			
			Node* newNode = new Node(pages[i]);
			doubleList.push(newNode);
			pageToNode[pages[i]]=newNode;
			
			times++;
		}else{
			doubleList.update(pageToNode[pages[i]]);
		}
		
//		printf("%d: ",i);
//		doubleList.show();
//		printf("\n");
	}
	
	return times;
} 

const int inf = 1000000000;	// 定义无穷大 
struct PageUnit{
	int page;
	int lastPosition;
	PageUnit(int page=-1,int lastPosition=inf){
		this->page = page;
		this->lastPosition = lastPosition;
	}
	
	bool operator < (const PageUnit& x) const{
		return x.lastPosition < lastPosition;
	} 
	
}; 

// 展示set
void showSet(set<PageUnit> x,int countPerLine){
	
	set<PageUnit>::iterator it = x.begin();
	
	for (int i=1;it!=x.end();it++,i++){
		cout<<it->page<<"\t";
		if (i%countPerLine==0){
			cout<<endl;
		}
	}
	if (x.size()%countPerLine!=0){
		cout<<endl;
	}
} 

const int off = 40;	// ＞=32就可以了，因为页面才32张，且序号区间为[0,31] 

// OPT算法，返回页面导入次数
// pages: 页面序列
// memoryCapacity: 内存所能容纳的页数 
int OPT(vector<int> pages,int memoryCapacity){
	set<PageUnit> memorySet;		// 模拟内存，用于存放页面 
	map<int,int> lastPosition;		// 存放页面x在位置i，最后最近的一个页面位置
	map<int,int> tmpMap;			// 存放页面x最后最近的一个页面位置
	int times = 0;					// 页面换入次数 
	// 创建映射 
	for (int i=pages.size()-1;i>=0;i--){
		if (tmpMap[pages[i]]==0){
			lastPosition[i*off + pages[i]] = inf;
			tmpMap[pages[i]] = i;
		}else{
			lastPosition[i*off + pages[i]] = tmpMap[pages[i]];
			tmpMap[pages[i]] = i;
		}
	}
	
	// 实施算法 
	for(int i=0;i<pages.size();i++){
		
		PageUnit tmp(-1,i);	// 这里只有i是有用的 
		set<PageUnit>::iterator it = memorySet.find(tmp); // 之所以找i,是因为如果该页面存在，那么它的下一个位置一定是i
		
		// 页面不存在
		if (it==memorySet.end()){
			
			// 如果内存已满
			if (memorySet.size() == memoryCapacity){
				memorySet.erase(memorySet.begin());	// 删除最久未使用的 
			}
			times++;
		}else{
			memorySet.erase(it);
		}

		PageUnit pageUnit(pages[i],lastPosition[i*off + pages[i]]);
		memorySet.insert(pageUnit);
		
	} 
	 
	
	return times;
}

int main(){
	vector<int> commands = getRandomCommands();
	
	vector<int> pages = commandsToPages(commands);
	printf("页面序列:\n"); 
	showVector(pages,15);
	printf("------------------------\n");
	printf("内存大小\tFIFO\tLRU\tOPT\n"); 
	for (int i=1;i<=32;i++){
		printf("%d\t\t%d\t%d\t%d\n",i,FIFO(pages,i),LRU(pages,i),OPT(pages,i));
	}
} 
