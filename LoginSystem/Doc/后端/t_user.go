package models

import (
	"database/sql"
	"errors"
	"fmt"
	"time"

	"../common"
	"github.com/astaxie/beego/logs"
)

type TUser struct {
	Openid      string    `json:"openid"`
	Nickname    string    `json:"nickname"`
	GitId       string    `json:"git_id"`
	AvatarUrl   string    `json:"avatar_url"`
	GitAccount  string    `json:"git_account"`  //慎用，可能去除
	GitPassword string    `json:"git_password"` //慎用，可能去除
	Repository  string    `json:"repository"`   //慎用，可能去除
	CreateTime  time.Time `json:"create_time"`

	Email     string `json:"email"`
	ProjectId int64  `json:"project_id"`
	Phone     string `json:"phone"`
}

func GetUser(arr_uid string) (t *TUser, err error) {
	isuid := common.IsUID(arr_uid)

	if !isuid {
		err := fmt.Errorf("用户id不符合规范")
		logs.Error(err)
		return nil, err
	}
	rows, err := db.Query("select openid,nickname,git_id,avatar_url,git_account,git_password,repository,create_time from t_user where openid=$1", arr_uid)
	if err != nil {
		logs.Error(err)
		return t, err
	}
	defer rows.Close()
	if rows.Next() {
		var Openid sql.NullString
		var Nickname sql.NullString
		var GitId sql.NullString
		var AvatarUrl sql.NullString
		var GitAccount sql.NullString
		var GitPassword sql.NullString
		var Repository sql.NullString
		var CreateTime time.Time
		t = new(TUser)
		err := rows.Scan(&Openid, &Nickname, &GitId, &AvatarUrl, &GitAccount, &GitPassword, &Repository, &CreateTime)
		if err != nil {
			logs.Error(err)
			return t, err
		}
		t.Openid = Openid.String
		t.Nickname = Nickname.String
		t.GitId = GitId.String
		t.AvatarUrl = AvatarUrl.String
		t.GitAccount = GitAccount.String
		t.GitPassword = GitPassword.String
		t.Repository = Repository.String
		t.CreateTime = CreateTime
	} else {
		err := fmt.Errorf("没有这个用户id")
		logs.Error(err)
		return nil, err
	}
	return t, nil
}

//获取用户项目id
func GetUserProjectId(uid string) (projectId int, err error) {
	if !common.IsUID(uid) {
		err = fmt.Errorf("illegal argument of uid=%s ", uid)
		logs.Warn(err)
		return 0, err
	}

	var proId sql.NullInt64
	row := db.QueryRow("SELECT project_id FROM t_user where openid = $1", uid)
	err = row.Scan(&proId)
	if err != nil {
		logs.Error(err)
		return 0, err
	}

	if !proId.Valid {
		return 0, fmt.Errorf("The projectId of user %s is null ", uid)
	}
	projectId = int(proId.Int64)
	return projectId, nil
}

func GetUserRepository(uid string) (repository string, err error) {
	row := db.QueryRow("SELECT repository FROM t_user where openid = $1", uid)
	err = row.Scan(&repository)
	if err != nil {
		return "", err
	}
	if repository == "" {
		return "", fmt.Errorf("The  repository of user %s is null !", uid)
	}
	return repository, nil
}

func SaveUser(u *TUser) (err error) {
	if u == nil {
		err = fmt.Errorf("argument user is nil")
		logs.Error(err)
		return
	}
	if u.Openid == "" {
		err = fmt.Errorf("primary key is blank")
		logs.Error(err)
		return
	}

	stmt, err := db.Exec("INSERT INTO t_user(openid, nickname, git_id, avatar_url, "+
		"git_account, git_password, repository, email, project_id, phone) "+
		"values ($1,$2,$3,$4,$5,$6,$7,$8,$9,$10)",
		u.Openid, u.Nickname, u.GitId, u.AvatarUrl, u.GitAccount, u.GitPassword, u.Repository, u.Email, u.ProjectId, u.Phone)

	if err != nil {
		logs.Error(err)
		return err
	}
	affect, err := stmt.RowsAffected()
	if err != nil {
		logs.Error(err)
		return err
	}
	if affect == 0 {
		err = fmt.Errorf("save user failed, affected rows is 0 ")
		logs.Error(err)
		return err
	}
	return nil
}
func GetUserGitID(uid string) (gitID string, err error) {
	if !common.IsUID(uid) {
		err = errors.New("uid无效")
		logs.Error(err)
		return
	}
	row := db.QueryRow("select git_id from t_user where openid=$1", uid)
	err = row.Scan(&gitID)
	if err != nil {
		logs.Error(err)
		return
	}
	if gitID == "" {
		err = errors.New("获取到的gitID为空，uid为：" + uid)
		logs.Error(err)
		return
	}
	return
}
func IsUserExist(uid string) (isUserExist bool, err error) {
	if !common.IsUID(uid) {
		err = errors.New("uid is invalid")
		logs.Error(err)
		return
	}
	row, err := db.Query("select openid from t_user where openid=$1", uid)
	if err != nil {
		logs.Error(err)
		return
	}
	if row == nil {
		err = errors.New("row is nil")
		logs.Error(err)
		return
	}
	var OpenId sql.NullString
	for row.Next() {
		err = row.Scan(&OpenId)
		if err != nil {
			logs.Error(err)
			return
		}
	}
	if OpenId.Valid {
		isUserExist = true
		return
	} else {
		isUserExist = false
		return
	}
	return
}

// 判断登录用户信息是否正确
func UserJudge(uid string, pwd string) (status int, errorData string, err error) {
	logs.Info("判断统计者登录信息")
	if uid == "" {
		status = -3
		err = fmt.Errorf("uid is null")
		logs.Error(err)
		return
	}
	if pwd == "" {
		status = -3
		err = fmt.Errorf("pwd is null")
		logs.Error(err)
		return
	}
	type User struct {
		UID string
		Pwd string
	}
	users := []User{
		{UID: "11", Pwd: "11"},
	}

	if len(users) == 0 {
		status = -3
		err = fmt.Errorf("users length is zore")
		logs.Error(err)
		return
	}

	for i := 0; i < len(users); i++ {
		if uid == users[i].UID {
			if pwd == users[i].Pwd {
				status = 0
				return
			} else {
				status = -2
				errorData = "密码错误"
				return
			}
		}
	}
	status = -1
	errorData = "账号不存在"
	return
}
