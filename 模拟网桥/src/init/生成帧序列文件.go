package Generate

import (
	"fmt"
	"math/rand"
	"os"
	"time"
)


// 生成帧序列文件，文件名为: Net-<网络号>.txt (在当前目录下)
// count: 帧序列文件个数
// hostPerNet: 每个网络拥有的主机数
// frameCount: 每一个网络产生多少个帧
// messageMinLen: 消息最小长度
// messageMaxLen: 消息最大长度
// fileNameFormatter: 文件名字的规范
func CreateFrameSeqFile(count int,hostPerNet int,frameCount int,messageMinLen int,messageMaxLen int,fileNameFormatter string) (err error){

	// 随机种子
	rand.Seed(time.Now().Unix())

	// 生成count个网段的MAC地址集合
	netMACSets := make([][]string,count)
	for i:=0;i<count;i++{
		netMACSets[i] = createMACSet(hostPerNet)
	}


	// 所有网段总的MAC地址集合
	allMACSet := make([]string,0)
	for i:=0;i<count;i++{
		for t:=0;t<len(netMACSets[i]);t++{
			allMACSet = append(allMACSet,netMACSets[i][t])
		}
	}

	// 生成count个网段的随机帧集合(100个)
	frameSets := make([][]string,count)
	for i:=0;i<count;i++{
		frameSets[i] = createFrameSet(netMACSets[i],allMACSet,frameCount,messageMinLen,messageMaxLen)
	}


	// 创建文件，并把帧写到文件
	// 文件名为: Net-<网络号>.txt
	for i:=0;i<count;i++{
		err = frameSetToFile(fmt.Sprintf(fileNameFormatter,i),frameSets[i])
		if err != nil{
			return err
		}
	}

	return nil
}


// 把帧序列写到文件
func frameSetToFile(fileName string,frameSet []string) error {
	file,err := os.Create(fileName)
	if err != nil{
		return err
	}
	defer func() {
		file.Close()
	}()

	for i:=0;i<len(frameSet);i++{
		_, err := file.WriteString(frameSet[i] + "\n")
		if err!=nil{
			return err
		}
	}
	return nil
}




// 创建随机MAC地址(格式化过的)
func createMAC() string{
	mac := rand.Int63n(1<<48)
	MAC := fmt.Sprintf("%012X",mac)

	// 接下来是为了格式化MAC，让它更好看(为了锻炼算法，我把他写复杂了一点)
	bytes:=[]byte(MAC)
	for i:=0;i<5;i++{
		bytes = append(bytes,' ')
	}

	write,read:=len(bytes)-1,len(MAC)-1
	for read>0 && write>0{
		bytes[write] = bytes[read]
		write--
		read--
		bytes[write] = bytes[read]
		write--
		read--
		if write > 0{
			bytes[write] = '-'
		}
		write--
	}
	return string(bytes)
}

// 创建随机消息(消息内容是一串01比特流，长度[minLen,maxLen])
func createMessage(minLen,maxLen int) string{
	length:=rand.Int()%(minLen+1)+(maxLen-minLen)

	bytes := make([]byte,0)
	for i:=0;i<length;i++{
		bytes = append(bytes,'0' + uint8(rand.Intn(2)))
	}
	return string(bytes)
}

// 创建从secMACSet到desMACSet的帧(每一字段以空格分开)
func createFrame(srcMACSet []string,desMACSet []string,minLen int,maxLen int) string{

	srcMAC := srcMACSet[rand.Intn(len(srcMACSet))]
	desMAC := desMACSet[rand.Intn(len(desMACSet))]
	return srcMAC + " " + desMAC + " " + createMessage(minLen,maxLen)
}

// 创建帧集合
// n为帧个数
func createFrameSet(srcMACSet []string,desMACSet []string,n int,minLen int,maxLen int) []string{
	frameSet := make([]string,0)
	for i:=0;i<n;i++{
		frameSet = append(frameSet,createFrame(srcMACSet,desMACSet,minLen,maxLen))
	}
	return frameSet
}

// 创建MAC地址集合
// n 为集合元素个数
func createMACSet(n int) []string{
	set:=make([]string,0)
	for i:=0;i<n;i++{
		set = append(set,createMAC())
	}
	return set
}