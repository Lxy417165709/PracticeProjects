package commonObjects

import "fmt"

// 帧结构
type Frame struct {
	SrcMAC  string
	DstMAC  string
	Message string
}

// 展示帧
// 输出时，帧消息长度是固定的，为20个字节，不满20个字节的需要向前填充"0"
func (f *Frame)Show() {
	fmt.Println(" -------------------------------------------")
	fmt.Printf("|		源MAC: 		%s		|\n", f.SrcMAC)
	fmt.Printf("|		目的MAC: 	%s		|\n", f.DstMAC)
	fmt.Printf("|		消息内容: 	%020s	|\n", f.Message)
	fmt.Println(" -------------------------------------------")
}