# include<stdio.h>
# include<algorithm>
# include<vector>
# include<time.h>
# include<iostream>
# include<map> 
using namespace std;



// ����[low,high]���������� 
int getRandom(int low, int high){
	return rand()%(high-low+1) + low; 
}

// ������ 
class Disk{
	vector<int> trackSequence;		// �ŵ�����
	int nowPointer;					// ��ǰ��ͷ���ڴŵ� 
	int direction;					//  direction == 0 ʱ��ʾ��ͷ�ƶ�����Ϊ�󣬷���Ϊ�ҡ� 
	int processPerTime;				// ÿһ���ܴ�����ٸ��ŵ�
	int trackSum;					// �ܹ��ж��ٸ��ŵ�, �ŵ����Ϊ [1,trackSum] 
	
	
	// ���̳�ʼ������ 
	public: Disk(int nowPointer,int direction,int processPerTime,int trackSum){
		this->nowPointer = nowPointer;
		this->direction = direction;
		this->processPerTime = processPerTime;
		this->trackSum = trackSum;
	}
	
	// ������� processPerTime�� ���ظ��ŵ����� 
	public: void generateTrackSequence(){
		if (processPerTime>trackSum){
			printf("�ô��̻��ˣ�������Ϊÿ�δ���Ĵŵ���(%d)�����ܴŵ���(%d)\n",this->processPerTime,this->trackSum);
			return;
		}
		this->trackSequence.clear();
	
		int* selectTrackNums = new int[trackSum+1];	// һ�����飬��ŵ�ǰ�ɱ�ѡȡ�Ĵŵ��š�
		for (int i=1;i<=trackSum;i++){
			selectTrackNums[i] = i;
		} 
		int l = 1;			// ��ָ�룬ָ��selectTrackNums����߽�
		int r = this->trackSum;	// ��ָ�룬ָ��selectTrackNums���ұ߽� 
		
		
		int count = 0;	// ����������Ԫ�ظ��� 
		while (count < this->processPerTime){
			int selectedPosition = getRandom(l,r);
			trackSequence.push_back(selectTrackNums[selectedPosition]);
			swap(selectTrackNums[selectedPosition],selectTrackNums[r]);
			r--;
			count++;
		}
		printf("\t�ŵ�����������ɣ������� %d ���ŵ��������������ʹ��Ѱ���㷨�ˡ�\n",count);
	}
	
	// ����Ѱ���㷨 --- �����ȷ��� 
	// ������ͷ�ƶ����ܴŵ��� 
	public: int FCFS(){
		if (this->trackSequence.size()==0){
			printf("�ô��̻�û�����ŵ����У������������С� --- from FCFS\n");
			return -1;
		} 
		vector<int> diskHeadMoveSequence = this->trackSequence;	// �����ȷ����㷨�� ��ͷ�ƶ����о��Ǵŵ����С�
		// չʾ�´�ͷ�ƶ�˳�� 
		showDiskHeadMoveSequence(diskHeadMoveSequence); 
		return caculateTrackCount(diskHeadMoveSequence);
	}
	
	// ����Ѱ���㷨 --- ���Ѱ��ʱ�������㷨
	// ������ͷ�ƶ����ܴŵ��� 
	public: int SSTF(){
		if (this->trackSequence.size()==0){
			printf("�ô��̻�û�����ŵ����У������������С� --- from SSTF\n");
			return -1;
		}
		map<int,bool> isVisit;						// ��Ŵŵ������������Ƿ��ѱ����� 
		vector<int> diskHeadMoveSequence;			// ��ͷ�ƶ�˳������ 
		vector<int> sequence = this->trackSequence;	// �������ֹԭtrackSequence���ݱ��޸� 
		sort(sequence.begin(),sequence.end());

		int nextPosition = -1;	// ��ǰ��ͷ���ڴŵ��������е����� 
		
		// �����������ж϶���Ϊ�˻�ȡ��ǰ��ͷ���ڴŵ��������е����� 
		// ˵����ǰ��ͷ�ڴŵ����е���С�ŵ�����߻������С�ŵ� 
		if (this->nowPointer<=sequence[0]){
			nextPosition = 0;
		}
		// ˵����ǰ��ͷ�ڴŵ����е���С�ŵ����ұ� 
		if (this->nowPointer>=sequence[sequence.size()-1]){
			nextPosition = sequence.size()-1;
		}
		
		// ˵����ǰ��ͷ�ڴŵ����е��м� 
		if (this->nowPointer>sequence[0] && this->nowPointer < sequence[sequence.size()-1]){
			int R = lower_bound(sequence.begin(),sequence.end(),this->nowPointer) - sequence.begin();
			int L = R-1;
			nextPosition = getPosition(sequence,L,R,this->nowPointer);
		}
		
		// ����㷨�������Ż���... 
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
		
		// չʾ�´�ͷ�ƶ�˳�� 
		showDiskHeadMoveSequence(diskHeadMoveSequence);
		return caculateTrackCount(diskHeadMoveSequence);
	}
	
