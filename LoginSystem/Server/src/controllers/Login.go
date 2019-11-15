package controllers

import (
	"common"
	"fmt"
	"github.com/astaxie/beego/logs"
	"models"
	"net/http"
)

func Login(w http.ResponseWriter, r *http.Request) {

	// 获取请求体
	reqBody, err := common.ParseRequest(r)
	if err != nil {
		logs.Error(err)
		common.Response(w, r, -1, nil, "登陆失败 - 解析请求失败", "/server/login")
		return
	}

	// 获取登录表单
	loginForm, ok := reqBody.Data.(map[string]interface{})
	if !ok {
		err := fmt.Errorf("获取登录表单失败")
		logs.Error(err)
		common.Response(w, r, -10, nil, "登录失败 - 获取登录表单失败", "/server/login")
		return
	}

	// 获取登录手机
	loginPhone, ok := loginForm["loginPhone"].(string)
	if !ok {
		err := fmt.Errorf("获取登录手机字段失败")
		logs.Error(err)
		common.Response(w, r, -11, nil, "登录失败 - 获取登录手机字段失败", "/server/login")
		return
	}
	// 获取登录邮箱
	loginEmail, ok := loginForm["loginEmail"].(string)
	if !ok {
		err := fmt.Errorf("获取登录邮箱字段失败")
		logs.Error(err)
		common.Response(w, r, -12, nil, "登录失败 - 获取登录邮箱字段失败", "/server/login")
		return
	}

	// 获取登录密码
	loginPassword, ok := loginForm["loginPassword"].(string)
	if !ok {
		err := fmt.Errorf("获取登录密码字段失败")
		logs.Error(err)
		common.Response(w, r, -13, nil, "登录失败 - 获取登录密码字段失败", "/server/login")
		return
	}

	// 下面就需要数据库的操作了
	userAccountInformation := common.UserAccountInformation{
		UserPhone:loginPhone,
		UserEmail:loginEmail,
		UserPassword:loginPassword,
	}

	// 通过用户信息 获取 用户id
	userId,err := models.GetUserId(userAccountInformation)
	if err != nil{
		logs.Error(err)
		common.Response(w, r, -20, nil, "登录失败 - 获取用户ID失败", "/server/login")
		return
	}

	// 通过userId判断用户是否注册了
	if userId == 0{
		common.Response(w, r, -30, userId, "登录失败 - 账号或密码错误", "/server/login")
	}else{

		// 设置cookies
		err :=  common.SetCookies(w,userId)
		if err != nil {
			logs.Error(err)
			common.Response(w, r, -40, userId, "登录失败 - cookies生成错误", "/server/login")
			return
		}

		// 获取用户信息
		userAccountInformation,err := models.GetUserAccountInformation(userId)
		if err != nil{
			logs.Error(err)
			common.Response(w, r, -50, userId, "登录失败 - 获取用户账户信息失败", "/server/login")
			return
		}

		err = models.UpdateLastLoginTime(userId)	// 更新用户最近登录时间
		if err != nil {
			logs.Error(err)
			common.Response(w, r, -60, userId, "登录失败 - 更新用户最近登录时间失败", "/server/login")
		}

		common.Response(w, r, 0, userAccountInformation, "登录成功！", "/server/login")
	}

}
