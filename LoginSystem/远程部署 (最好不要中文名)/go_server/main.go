package main

import (
	"common"
	"controllers"
	"env"
	"fmt"
	"github.com/astaxie/beego/logs"
	"log"
	"models"
	"net/http"
)

// 服务器测试接口
func test(w http.ResponseWriter, r *http.Request) {
	_, err := w.Write([]byte("You have been connecting!"))

	if err != nil {
		log.Fatal(err)
	}
}

// 测试代码 (测试成功)
func testSql(){
	logs.Info(models.GetUserAccountInformation(1))
}

// 系统初始化
func systemInitial() error {
	// 环境初始化
	err := env.LoadConf("conf.ini")
	if err != nil {
		return err
	}

	// 数据库初始化
	err = models.Initdb()
	if err != nil {
		return err
	}

	// 日志初始化
	err = common.InitLogs()
	if err != nil {
		return err
	}
	return nil
}

func main() {
	err := systemInitial()
	if err != nil {
		log.Fatal(err)
		return
	}
	logs.Info("Init Success!")

	// 服务器设置
	http.HandleFunc("/server/test", test)
	http.HandleFunc("/server/register", controllers.Register)
	http.HandleFunc("/server/login", controllers.Login)
	http.HandleFunc("/server/getPersonalInformation", controllers.GetPersonalInformation)
	http.HandleFunc("/server/updateUserPersonalInformation",controllers.UpdateUserPersonalInformation)
	http.HandleFunc("/server/uploadPhoto",controllers.UploadPhoto)
	//addr := fmt.Sprintf(": 8080", env.Conf.Server.Port)		// 监听地址
	addr := fmt.Sprintf(":%d", env.Conf.Server.Port)		// 监听地址
	err = http.ListenAndServe(addr, nil)

	if err != nil {
		logs.Alert(err)
	}

}
