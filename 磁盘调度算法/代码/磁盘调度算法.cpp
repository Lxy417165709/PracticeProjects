# include<stdio.h>
# include<algorithm>
# include<vector>
# include<time.h>
# include<iostream>
# include<map> 
using namespace std;



// 返回[low,high]区间的随机数 
int getRandom(int low, int high){
	return rand()%(high-low+1) + low; 
}

// 磁盘类 
class Disk{
	vector<int> trackSequence;		// 磁道序列
	int nowPointer;					// 当前柱头所在磁道 
	int direction;					//  direction == 0 时表示磁头移动方向为左，否则为右。 
	int processPerTime;				// 每一次能处理多少个磁道
	int trackSum;					// 总共有多少个磁道, 磁道编号为 [1,trackSum] 
	
	
	// 磁盘初始化函数 
	public: Disk(int nowPointer,int direction,int processPerTime,int trackSum){
		this->nowPointer = nowPointer;
		this->direction = direction;
		this->processPerTime = processPerTime;
		this->trackSum = trackSum;
	}
	
	// 随机生成 processPerTime个 不重复磁道序列 
	public: void generateTrackSequence(){
		if (processPerTime>trackSum){
			printf("该磁盘坏了！！！因为每次处理的磁道数(%d)大于总磁道数(%d)\n",this->processPerTime,this->trackSum);
			return;
		}
		this->trackSequence.clear();
	
		int* selectTrackNums = new int[trackSum+1];	// 一个数组，存放当前可被选取的磁道号。
		for (int i=1;i<=trackSum;i++){
			selectTrackNums[i] = i;
		} 
		int l = 1;			// 左指针，指向selectTrackNums的左边界
		int r = this->trackSum;	// 右指针，指向selectTrackNums的右边界 
		
		
		int count = 0;	// 已生成序列元素个数 
		while (count < this->processPerTime){
			int selectedPosition = getRandom(l,r);
			trackSequence.push_back(selectTrackNums[selectedPosition]);
			swap(selectTrackNums[selectedPosition],selectTrackNums[r]);
			r--;
			count++;
		}
		printf("\t磁道序列生成完成，共产生 %d 个磁道。接下来你可以使用寻道算法了。\n",count);
	}
	
	// 磁盘寻道算法 --- 先来先服务 
	// 返回柱头移动的总磁道数 
	public: int FCFS(){
		if (this->trackSequence.size()==0){
			printf("该磁盘还没产生磁道序列，请先生成序列。 --- from FCFS\n");
			return -1;
		} 
		vector<int> diskHeadMoveSequence = this->trackSequence;	// 先来先服务算法的 磁头移动序列就是磁道序列。
		// 展示下磁头移动顺序 
		showDiskHeadMoveSequence(diskHeadMoveSequence); 
		return caculateTrackCount(diskHeadMoveSequence);
	}
	
	// 磁盘寻道算法 --- 最短寻道时间优先算法
	// 返回柱头移动的总磁道数 
	public: int SSTF(){
		if (this->trackSequence.size()==0){
			printf("该磁盘还没产生磁道序列，请先生成序列。 --- from SSTF\n");
			return -1;
		}
		map<int,bool> isVisit;						// 存放磁道序列索引的是否已被访问 
		vector<int> diskHeadMoveSequence;			// 磁头移动顺序序列 
		vector<int> sequence = this->trackSequence;	// 深拷贝，防止原trackSequence内容被修改 
		sort(sequence.begin(),sequence.end());

		int nextPosition = -1;	// 当前磁头所在磁道在序列中的索引 
		
		// 接下来三个判断都是为了获取当前磁头所在磁道在序列中的索引 
		// 说明当前磁头在磁道序列的最小磁道的左边或就是最小磁道 
		if (this->nowPointer<=sequence[0]){
			nextPosition = 0;
		}
		// 说明当前磁头在磁道序列的最小磁道的右边 
		if (this->nowPointer>=sequence[sequence.size()-1]){
			nextPosition = sequence.size()-1;
		}
		
		// 说明当前磁头在磁道序列的中间 
		if (this->nowPointer>sequence[0] && this->nowPointer < sequence[sequence.size()-1]){
			int R = lower_bound(sequence.begin(),sequence.end(),this->nowPointer) - sequence.begin();
			int L = R-1;
			nextPosition = getPosition(sequence,L,R,this->nowPointer);
		}
		
		// 这个算法还可以优化的... 
		int count = 0;
		diskHeadMoveSequence.push_back(sequence[nextPosition]);
		isVisit[nextPosition] = true;
		count++;
		while(count < this->processPerTime){
			int l = nextPosition;
			int r = nextPosition;
			while((l<0 || isVisit[l]==true) && (r>=this->processPerTime || isVisit[r]==true)){
				l--;
				r++;
			}
			if ((l>=0 && isVisit[l]==false) && (r<this->processPerTime && isVisit[r]==false)){
				nextPosition = getPosition(sequence,l,r,sequence[nextPosition]);
				
			}else{
				if (l>=0 && isVisit[l]==false){
					nextPosition = l;
				}else{
					nextPosition = r;
				}
			}
			diskHeadMoveSequence.push_back(sequence[nextPosition]);
			isVisit[nextPosition] = true;
			count++;
		}
		
		// 展示下磁头移动顺序 
		showDiskHeadMoveSequence(diskHeadMoveSequence);
		return caculateTrackCount(diskHeadMoveSequence);
	}
	
