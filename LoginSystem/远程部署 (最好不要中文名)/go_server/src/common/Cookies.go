package common

import (
	"fmt"
	"net/http"
)
// 设置cookies
func SetCookies(w http.ResponseWriter,userId int64) error{
	// 生成token字符串
	tokenString,err := GenerateTokenString(userId)
	if err != nil {
		return err
	}

	// 构建cookies (这个cookies没有安全性，记得要采用加密算法加密！)
	cookies := &http.Cookie{
		Name:  "token",
		Value: tokenString,
		Path:  "/", // 一定要指定这个.. 不然获取不了cookies
	}

	// 设置cookies
	http.SetCookie(w, cookies)
	return nil
}

// cookies检验
func CookiesCheck(r *http.Request,userId int64) (err error){

	// 空指针
	if r==nil {
		err := fmt.Errorf("请求为空指针")
		return err
	}
	// 请求没cookies
	if len(r.Cookies()) == 0{
		err := fmt.Errorf("请求没有cookies")
		return err
	}

	// 获取token字符串,检测
	tokenString:= r.Cookies()[0].Value
	err = TokenCheck(tokenString,userId)
	if err != nil {
		return err
	}

	return nil
}

