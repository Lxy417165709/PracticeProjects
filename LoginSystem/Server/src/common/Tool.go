package common

import (
	"encoding/base64"
	"env"
	"io/ioutil"
)
// 通过用户头像名，获取头像的base64编码
func UserPhotoToBase64Str(userPhoto string) (base64Str string,err error){

	// 读取图片
	bytes,err := ioutil.ReadFile(env.Conf.Server.Filepath + "img/" + userPhoto)
	if err != nil {
		return base64Str,err
	}

	// 字节流转为base64编码
	base64Str = base64.StdEncoding.EncodeToString(bytes)

	return base64Str,err
}