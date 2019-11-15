func UserLogin(w http.ResponseWriter, r *http.Request) {
	var userStatus UserStatus
	req, err := GetBodyData(r)
	if err != nil {
		logs.Error(err)
		ErrorResp(w, r, err.Error(), http.StatusInternalServerError)
		return
	}
	data, ok := req["data"].(map[string]interface{})
	if !ok {
		logs.Warn("读取表单参数错误")
		ErrorResp(w, r, "读取表单参数错误", http.StatusBadRequest)
		return
	}

	uid, ok1 := data["uid"].(string)
	nickName, ok2 := data["nickname"].(string)
	//email, ok3 := data["email"].(string)
	if !ok1 || !ok2 {
		logs.Warn("读取表单参数错误")
		ErrorResp(w, r, "读取表单参数错误", http.StatusBadRequest)
		return
	}
	user := new(models.TUser)
	IsUserExist, err := models.IsUserExist(uid)
	if err != nil {
		logs.Error(err)
		ErrorResp(w, r, err.Error(), http.StatusInternalServerError)
		return
	}
	user.Openid = uid
	user.Nickname = nickName
	if !IsUserExist {
		err = InitUser(user)
		if err != nil {
			logs.Error(err)
			ErrorResp(w, r, err.Error(), http.StatusInternalServerError)
			return
		}
	}

	userStatus.ProblemNum, err = common.GetProblemNum(uid)
	if err != nil {
		logs.Error(err)
		ErrorResp(w, r, err.Error(), http.StatusInternalServerError)
		return
	}

	token, err := common.CreateToken([]byte(common.SecretKey), uid, "student")
	cookie := &http.Cookie{
		Name:  "token",
		Value: token,
	}
	http.SetCookie(w, cookie)

	if err := SuccessResp(w, r, userStatus, "登录成功", 0); err != nil {
		logs.Error(err)
		return
	}
}
func UserLogout(w http.ResponseWriter, r *http.Request) {
	cookie := http.Cookie{
		Name:   "token",
		MaxAge: -1,
	}
	http.SetCookie(w, &cookie)
	if err := SuccessResp(w, r, "", "注销成功", 0); err != nil {
		logs.Error(err)
		return
	}
}
func CheckStatus(w http.ResponseWriter, r *http.Request) {
	logs.Debug(0)
	token, err := r.Cookie("token")
	if err != nil {
		logs.Error(err)
		if err := SuccessResp(w, r, -1, "身份认证失效，请先登录", 0); err != nil {
			logs.Error(err)
			return
		}
		return
	}
	if token == nil {
		logs.Error(err)
		if err := SuccessResp(w, r, -1, "身份认证失效，请先登录", 0); err != nil {
			logs.Error(err)
			return
		}
		return
	}
	claims, err := common.ParseToken(token.Value, []byte(common.SecretKey))
	if err != nil {
		logs.Error(err)
		if err := SuccessResp(w, r, -1, "身份认证失效，请先登录", 0); err != nil {
			logs.Error(err)
			return
		}
		return
	}
	//token刷新机制(如果token在一天内过期，进行token刷新)
	newToken, isRefresh, err := common.IsRefreshToken(claims)
	if err != nil {
		logs.Error(err)
		ErrorResp(w, r, err.Error(), http.StatusBadRequest)
		return
	}
	if isRefresh {
		cookie := &http.Cookie{
			Name:  "token",
			Value: newToken,
		}
		http.SetCookie(w, cookie)
	}

	identity := claims.(jwt.MapClaims)["identity"].(string)
	status := -1
	if identity == "manager" {
		status = 0
	} else if identity == "student" {
		status = 1
	}
	if err := SuccessResp(w, r, status, "身份认证通过", 0); err != nil {
		logs.Error(err)
		return
	}
	return
}