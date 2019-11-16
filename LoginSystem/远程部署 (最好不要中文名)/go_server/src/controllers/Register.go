package controllers

import (
	"common"
	"fmt"
	"github.com/astaxie/beego/logs"
	"models"
	"net/http"
	"time"
)



// 这里还有一些业务逻辑没处理，比如手机是空的，手机不符合规范，邮箱空....
// 成功则返回用户账户信息
// 这里可以先去数据库注册手机or邮箱是否已经存在了
func Register(w http.ResponseWriter, r *http.Request) {

	API:= "/server/register"
	reqBody,err := common.ParseRequest(r)
	if err != nil {
		logs.Error(err)
		common.Response(w, r, -1, nil, "注册失败 - 请求体解析错误", API)
		return
	}

	// 获得注册表单
	registerForm,ok := reqBody.Data.(map[string]interface{})
	if !ok {
		err := fmt.Errorf("获取注册表单失败")
		logs.Error(err)
		common.Response(w, r, -10, nil, "注册失败 - 获取注册表单失败", API)
		return
	}

	// 获取表单字段的注册手机
	registerPhone,ok := registerForm["registerPhone"].(string)
	if !ok {
		err := fmt.Errorf("获取注册手机字段失败")
		logs.Error(err)
		common.Response(w, r, -11, nil, "注册失败 - 获取注册手机字段失败", API)
		return
	}

	// 获取表单字段的注册邮箱
	registerEmail,ok := registerForm["registerEmail"].(string)
	if !ok {
		err := fmt.Errorf("获取注册邮箱字段失败")
		logs.Error(err)
		common.Response(w, r, -12, nil, "注册失败 - 获取注册邮箱字段失败", API)
		return
	}

	// 获取表单字段的注册密码
	registerPassword,ok := registerForm["registerPassword"].(string)
	if !ok {
		err := fmt.Errorf("获取注册密码字段失败")
		logs.Error(err)
		common.Response(w, r, -13, nil, "注册失败 - 获取注册密码字段失败", API)
		return
	}

	// 获取表单字段的注册密码
	registerType,ok := registerForm["registerType"].(float64)
	if !ok {
		err := fmt.Errorf("获取注册类型字段失败")
		logs.Error(err)
		common.Response(w, r, -14, nil, "注册失败 - 获取注册类型字段失败", API)
		return
	}
	// 获取表单字段的注册时间
	registerTime,ok := registerForm["registerTime"].(float64)

	if !ok {
		err := fmt.Errorf("获取注册时间字段失败")
		logs.Error(err)
		common.Response(w, r, -15, nil, "注册失败 - 获取注册时间字段失败", API)
		return
	}

	// 获取表单字段的最近登录时间
	lastLoginTime,ok := registerForm["lastLoginTime"].(float64)
	if !ok {
		err := fmt.Errorf("获取最近登录时间字段失败")
		logs.Error(err)
		common.Response(w, r, -16, nil, "注册失败 - 获取最近登录时间字段失败", API)
		return
	}

	// 构建用户账户信息结构
	userAccountInformation := common.UserAccountInformation{
		UserPhone: registerPhone,
		UserEmail: registerEmail,
		UserPassword: registerPassword,
		UserType:int64(registerType),
		UserRegisterTime: int64(registerTime),
		UserLastLoginTime:int64(lastLoginTime),
	}
	// 先从数据库查询注册手机是否已经存在
	uid,err := models.GetUserIdByUserPhone(registerPhone)
	if err != nil{
		logs.Error(err)
		common.Response(w, r, -20, nil, "注册失败 - 查询注册手机操作失败", API)
		return
	}
	// 表示手机已经存在
	if uid != 0 {
		common.Response(w, r, -21, nil, "注册失败 - 该手机已被注册", API)
		return
	}

	// 从数据库查询注册邮箱是否已经存在
	uid,err = models.GetUserIdByUserEmail(registerEmail)
	if err != nil{
		logs.Error(err)
		common.Response(w, r, -22, nil, "注册失败 - 查询邮箱操作失败", API)
		return
	}
	// 表示邮箱已经存在
	if uid != 0 {
		common.Response(w, r, -23, nil, "注册失败 - 该邮箱已被注册", API)
		return
	}


	// 将该账户信息插入数据库
	err = models.InsertUserAccountInformation(userAccountInformation)
	if err != nil{
		logs.Error(err)
		common.Response(w, r, -30, nil, "注册失败 - 账户信息插入数据库失败", API)
		return
	}

	// 获取用户id
	userId,err := models.GetUserId(userAccountInformation)
	if err != nil{
		logs.Error(err)
		common.Response(w, r, -40, nil, "注册失败 - 用户ID不存在", API)
		return
	}

	// 注册成功后设置cookies
	err =  common.SetCookies(w,userId)
	if err != nil {
		logs.Error(err)
		common.Response(w, r, -50, userId, "cookies生成错误!", "/server/login")
		return
	}

	// 构建用户个人信息结构
	userPersonalInformation := common.UserPersonalInformation{
		UserId: userId,
		UserName:"武藤纯子",
		UserSex: "秘密",
		UserPhoto: "default.jpg",
		UserContactPhone:userAccountInformation.UserPhone,
		UserContactEmail:userAccountInformation.UserEmail,
		UserBirthday:time.Now().Unix()*1000 ,	// 为了符合时间戳规程
	}
	// 将个人信息插入数据库
	err = models.InsertUserPersonalInformation(userPersonalInformation)
	if err != nil{
		logs.Error(err)
		common.Response(w, r, -60, userId, "注册成功！但用户个人信息没有录入！", API)
		return
	}

	// 注意这里要给用户账户信息添加userID,不然返回前端的uid是0
	userAccountInformation.UserId = userId
	common.Response(w, r, 0, userAccountInformation, "注册成功！", API)
}

