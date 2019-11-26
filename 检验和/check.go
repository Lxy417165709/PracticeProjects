package main

import (
	"fmt"
	"io"
	"os"
)

// 获取文件校验和
// 返回校验和字符串，以及错误信息(如果出错的话)
func getCheckSum(filePath string) (result string, err error) {
	file, err := os.OpenFile(filePath, os.O_APPEND, 0777)
	if err != nil{
		return result,err
	}
	defer file.Close()

	buffer := make([]byte, 200)
	checkSum := 0               // 校验和
	checkByteCnt := uint8(16)   // 校验和位数
	mark := 1<<checkByteCnt - 1 // 掩码，使checkSum保留低16位
	for true {
		n, err := file.Read(buffer)
		if err != nil && err != io.EOF {
			return result, err
		}

		// 表示文件读取完毕
		if n == 0 {
			break
		}

		// 每次取2个字节进行求和
		for i := 0; i < n; i = i + 2 {
			num := int(buffer[i])<<(checkByteCnt/2) | int(buffer[i+1])
			checkSum += num

			// 进位
			for checkSum>>checkByteCnt != 0 {
				carry := checkSum >> checkByteCnt
				checkSum = checkSum & mark
				checkSum += carry
			}
		}
	}
	result = fmt.Sprintf("%04X", checkSum)
	return result, nil
}

func main() {


	// 根据用户输入的文件路径，获取文件校验和
	for i:=1;i<len(os.Args);i++{
		filePath:=os.Args[i]
		result,err:=getCheckSum(filePath)
		if err!=nil{
			fmt.Printf("文件路径(%s)有误，系统无法识别。 \n",filePath)
			continue
		}
		fmt.Printf("文件(%s)校验和为: %s \n",filePath,result)
	}

}
