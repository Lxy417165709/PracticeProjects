package common

import (
	"encoding/json"
	"io/ioutil"
	"net/http"
)

// 解析请求, 取得请求协议结构 (请求协议 即 请求体)
func ParseRequest(r *http.Request) (reqProto ReqProto, err error) {
	// 获取请求体字节流
	reqBodyBytes, err := ioutil.ReadAll(r.Body)
	if err != nil {
		return reqProto, err
	}
	// 获取请求协议结构
	reqProto = ReqProto{}
	err = json.Unmarshal(reqBodyBytes, &reqProto)
	if err != nil {
		return reqProto, err
	}
	return reqProto, err
}