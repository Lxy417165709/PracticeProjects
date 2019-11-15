package common

import (
	"errors"
	"github.com/astaxie/beego/logs"
	"github.com/dgrijalva/jwt-go"
	"time"
)

const SecretKey  ="secretkey"


type jwtCustomClaims struct {
	jwt.StandardClaims
	Uid string `json:"uid"`
	Identity string `json:"identity"`
}


func CreateToken(SecretKey []byte, uid string,identity string) (tokenString string, err error) { //创建token
	claims := &jwtCustomClaims{
		jwt.StandardClaims{
			ExpiresAt: int64(time.Now().Add(time.Hour*72).Unix()),
		},
		uid,
		identity,
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	tokenString, err = token.SignedString(SecretKey)

	return
}
func ParseToken(tokenSrt string, SecretKey []byte) (claims jwt.Claims, err error) {	//解析token
	var token *jwt.Token
	token, err = jwt.Parse(tokenSrt, func(*jwt.Token) (interface{}, error) {
		return SecretKey, nil
	})
	claims = token.Claims
	return
}

func IsRefreshToken(claims jwt.Claims)(token string,isRefresh bool,err error){	//token刷新机制(如果token在一天内过期，进行token刷新)
	if claims.(jwt.MapClaims)["exp"]==nil{
		err = errors.New("读取token参数错误")
		logs.Error(err)
		return
	}
	if claims.(jwt.MapClaims)["exp"].(float64) < float64(time.Now().Add(time.Hour*24).Unix()) {
		isRefresh=true
		token,err=CreateToken([]byte(SecretKey),claims.(jwt.MapClaims)["uid"].(string),claims.(jwt.MapClaims)["identity"].(string))
		if err!=nil{
			logs.Error(err)
			return
		}
	}else{
		isRefresh=false
	}
	return
}


//func TestCreateToken(t *testing.T)  {					//测试
//	token, _ := CreateToken([]byte(SecretKey), "YDQ", 2222, true)
//
//	fmt.Println(token)
//	claims, err := ParseToken(token, []byte(SecretKey))
//	if err != nil {
//		logs.Error(err)
//		return
//	}
//
//	fmt.Println("claims:", claims)
//	fmt.Printf("claims uid:%v,claims uid type:%T\n", claims.(jwt.MapClaims)["uid"],claims.(jwt.MapClaims)["uid"])
//
//	//此处注意接口(interface)的取值方法
//	var i float64
//
//	//错误
//	//i=claims.(jwt.MapClaims)["uid"]
//
//	//正确
//	i=claims.(jwt.MapClaims)["uid"].(float64)
//	fmt.Printf("i:%v,i type:%T\n",i,i)
//}