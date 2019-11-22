# include<stdio.h>
# include<stdlib.h>
# include<vector>
# include<time.h>
# include<iostream>
# include<map>
# include<queue>
# include<set>
using namespace std;

// ����[low,high]���������� 
int getRandom(int low,int high){
	return rand()%(high-low+1) + low; 
}

// ��ȡ���ָ� 
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

// ��ȡ˳��ָ�
vector<int> getOrderCommands(){
	vector<int> commands;
	for (int i=0;i<=319;i++){
		commands.push_back(i);
	} 
	return commands;
}

// ��ȡ���ָ� 
vector<int> getRandomCommands2(){
	vector<int> commands;
	for (int i=0;i<=319;i++){
		commands.push_back(getRandom(0,319));
	} 
	return commands;
}

// ָ�תҳ�漯
vector<int> commandsToPages(vector<int> commands){
	vector<int> pages;
	for (int i=0;i<commands.size();i++){
		pages.push_back(commands[i]/10);
	}
	return pages;
}

// չʾvector 
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



// FIFO�㷨������ҳ�浼�����
// pages: ҳ������
// memoryCapacity: �ڴ��������ɵ�ҳ�� 
int FIFO(vector<int> pages,int memoryCapacity){
	map<int,bool> isLiving; // ���ڼ�¼ҳ�Ƿ����ڴ���
	queue<int> memoryQueue;	// ���ڴ�ŵ�ǰ�ڴ��е�ҳ��
	int times = 0;	// ���ڼ�¼ҳ�浼�����
	
	for (int i=0; i<pages.size();i++){
		//printf("�� %d ��: ",i);
		
		// �����ҳ�����ڴ��� 
		if (isLiving[pages[i]]==false){
			
			// ����ڴ�û�п���λ�� 
			if (memoryQueue.size() >= memoryCapacity){
				int oldestPage = memoryQueue.front();
				memoryQueue.pop();
				isLiving[oldestPage]=false;
				//printf("�� %d ҳ���û�����棬",oldestPage);
			}
			memoryQueue.push(pages[i]);
			isLiving[pages[i]]=true ;
			times++;
			//printf("�� %d ҳ�����ڴ档\n",pages[i]);
		}else{
			//printf("�� %d ҳ�Ѿ��������ڴ��С�\n",pages[i]);
		}
		
	}
	return times;
	 
} 
 


// ˫������ڵ� 
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


// ˫������
class DoubleList{
	Node* dummyHead;	// ��ͷ
	Node* dummyTail;	// ��β
	int size;			// ������ 
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
	// ����ڵ� 
	public: void push(Node* node){
		// Node newNode(page,dummyTail->pre,dummyTail);
		node->pre  = dummyTail->pre;
		node->next = dummyTail; 
		dummyTail->pre->next = node;
		dummyTail->pre = node;
		size++;
	}
	// ɾ���ڵ� 
	public: void del(Node* node){
		node->next->pre = node->pre;
		node->pre->next = node->next; 
		size--;
	}
	
	// ���½ڵ�
	public: void update(Node* node){
		this->del(node);
		this->push(node);
	}
	
	// ɾ��ͷ�ڵ㲢���� (����ǿղſ���) 
	public: Node* pop(){
		Node* node = dummyHead->next;
		dummyHead->next =  dummyHead->next->next;
		dummyHead->next->pre = dummyHead;
		size--;
		return node;
	}
	
	// չʾ�������� 
	public: void show(){
		Node* tmp = dummyHead->next;
		while (tmp!=dummyTail){
			printf("%d ",tmp->page);
			tmp = tmp->next;
		}
		printf("\n");
	}
	
}; 



// LRU�㷨������ҳ�浼�����
// pages: ҳ������
// memoryCapacity: �ڴ��������ɵ�ҳ�� 
int LRU(vector<int> pages,int memoryCapacity){
	DoubleList doubleList;	// ˫���������ڴ���ڴ�ҳ��
	map<int,Node*> pageToNode; // ���ڴ��ҳ����˫������ڵ��ӳ���ϵ (����O(1)ʱ�����ҳ�page��λ��) 
	int times = 0;	// ���ڼ�¼ҳ�浼�����
	for(int i=0;i<pages.size();i++){
		// ����ڴ治���ڸ�ҳ��
		if (pageToNode[pages[i]]==NULL){
			// ����ڴ�����
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

const int inf = 1000000000;	// ��������� 
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

// չʾset
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

const int off = 40;	// ��=32�Ϳ����ˣ���Ϊҳ���32�ţ����������Ϊ[0,31] 

// OPT�㷨������ҳ�浼�����
// pages: ҳ������
// memoryCapacity: �ڴ��������ɵ�ҳ�� 
int OPT(vector<int> pages,int memoryCapacity){
	set<PageUnit> memorySet;		// ģ���ڴ棬���ڴ��ҳ�� 
	map<int,int> lastPosition;		// ���ҳ��x��λ��i����������һ��ҳ��λ��
	map<int,int> tmpMap;			// ���ҳ��x��������һ��ҳ��λ��
	int times = 0;					// ҳ�滻����� 
	// ����ӳ�� 
	for (int i=pages.size()-1;i>=0;i--){
		if (tmpMap[pages[i]]==0){
			lastPosition[i*off + pages[i]] = inf;
			tmpMap[pages[i]] = i;
		}else{
			lastPosition[i*off + pages[i]] = tmpMap[pages[i]];
			tmpMap[pages[i]] = i;
		}
	}
	
	// ʵʩ�㷨 
	for(int i=0;i<pages.size();i++){
		
		PageUnit tmp(-1,i);	// ����ֻ��i�����õ� 
		set<PageUnit>::iterator it = memorySet.find(tmp); // ֮������i,����Ϊ�����ҳ����ڣ���ô������һ��λ��һ����i
		
		// ҳ�治����
		if (it==memorySet.end()){
			
			// ����ڴ�����
			if (memorySet.size() == memoryCapacity){
				memorySet.erase(memorySet.begin());	// ɾ�����δʹ�õ� 
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
	printf("ҳ������:\n"); 
	showVector(pages,15);
	printf("------------------------\n");
	printf("�ڴ��С\tFIFO\tLRU\tOPT\n"); 
	for (int i=1;i<=32;i++){
		printf("%d\t\t%d\t%d\t%d\n",i,FIFO(pages,i),LRU(pages,i),OPT(pages,i));
	}
} 
