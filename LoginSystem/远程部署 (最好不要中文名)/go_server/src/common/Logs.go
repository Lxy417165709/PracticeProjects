package common

import "github.com/astaxie/beego/logs"

//var Logs *logs.BeeLogger // 日志器

// 日志初始化
func InitLogs() error {
	//Logs = logs.NewLogger(10000)         // 创建一个日志记录器，参数为缓冲区的大小
	//err := Logs.SetLogger("console", "") // 设置日志记录方式：控制台记录
	//if err != nil {
	//	return err
	//}
	//Logs.SetLevel(logs.LevelDebug) // 设置日志写入缓冲区的等级：Debug级别（最低级别，所以所有log都会输入到缓冲区）
	//Logs.EnableFuncCallDepth(true) // 输出log时能显示输出文件名和行号（非必须）

	logs.SetLevel(logs.LevelDebug)
	logs.EnableFuncCallDepth(true)
	/* 日志输出测试
		Logs.Emergency("Emergency")
		Logs.Alert("Alert")
		Logs.Critical("Critical")
		Logs.Error("Error")
		Logs.Warning("Warning")
		Logs.Notice("Notice")
		Logs.Informational("Informational")
		Logs.Debug("Debug")
	*/

	//err = logs.SetLogger(logs.AdapterConsole, "")	// 可输出行号
	//log.SetFlags(log.Lshortfile | log.LstdFlags)

	return nil
}
