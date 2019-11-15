package common

import (
	"fmt"
	"reflect"
	"strings"

	"github.com/astaxie/beego/logs"
	"github.com/go-ini/ini"
)

var Cfg *ini.File

func LoadConf(confPath string) error {
	if confPath == "" {
		err := fmt.Errorf("configure file path should not be blank")
		logs.Error(err)
		return err
	}
	var err error
	Cfg, err = ini.Load(confPath)
	if err != nil || Cfg == nil {
		err := fmt.Errorf("can not load configure file:%s %s ", confPath, err)
		logs.Error(err)
		return err
	}
	//检查并初始化配置结构体
	err = InitConfigure()
	if err != nil {
		logs.Error(err)
		return err
	}
	return nil
}

//配置文件结构体，配置文件上的内容需要一一对应，可多不可少
type Configure struct {
	Common struct {
		Version   string `json:"version" remark:"软件发布版本，对应仓库tag版本"`
		AppMode   string `json:"app_mode" remark:"开发模式develop/test/product"`
		Host      string `json:"host" remark:"应用部署主机地址"`
		HttpPort  string `json:"http_port" remark:"后端程序端口"`
		ApiPort   string `json:"api_port" remark:"附加api端口"`
		ApiUrl    string `json:"api_url" remark:"附加api url"`
		LogLevel  string `json:"log_level" remark:"日志级别"`
		GitPrefix string `json:"git_prefix" remark:"用户git仓库url前缀"`
	}
	Static struct {
		StaticUrl    string `json:"static_url" remark:"指导文档、图片等静态资源地址"`
		BaseUrl      string `json:"base_url" remark:"静态资源基础url"`
		StaticPrefix string `json:"static_prefix" remark:"静态资源地址前缀"`
	}
	Db struct {
		Host     string `json:"host" remark:"数据库主机"`
		Port     int    `json:"port" remark:"数据库端口"`
		User     string `json:"user" remark:"数据库用户"`
		Password string `json:"password" remark:"数据库密码"`
		Dbname   string `json:"dbname" remark:"数据库名"`
		Sslmode  string `json:"sslmode" remark:"ssl模式"`
	}
	Redis struct {
		Host     string `json:"host" remark:"redis主机"`
		Port     int    `json:"port" remark:"redis端口"`
		Password string `json:"password" remark:"redis密码"`
	}
	GitService struct {
		User       string `json:"user" remark:"超级用户，拉取用户代码"`
		Password   string `json:"password"  remark:"超级用户密码"`
		RepoPrefix string `json:"repo_prefix" remark:"仓库前缀地址"`
		BaseUrl    string `json:"base_url" remark:"Git服务api基础地址url"`
		ImportUrl  string `json:"import_url" remark:"初始化仓库地址"`
	}
	Docker struct {
		Images string `json:"images" remark:"docker镜像列表，逗号分隔"`
	}
}

var Conf = &Configure{}

//初始化配置信息，测试需要修改配置文件
func InitConfigure() (err error) {
	confValue := reflect.ValueOf(Conf).Elem()
	confType := reflect.TypeOf(*Conf)

	for i := 0; i < confType.NumField(); i++ {
		section := confType.Field(i)
		sectionValue := confValue.Field(i)

		//读取节类型信息
		for j := 0; j < section.Type.NumField(); j++ {
			key := section.Type.Field(j)
			keyValue := sectionValue.Field(j)

			sec := strings.ToLower(section.Name) //配置文件节名
			remark := key.Tag.Get("remark")      //配置备注
			tag := key.Tag.Get("json")           //配置键节名
			if tag == "" {
				err = fmt.Errorf("can not found a tag name `json` in struct of [%s].%s", sec, tag)
				logs.Error(err)
				return err
			}
			if Cfg == nil {
				err = fmt.Errorf("init configure file failed ")
				logs.Error(err)
				return err
			}
			//读取配置文件初始化结构体
			value := Cfg.Section(sec).Key(tag)
			if value == nil {
				err = fmt.Errorf("get key failed [%s].%s %s", sec, tag, remark)
				logs.Error(err)
				return err
			}
			//根据不同类型初始化
			k := keyValue.Interface()
			switch k.(type) {
			case string:
				s := value.String()
				if s == "" {
					err = fmt.Errorf("cant not read configure item [%s].%s %s", sec, tag, remark)
					logs.Error(err)
					return err
				}
				keyValue.SetString(s)

			case int:
				n, err := value.Int()
				if err != nil || n < 0 {
					err = fmt.Errorf("not a valid number [%s].%s %s", sec, tag, remark)
					logs.Error(err)
					return err
				}
				keyValue.SetInt(int64(n))

			default:
				logs.Warn("nonsupport type of config key", key.Type)
			}
		}
	}
	return
}
