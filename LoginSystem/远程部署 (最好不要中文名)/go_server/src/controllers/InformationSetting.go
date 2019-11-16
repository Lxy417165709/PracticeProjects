package controllers

import (
	"common"
	"fmt"
	"github.com/astaxie/beego/logs"
	"models"
	"net/http"
)
// 更新用户个人信息
func UpdateUserPersonalInformation(w http.ResponseWriter,r *http.Request){
	API:="/server/updateUserPersonalInformation"

	// 获取请求体
	reqBody,err := common.ParseRequest(r)
	if err!=nil {
		logs.Error(err)
		common.Response(w,r,-1,nil,"更新失败",API)
		return
	}

	// 获取请求表单
	requestForm,ok := reqBody.Data.(map[string]interface{})
	if !ok {
		err := fmt.Errorf("获取请求表单失败")
		logs.Error(err)
		common.Response(w,r,-10,nil,"更新失败",API)
		return
	}

	// 获取用户Id
	userId,ok := requestForm["userId"].(float64)	// 前端传来的userId的类型是float64
	if !ok {
		err := fmt.Errorf("获取用户ID失败")
		logs.Error(err)
		common.Response(w,r,-11,nil,"更新失败",API)
		return
	}
	// 获取用户头像
	userPhoto,ok := requestForm["userPhoto"].(string)
	if !ok {
		err := fmt.Errorf("获取用户头像失败")
		logs.Error(err)
		common.Response(w,r,-12,nil,"更新失败",API)
		return
	}

	// 获取用户名
	userName,ok := requestForm["userName"].(string)
	if !ok {
		err := fmt.Errorf("获取用户名失败")
		logs.Error(err)
		common.Response(w,r,-13,nil,"更新失败",API)
		return
	}

	// 获取用户性别
	userSex,ok := requestForm["userSex"].(string)
	if !ok {
		err := fmt.Errorf("获取用户性别失败")
		logs.Error(err)
		common.Response(w,r,-14,nil,"更新失败",API)
		return
	}

	// 获取用户联系电话
	userContactPhone,ok := requestForm["userContactPhone"].(string)
	if !ok {
		err := fmt.Errorf("获取用户联系电话失败")
		logs.Error(err)
		common.Response(w,r,-15,nil,"更新失败",API)
		return
	}

	// 获取用户联系邮箱
	userContactEmail,ok := requestForm["userContactEmail"].(string)
	if !ok {
		err := fmt.Errorf("获取用户联系邮箱失败")
		logs.Error(err)
		common.Response(w,r,-16,nil,"更新失败",API)
		return
	}

	// 获取用户生日
	userBirthday,ok := requestForm["userBirthday"].(float64)
	if !ok {
		err := fmt.Errorf("获取用户生日失败")
		logs.Error(err)
		common.Response(w,r,-17,nil,"更新失败",API)
		return
	}

	// cookies检测
	err = common.CookiesCheck(r,int64(userId))
	if err != nil {
		logs.Error(err)
		common.Response(w, r, -20, nil, "Cookies检测不通过", API)
		return
	}

	// 构建用户个人信息结构体
	userPersonalInformation := common.UserPersonalInformation{
		UserId:int64(userId),
		UserPhoto:userPhoto,
		UserName:userName,
		UserSex:userSex,
		UserContactPhone:userContactPhone,
		UserContactEmail:userContactEmail,
		UserBirthday:int64(userBirthday),
	}

	// 执行更新操作
	err = models.UpdateUserPersonalInformation(userPersonalInformation)
	if err!=nil{
		logs.Error(err)
		common.Response(w,r,-30,nil,"更新失败",API)
		return
	}

	// 获取头像的base64编码
	base64Str,err := common.UserPhotoToBase64Str(userPersonalInformation.UserPhoto)
	if err != nil {
		logs.Error(err)
		common.Response(w, r, -40, nil, "更新成功，但头像获取失败", API)
		return
	}

	// 构建结构体，返回给前端 (里面包括了图像数据) (这个结构在informationGetting的时候也出现过)
	data := struct{
		UserPersonalInformation common.UserPersonalInformation
		UserPhotoData string
	}{
		userPersonalInformation,
		base64Str,
	}
	common.Response(w,r,0,data,"更新成功",API)
}