	// ��ȡ sequence[l] ��sequence[r] �ĸ���pointer��
	// ����l �� r 
	int getPosition(vector<int> sequence,int l,int r,int pointer){
		if (abs(pointer-sequence[l])>abs(pointer-sequence[r])){
			return r;
		}else{
			return l;
		}
		return -1;
	}
	

	
	// ����Ѱ���㷨 --- ���ݷ� 
	// ������ͷ�ƶ����ܴŵ��� 
	public: int SCAN(){
		if (this->trackSequence.size()==0){
			printf("�ô��̻�û�����ŵ����У������������С� --- from SSTF\n");
			return -1;
		}
		vector<int> diskHeadMoveSequence;			// ��ͷ�ƶ�˳������ 
		vector<int> sequence = this->trackSequence;	// �������ֹԭtrackSequence���ݱ��޸� 
		sort(sequence.begin(),sequence.end());

		int index = -1;	// ��ǰ��ͷ���ڴŵ��������е����� 
		 
		// �����������ж϶���Ϊ�˻�ȡ��ǰ��ͷ���ڴŵ��������е����� 
		// ˵����ǰ��ͷ�ڴŵ����е���С�ŵ�����߻������С�ŵ� 
		if (this->nowPointer<=sequence[0]){
			index = 0;
		}
		// ˵����ǰ��ͷ�ڴŵ����е���С�ŵ����ұ� 
		if (this->nowPointer>=sequence[sequence.size()-1]){
			index = sequence.size()-1;
		}
		
		// ˵����ǰ��ͷ�ڴŵ����е��м� 
		if (this->nowPointer>sequence[0] && this->nowPointer < sequence[sequence.size()-1]){
			int R = upper_bound(sequence.begin(),sequence.end(),this->nowPointer) - sequence.begin();
			index = this->direction==0? R-1 : R;
		}
		diskHeadMoveSequence.push_back(sequence[index]);
		
		// ����ͷ�ƶ�����Ϊ��ʱ 
		if (this->direction==0){
			// ����ɨ 
			for (int i=index-1;i>=0;i--){
				diskHeadMoveSequence.push_back(sequence[i]);
			}
			// ������ɨ 
			for (int i=index+1;i<sequence.size();i++){
				diskHeadMoveSequence.push_back(sequence[i]);
			}
		}else{
			// ����ɨ 
			for (int i=index+1;i<sequence.size();i++){
				diskHeadMoveSequence.push_back(sequence[i]);
			}
			// ������ɨ 
			for (int i=index-1;i>=0;i--){
				diskHeadMoveSequence.push_back(sequence[i]);
			}
		}
		
		
		// չʾ�´�ͷ�ƶ�˳�� 
		showDiskHeadMoveSequence(diskHeadMoveSequence);
		return caculateTrackCount(diskHeadMoveSequence);
	}
	
	// ������ͷ�ƶ����ܴŵ���  
	int caculateTrackCount(vector<int> diskHeadMoveSequence){
		int pointer = this->nowPointer;	
		int trackCount = 0;	
		
		// �㷨˼��: ֱ��ģ�� 
		for (int i=0;i < diskHeadMoveSequence.size();i++){
			trackCount += abs(diskHeadMoveSequence[i]-pointer);
			pointer = diskHeadMoveSequence[i]; 
		}
		return trackCount; 
	};	
	
	// �����ͷ�ƶ����� 
	void showDiskHeadMoveSequence(vector<int> diskHeadMoveSequence){
		
		printf("\n\t%d(��ʼ��ͷλ��)\n",this->nowPointer);
		
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
	int nowPointer = 200;			// ��ǰ��ͷ���ڴŵ� 
	int direction = 1;				// direction == 0 ʱ��ʾ��ͷ�ƶ�����Ϊ�󣬷���Ϊ�ҡ� 
	int processPerTime = 500;		// ÿһ���ܴ�����ٸ��ŵ�
	int trackSum = 1000;			// �ܹ��ж��ٸ��ŵ�, �ŵ����Ϊ [1,trackSum] 
	
	printf("\n");
	
	Disk disk(nowPointer, direction, processPerTime, trackSum);
	printf("\t::���̳�ʼ����ɡ�\n");
	printf("\t\t��ͷλ��: �� %d �ŵ�\n",nowPointer);
	printf("\t\t��ͷ�ƶ�����: ���� \n");
	printf("\t\t����ŵ�Ԫ�ظ���: %d \n",processPerTime);
	printf("\t\t�ŵ���Χ: [1, %d] \n",trackSum);
	
	printf("\n");
	printf("\t��ʼ��������ŵ�����...\n\n");
	disk.generateTrackSequence();
	printf("		ʹ��FCFS�㷨����ͷ�ƶ��ŵ���Ϊ: %d\n\n",disk.FCFS());
	printf("		ʹ��SSTF�㷨����ͷ�ƶ��ŵ���Ϊ: %d\n\n",disk.SSTF());
	printf("		ʹ��SCAN�㷨����ͷ�ƶ��ŵ���Ϊ: %d\n\n",disk.SCAN());

} 
