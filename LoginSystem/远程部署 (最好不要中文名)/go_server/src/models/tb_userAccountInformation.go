package models

import (
	"common"
	"database/sql"
	"fmt"
	"time"
)



// 插入新用户信息, 成功则返回用户id, 失败时userId = 0 ，err != nil
// 存在问题 (userId 无法获取) (我决定删除userId返回功能，之后再通过查询获得userId)
func InsertUserAccountInformation(userAccountInformation common.UserAccountInformation) (err error){


	// 构建插入sql语句
	stmt, err := db.Prepare(
		`insert into 
				tb_userAccountInformation(userPhone,userEmail,userPassword,userType,userRegisterTime,userLastLoginTime)
 				values($1,$2,$3,$4,$5,$6) returning userId`,
	)
	if err != nil{
		return err
	}

	// 执行插入操作
	_, err = stmt.Exec(
		userAccountInformation.UserPhone,
		userAccountInformation.UserEmail,
		userAccountInformation.UserPassword,
		userAccountInformation.UserType,
		userAccountInformation.UserRegisterTime,
		userAccountInformation.UserLastLoginTime,
	)
	if err != nil{
		return err
	}

	return err

}

// 通过用户id 获取 用户账户信息
func GetUserAccountInformation(userId int64) (userAccountInformation *common.UserAccountInformation, err error) {

	// 判断用户id的合法性
	if userId <= 0 {
		err := fmt.Errorf("用户ID非法")
		return userAccountInformation, err
	}
	rows, err := db.Query(
		`select * from tb_userAccountInformation where userId=$1`,
		userId,
	)
	if err != nil {
		return userAccountInformation, err
	}
	defer rows.Close()

	// 把查询结果保存到结构体中
	if rows.Next() {

		var userId sql.NullInt64
		var userPhone sql.NullString
		var userEmail sql.NullString
		var userPassword sql.NullString
		var userType sql.NullInt64
		var userRegisterTime sql.NullInt64
		var userLastLoginTime sql.NullInt64
		var reserved1 sql.NullString	// 保留字段1
		var reserved2 sql.NullString	// 保留字段2

		// 扫描信息
		err := rows.Scan(&userId, &userPhone, &userEmail, &userPassword,&userType,&userRegisterTime,&userLastLoginTime,&reserved1,&reserved2)

		// 扫描信息出错
		if err != nil {
			return userAccountInformation, err
		}
		// 新建信息结构体,把信息写入结构体
		userAccountInformation = &common.UserAccountInformation{
			userId.Int64,
			userPhone.String,
			userEmail.String,
			userPassword.String,
			userType.Int64,
			userRegisterTime.Int64,
			userLastLoginTime.Int64,
		}


	} else {
		err := fmt.Errorf("用户ID不存在")
		return userAccountInformation, err
	}

	return userAccountInformation, err
}

// 通过用户id 更新 用户最近登录时间
func UpdateLastLoginTime(userId int64) (err error){

	// 修改预备
	stmt,err := db.Prepare("update tb_userAccountInformation set userLastLoginTime=$2  where userId = $1")
	if err != nil{
		return err
	}
	// 执行修改操作
	result,err := stmt.Exec(userId,time.Now().Unix())
	if err != nil{
		return err
	}
	// 获取影响的行
	rowsAffected,err := result.RowsAffected()
	if err != nil{
		return err
	}

	if rowsAffected != 1{
		err := fmt.Errorf("修改最近登录时间失败，可能是数据库没有该用户！")
		return err
	}

	return err

}

// 通过用户账户信息(手机or邮箱字段) 获取 用户id
func GetUserId(userAccountInformation common.UserAccountInformation) (userId int64, err error){


	// 执行sql语句，通过用户的邮箱或手机获取用户的信息
	rows,err := db.Query("select userId from tb_userAccountInformation where userPassword=$1 and userPhone=$2 or userPassword=$1 and userEmail=$3",
		userAccountInformation.UserPassword,
		userAccountInformation.UserPhone,
		userAccountInformation.UserEmail,
	)
	if err != nil{
		return userId,err
	}
	if rows == nil{
		err := fmt.Errorf("rows为空")
		return userId,err
	}
	// 开始获取userId
	var uid sql.NullInt64
	for rows.Next(){
		// 扫描信息
		err := rows.Scan(&uid)
		if err != nil{
			return 0,err
		}
	}
	return uid.Int64,err
}

// 通过用户手机 获取 用户ID (可用于判断手机是否已经被注册了)
func GetUserIdByUserPhone(userPhone string) (userId int64, err error){
	// 执行sql语句，通过用户的邮箱或手机获取用户的信息
	rows,err := db.Query("select userId from tb_userAccountInformation where userPhone=$1",
		userPhone,
	)
	if err != nil{
		return userId,err
	}
	if rows == nil{
		err := fmt.Errorf("rows为空")
		return userId,err
	}
	// 开始获取userId
	var uid sql.NullInt64
	for rows.Next(){
		// 扫描信息
		err := rows.Scan(&uid)
		if err != nil{
			return 0,err
		}
	}
	return uid.Int64,err

}

func GetUserIdByUserEmail(userEmail string) (userId int64, err error){
	// 执行sql语句，通过用户的邮箱或手机获取用户的信息
	rows,err := db.Query("select userId from tb_userAccountInformation where userEmail=$1",
		userEmail,
	)
	if err != nil{
		return userId,err
	}
	if rows == nil{
		err := fmt.Errorf("rows为空")
		return userId,err
	}
	// 开始获取userId
	var uid sql.NullInt64
	for rows.Next(){
		// 扫描信息
		err := rows.Scan(&uid)
		if err != nil{
			return 0,err
		}
	}
	return uid.Int64,err

}