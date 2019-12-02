package main

import (
	"commonObjects"
	"fmt"
	"init"
)

const (
	netCount = 5 	// 网桥连接的网络数
	HostPerNet = 5		// 每一个网络拥有的MAC地址(即主机数)
	frameCount = 10	// 每一个网络发送的帧的数量
	messageMinLen = 10	// 消息的最短长度
	messageMaxLen = 20	// 消息的最长长度
	fileNameFormatter = "Net-%d.txt" 	// 文件名字的规范
)

func main() {
	// 生成帧序列文件 (文件名为Net-<网络号>.txt)
	err := Generate.CreateFrameSeqFile(netCount,HostPerNet,frameCount,messageMinLen,messageMaxLen,fileNameFormatter)
	if err != nil {
		fmt.Println("序列文件创建失败", err)
		return
	}

	// 帧序列
	frameSeqs := make([][]commonObjects.Frame, 0)

	// 开始从文件读取帧序列，并保留在帧集合中
	for i:=0;i<netCount;i++{
		frameSeq, err := Generate.ReadFrameSet(fmt.Sprintf("Net-%d.txt",i))
		if err != nil {
			fmt.Println(err)
			return
		}
		frameSeqs = append(frameSeqs, frameSeq)
	}



	// 开始模拟网桥

	// 创建空白转发表
	forwardingTable := commonObjects.NewForwardingTable(netCount)
	for i := 0; i < frameCount; i++ {
		fmt.Printf("\n第 %d 轮: \n",i)
		for t := 0; t < netCount; t++ {

			// 源MAC的网络号我们是知道的
			//  (这里的网络号不是IP地址网络号，而是我自己取的，从0开始..0表示第一个网络,1表示第二个，以此类推)
			forwardingTable.Insert(frameSeqs[t][i].SrcMAC, t)

			// 是否转发
			isToForwardFlag := forwardingTable.IsToForward(frameSeqs[t][i].DstMAC, t)

			if isToForwardFlag == -2 {
				fmt.Printf("::	网络%d发送了一个到自己网络的帧。		(网桥不转发)\n", t)
			}
			if isToForwardFlag == -1 {
				fmt.Printf("::	网络%d发送了一个到达未知网络的帧。		(网桥转发)\n", t)
			}
			if isToForwardFlag != -2 && isToForwardFlag != -1{
				fmt.Printf("::	网络%d发送了一个到达网络%d的帧。		(网桥转发)\n", t, isToForwardFlag)
			}
			frameSeqs[t][i].Show()
		}
		forwardingTable.Show()
	}
}


