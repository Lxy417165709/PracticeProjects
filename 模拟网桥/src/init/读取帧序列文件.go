package Generate

import (
	"bufio"
	"commonObjects"
	"fmt"
	"io"
	"os"
	"strings"
)

// 读取文件的帧序列
// 返回帧结构切片，出错则返回错误
func ReadFrameSet(fileName string) (frameSet []commonObjects.Frame, err error) {
	file, err := os.Open(fileName)
	if err != nil {
		return frameSet, err
	}
	defer file.Close()
	buffer := bufio.NewReader(file)
	for {
		bytes, err := buffer.ReadBytes('\n')

		// 表示读取完毕
		if err == io.EOF {
			return frameSet, nil
		}

		// 表示出错
		if err != nil {
			return frameSet, err
		}

		frameString := string(bytes)
		frameSlice := strings.Split(frameString, " ")

		if len(frameSlice) != 3 {
			err := fmt.Errorf(fmt.Sprintf("该文件(%s)存在错误帧",fileName))
			return frameSet, err
		}

		// 这里最后会留有一个换行符，我们要把它去掉
		frameSlice[2] = strings.Trim(frameSlice[2],"\n")
		frameSet = append(frameSet, commonObjects.Frame{
			frameSlice[0],
			frameSlice[1],
			frameSlice[2],
		})
	}
}