	// 获取 sequence[l] 、sequence[r] 哪个离pointer近
	// 返回l 或 r 
	int getPosition(vector<int> sequence,int l,int r,int pointer){
		if (abs(pointer-sequence[l])>abs(pointer-sequence[r])){
			return r;
		}else{
			return l;
		}
		return -1;
	}
	

	
	// 磁盘寻道算法 --- 电梯法 
	// 返回柱头移动的总磁道数 
	public: int SCAN(){
		if (this->trackSequence.size()==0){
			printf("该磁盘还没产生磁道序列，请先生成序列。 --- from SSTF\n");
			return -1;
		}
		vector<int> diskHeadMoveSequence;			// 磁头移动顺序序列 
		vector<int> sequence = this->trackSequence;	// 深拷贝，防止原trackSequence内容被修改 
		sort(sequence.begin(),sequence.end());

		int index = -1;	// 当前磁头所在磁道在序列中的索引 
		 
		// 接下来三个判断都是为了获取当前磁头所在磁道在序列中的索引 
		// 说明当前磁头在磁道序列的最小磁道的左边或就是最小磁道 
		if (this->nowPointer<=sequence[0]){
			index = 0;
		}
		// 说明当前磁头在磁道序列的最小磁道的右边 
		if (this->nowPointer>=sequence[sequence.size()-1]){
			index = sequence.size()-1;
		}
		
		// 说明当前磁头在磁道序列的中间 
		if (this->nowPointer>sequence[0] && this->nowPointer < sequence[sequence.size()-1]){
			int R = upper_bound(sequence.begin(),sequence.end(),this->nowPointer) - sequence.begin();
			index = this->direction==0? R-1 : R;
		}
		diskHeadMoveSequence.push_back(sequence[index]);
		
		// 当磁头移动方向为左时 
		if (this->direction==0){
			// 向左扫 
			for (int i=index-1;i>=0;i--){
				diskHeadMoveSequence.push_back(sequence[i]);
			}
			// 再向右扫 
			for (int i=index+1;i<sequence.size();i++){
				diskHeadMoveSequence.push_back(sequence[i]);
			}
		}else{
			// 向右扫 
			for (int i=index+1;i<sequence.size();i++){
				diskHeadMoveSequence.push_back(sequence[i]);
			}
			// 再向左扫 
			for (int i=index-1;i>=0;i--){
				diskHeadMoveSequence.push_back(sequence[i]);
			}
		}
		
		
		// 展示下磁头移动顺序 
		showDiskHeadMoveSequence(diskHeadMoveSequence);
		return caculateTrackCount(diskHeadMoveSequence);
	}
	
	// 计算柱头移动的总磁道数  
	int caculateTrackCount(vector<int> diskHeadMoveSequence){
		int pointer = this->nowPointer;	
		int trackCount = 0;	
		
		// 算法思想: 直接模拟 
		for (int i=0;i < diskHeadMoveSequence.size();i++){
			trackCount += abs(diskHeadMoveSequence[i]-pointer);
			pointer = diskHeadMoveSequence[i]; 
		}
		return trackCount; 
	};	
	
	// 输出磁头移动序列 
	void showDiskHeadMoveSequence(vector<int> diskHeadMoveSequence){
		
		printf("\n\t%d(初始磁头位置)\n",this->nowPointer);
		
		for (int i=1;i <= diskHeadMoveSequence.size();i++){
			printf("\t->\t%d",diskHeadMoveSequence[i-1]);
			if (i%10==0){
				printf("\n");
			}
		}
		if ((diskHeadMoveSequence.size())%10!=0){
			printf("\n");
		}
		printf("\n");
	};	
							
	
	
};

int main(){
	srand((int)time(0));
	int nowPointer = 200;			// 当前柱头所在磁道 
	int direction = 1;				// direction == 0 时表示磁头移动方向为左，否则为右。 
	int processPerTime = 500;		// 每一次能处理多少个磁道
	int trackSum = 1000;			// 总共有多少个磁道, 磁道编号为 [1,trackSum] 
	
	printf("\n");
	
	Disk disk(nowPointer, direction, processPerTime, trackSum);
	printf("\t::磁盘初始化完成。\n");
	printf("\t\t磁头位置: 第 %d 磁道\n",nowPointer);
	printf("\t\t磁头移动方向: 向右 \n");
	printf("\t\t处理磁道元素个数: %d \n",processPerTime);
	printf("\t\t磁道范围: [1, %d] \n",trackSum);
	
	printf("\n");
	printf("\t开始生成随机磁道序列...\n\n");
	disk.generateTrackSequence();
	printf("		使用FCFS算法，磁头移动磁道数为: %d\n\n",disk.FCFS());
	printf("		使用SSTF算法，磁头移动磁道数为: %d\n\n",disk.SSTF());
	printf("		使用SCAN算法，磁头移动磁道数为: %d\n\n",disk.SCAN());

} 
