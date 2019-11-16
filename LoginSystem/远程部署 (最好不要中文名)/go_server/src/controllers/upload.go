package controllers

import (
	"common"
	"env"
	"github.com/astaxie/beego/logs"
	"io/ioutil"
	"net/http"
)

// 上传图片，由于前端是使用uploader的，所以此时不符合前端请求协议
// 成功了！ 文件名由前端指定   格式为 : 时间戳_用户ID.图片后缀，如 1573744349758_5.gif
func UploadPhoto(w http.ResponseWriter,r *http.Request){
	API:="uploadPhoto"

	//获取表单中的文件
	file, header, err := r.FormFile("file")	 // key要和前端约定好
	if err != nil {
		logs.Error(err)
		common.Response(w,r,-1,nil,"上传失败",API)
		return
	}

	// 解析文件为二进制数据
	blob, err := ioutil.ReadAll(file)
	if err != nil {
		logs.Error(err)
		common.Response(w,r,-10,nil,"上传失败",API)
		return
	}

	// 文件名
	fileName :=  header.Filename

	// 写到本地文件
	err = ioutil.WriteFile(env.Conf.Server.Filepath + "/img/" + fileName, blob, 0777)
	if err != nil {
		logs.Error(err)
		common.Response(w,r,-20,nil,"上传失败",API)
		return
	}

	common.Response(w,r,0,nil,"上传成功",API)
}
