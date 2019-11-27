package commonObjects

import "fmt"

// 网桥转发表
type ForwardingTable struct {
	netMACMap map[string]int // 每一个网络的MAC地址映射，将该MAC映射到它所对应的网络中
	MACSet	[][]string //MACSet[x][y]表示网络号为x的第y个MAC
}

// 展示转发表
func (f *ForwardingTable) Show() {

	netCount:= len(f.MACSet) // 网络数

	// 展示
	fmt.Printf(" - - 转发表 - - - - - - - - - - - - \n")
	fmt.Printf("|网络号			MAC地址				|\n")
	for i:=0;i<netCount;i++{
		for t:=0;t<len(f.MACSet[i]);t++{
			fmt.Printf("|%d			%s		|\n",i,f.MACSet[i][t])
		}
	}
	fmt.Printf(" - - - - - - - - - - - - - - - - - -\n")
}


// 构造函数
// netCount: 与网桥相连的网络数
func NewForwardingTable(netCount int) *ForwardingTable {
	return &ForwardingTable{make(map[string]int),make([][]string,netCount)}
}

// 插入帧
// 第一个参数是MAC地址，第二个参数是其所对应的网络号
func (f *ForwardingTable) Insert(MAC string, netNumber int) {
	// 不存在才插入MACSet
	if f.getNetNumber(MAC) == -1 {
		f.MACSet[netNumber] = append(f.MACSet[netNumber],MAC)
	}

	f.netMACMap[MAC] = netNumber

}

// 查询MAC属于哪个网络，-1 表示还没存在
func (f *ForwardingTable) getNetNumber(MAC string) int {
	if x, ok := f.netMACMap[MAC]; ok {
		return x
	}
	return -1
}

// 判断是否转发
// 第一个参数是目的MAC地址，第二个参数是发送者的网络号
// 返回值: -2 表示在同一网络,-1 表示转发表没有该dstMAC, 其他值表示的是所对应的网络号
func (f *ForwardingTable) IsToForward(dstMAC string, netNumber int) int {

	number := f.getNetNumber(dstMAC)

	// 这说明在同一网络，那么就不转发
	if number == netNumber {
		return -2
	}
	// 这说明dstMAC不存在于转发表，也不转发
	if number == -1 {
		return -1
	}

	// 这说明不在同一网络，则转发
	return number
}
