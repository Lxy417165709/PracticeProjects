package common

import (
	"encoding/json"
	"net/http"
	"time"
)

// 向前端响应
func Response(w http.ResponseWriter, r *http.Request, status int, data interface{}, msg string, API string) error {
	// 构建响应协议
	replyProto := ReplyProto{
		Status:    status,
		Method:    "POST",
		Data:      data,
		API:       API,
		Msg:       msg,
		Time:      time.Now().Unix(),
		CheckTime: time.Now().Unix(),
	}

	// 构建响应体字节流
	replyBodyBytes, err := json.Marshal(replyProto)
	if err != nil {
		return err
	}

	// 写回前端
	w.Header().Set("Content-Type", "application/json;charset=utf-8") // 这里其实可以省略
	_, err = w.Write(replyBodyBytes)
	if err != nil {
		return err
	}
	return nil
}

