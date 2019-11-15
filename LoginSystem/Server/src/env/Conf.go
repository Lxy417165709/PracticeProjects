package env

import (
	"fmt"
	"github.com/astaxie/beego/logs"
	"github.com/go-ini/ini"
	"reflect"
	"strings"
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

type Configure struct {
	Common struct {
		Version string `json:"version" remark:"软件发布版本，对应仓库tag版本"`
		Creator string `json:"creator" remark:"创建者"`
	}
	Db struct {
		Host     string `json:"host" remark:"数据库主机"`
		Port     int    `json:"port" remark:"数据库端口"`
		User     string `json:"user" remark:"数据库用户"`
		Password string `json:"password" remark:"数据库密码"`
		Dbname   string `json:"dbname" remark:"数据库名"`
		Sslmode  string `json:"sslmode" remark:"ssl模式"`
	}
	Server struct {
		Port     int    `json:"port" remark:"服务器监听端口"`
		Filepath string `json:"filepath" remark:"服务器默认文件路径"`
		TokenKey string `json:"tokenKey" remark:"token加密密钥"`
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
