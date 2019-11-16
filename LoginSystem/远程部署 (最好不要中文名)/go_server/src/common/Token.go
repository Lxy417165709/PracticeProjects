package common

import (
	"env"
	"fmt"
	"github.com/dgrijalva/jwt-go"
	"time"
)


type jwtUserClaims struct {
	jwt.StandardClaims
	UserId   int64 `json:"userId"`
}

// 生成token字符串
// expiresTime是过期时间,单位是毫秒,从现在开始算起
func GenerateTokenString(userId int64) (tokenString string, err error) {
	// 创建加密的token对象
	claims := &jwtUserClaims{
		jwt.StandardClaims{
			ExpiresAt: int64(time.Now().Add(time.Hour * 72).Unix()),
		},
		userId,
	}
	// 获取token对象
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)

	// 生成token字符串
	tokenString, err = token.SignedString([]byte(env.Conf.Server.TokenKey))
	if err != nil {
		return tokenString, err
	}
	return tokenString, err
}

// 解析token字符串,返回Claims对象
func ParseTokenString(tokenString string) (claims jwt.Claims, err error) {

	// 解析token字符串
	token, err := jwt.Parse(tokenString, func(*jwt.Token) (interface{}, error) {
		return []byte(env.Conf.Server.TokenKey), nil
	})
	if err != nil {
		return claims, err
	}
	// 返回Claims对象
	claims = token.Claims
	return claims, err
}

// token校验,出错则返回错误信息，否则返回nil
func TokenCheck(tokenString string,userId int64) (err error){
	// 解析token字符串
	claims,err := ParseTokenString(tokenString)
	if err!=nil {
		return err
	}
	// claims 格式： map[exp:1.574038538e+09 userId:5] <nil>

	// 合法性
	err = claims.Valid()
	if err!=nil {
		return err
	}

	// 解析
	mapClaims,ok := claims.(jwt.MapClaims)
	if !ok {
		err := fmt.Errorf("mapClaims断言出错")
		return err
	}

	// 获取过期时间字段
	if mapClaims["exp"]==nil{
		err = fmt.Errorf("读取token过期时间字段错误")
		return err
	}
	// 获取用户ID字段
	if mapClaims["userId"]==nil{
		err = fmt.Errorf("读取token用户id字段错误")
		return err
	}

	// 判断token是否过期
	if int64(mapClaims["exp"].(float64)) <= time.Now().Unix(){
		err = fmt.Errorf("token已过期")
		return err
	}


	// 判断用户id是否一致
	if int64(mapClaims["userId"].(float64)) != userId{
		err = fmt.Errorf("用户无效")
		return err
	}

	return nil
}
