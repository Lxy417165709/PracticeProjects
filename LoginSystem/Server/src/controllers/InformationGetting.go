package controllers

import (
	"common"
	"fmt"
	"github.com/astaxie/beego/logs"
	"models"
	"net/http"
)

// 获取用户个人信息
func GetPersonalInformation(w http.ResponseWriter,r *http.Request){
	API:= "/server/getPersonalInformation"

	// 获取请求体
	reqBody,err := common.ParseRequest(r)
	if err != nil{
		logs.Error(err)
		common.Response(w, r, -1, nil, "获取请求体失败", API)
		return
	}
	// 获取请求表单
	requestForm,ok := reqBody.Data.(map[string]interface{})

	if !ok {
		err := fmt.Errorf("获取请求表单失败")
		logs.Error(err)
		common.Response(w, r, -10, nil, "获取请求表单失败", API)
		return
	}

	// 获取用户ID (由于前端发来的是float64，所以必须用float64先断言)
	userId,ok := requestForm["userId"].(float64)
	if !ok {
		err := fmt.Errorf("获取用户ID失败")
		logs.Error(err)
		common.Response(w, r, -11, nil, "获取用户ID失败", API)
		return
	}


	// cookies检测
	err = common.CookiesCheck(r,int64(userId))
	if err != nil {
		logs.Error(err)
		common.Response(w, r, -20, nil, "Cookies检测不通过", API)
		return
	}

	// 获取用户个人信息
	userPersonalInformation,err := models.GetUserPersonalInformation(int64(userId))
	if err != nil {
		logs.Error(err)
		common.Response(w, r, -30, nil, "获取用户个人信息失败", API)
		return
	}

	// 获取头像的base64编码
	base64Str,err := common.UserPhotoToBase64Str(userPersonalInformation.UserPhoto)
	if err != nil {
		logs.Error(err)
		common.Response(w, r, -40, nil, "获取头像失败", API)
		return
	}

	// 构建结构体，返回给前端 (里面包括了图像数据)
	data := struct{
		UserPersonalInformation common.UserPersonalInformation
		UserPhotoData string
	}{
		*userPersonalInformation,
		base64Str,
	}

	common.Response(w, r, 0, data, "请求成功", API)

}

