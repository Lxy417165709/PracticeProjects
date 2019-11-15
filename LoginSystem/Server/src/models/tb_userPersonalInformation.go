package models

import (
	"common"
	"database/sql"
	"fmt"
	"strconv"
)

// 插入用户个人信息
func InsertUserPersonalInformation(userPersonalInformation common.UserPersonalInformation) (err error) {

	// 准备插入
	stmt, err := db.Prepare(
		`insert into tb_userPersonalInformation(userId,userPhoto,userName,userSex,userContactPhone,userContactEmail,userBirthday) values ($1,$2,$3,$4,$5,$6,$7)`)
	if err != nil {
		return err
	}

	// 执行插入
	result, err := stmt.Exec(
		userPersonalInformation.UserId,
		userPersonalInformation.UserPhoto,
		userPersonalInformation.UserName,
		userPersonalInformation.UserSex,
		userPersonalInformation.UserContactPhone,
		userPersonalInformation.UserContactEmail,
		userPersonalInformation.UserBirthday,
	)
	if err != nil {
		return err
	}

	// 检查插入结果
	rowsAffected, err := result.RowsAffected()
	if err != nil {
		return err
	}
	if rowsAffected != 1 {
		err := fmt.Errorf("插入失败")
		return err
	}

	return err

}

// 通过用户id 获取 用户个人信息
func GetUserPersonalInformation(uid int64) (userPersonalInformation *common.UserPersonalInformation, err error) {

	// 执行sql操作 --- 获取用户个人信息
	rows, err := db.Query(
		`select * from tb_userPersonalInformation where userId=$1`,
		uid,
	)
	if err != nil {
		return userPersonalInformation, err
	}
	defer rows.Close()

	// 查询失败
	if !rows.Next() {
		err := fmt.Errorf("用户ID不存在")
		return userPersonalInformation, err
	}

	// 把查询结果保存到结构体中
	var userId sql.NullInt64
	var userPhoto sql.NullString
	var userName sql.NullString
	var userSex sql.NullString
	var userContactPhone sql.NullString
	var userContactEmail sql.NullString
	var userBirthday sql.NullInt64
	var reserved1 sql.NullString // 保留字段1
	var reserved2 sql.NullString // 保留字段2

	// 扫描信息
	err = rows.Scan(&userId, &userPhoto, &userName, &userSex, &userContactPhone, &userContactEmail, &userBirthday, &reserved1, &reserved2)
	if err != nil {
		return userPersonalInformation, err
	}

	// 新建信息结构体,把信息写入结构体
	userPersonalInformation = &common.UserPersonalInformation{
		userId.Int64,
		userPhoto.String,
		userName.String,
		userSex.String,
		userContactPhone.String,
		userContactEmail.String,
		userBirthday.Int64,
	}
	return userPersonalInformation, err
}

/*
type UserPersonalInformation struct {
	UserId           int64  `json:"userId"`
	UserPhoto        string `json:"userPhoto"`
	UserName         string `json:"userName"`
	UserSex          string    `json:"userSex"`
	UserContactPhone string `json:"userContactPhone"`
	UserContactEmail string `json:"userContactEmail"`
	UserBirthday     int64  `json:"userBirthday"`
}
*/
// 修改用户个人信息
func UpdateUserPersonalInformation(userPersonalInformation common.UserPersonalInformation) (err error) {

	// 准备修改sql语句
	stmt, err := db.Prepare("update tb_userPersonalInformation " +
		"set userPhoto=$1,userName=$2,userSex=$3,userContactPhone=$4,userContactEmail=$5,userBirthday=$6 where userId=$7",
	)
	if err != nil {
		return err
	}
	// 执行sql操作
	result, err := stmt.Exec(
		userPersonalInformation.UserPhoto,
		userPersonalInformation.UserName,
		userPersonalInformation.UserSex,
		userPersonalInformation.UserContactPhone,
		userPersonalInformation.UserContactEmail,
		userPersonalInformation.UserBirthday,
		userPersonalInformation.UserId,
	)
	if err != nil {
		return err
	}
	// 判断是否修改成功
	rowsAffected, err := result.RowsAffected()
	if err != nil {
		return err
	}

	if rowsAffected != 1 {

		err := fmt.Errorf("数据库发现用户不唯一,受影响行数:" + strconv.Itoa(int(rowsAffected)))
		return err
	}

	return err
}
