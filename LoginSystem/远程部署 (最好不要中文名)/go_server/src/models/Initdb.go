package models

import (
	"database/sql"
	"env"
	"fmt"
	_ "github.com/lib/pq"
)

var db *sql.DB

// 初始化数据库
func Initdb() error {

	// 获取数据库信息
	dbInfo := fmt.Sprintf(
		`host=%s port=%d user=%s password=%s dbname=%s sslmode=%s`,
		env.Conf.Db.Host,
		env.Conf.Db.Port,
		env.Conf.Db.User,
		env.Conf.Db.Password,
		env.Conf.Db.Dbname,
		env.Conf.Db.Sslmode,
	)
	var err error

	// 开始连接数据库
	db, err = sql.Open("postgres", dbInfo)
	if err != nil {
		return err
	}

	// 设置数据库连接
	db.SetMaxIdleConns(10)
	db.SetMaxOpenConns(50)

	// ping 数据库
	return db.Ping()
}

// 关闭数据库
func Closedb() error {
	return db.Close()
}
