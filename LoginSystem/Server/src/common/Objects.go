package common

//后端响应数据通信协议
type ReplyProto struct {
	Status    int         `json:"status"` //状态 0正常，小于0出错，大于0可能有问题
	Msg       string      `json:"msg"`    //状态信息
	Data      interface{} `json:"data"`
	API       string      `json:"API"`        //api接口
	Method    string      `json:"method"`     //post,put,get,delete
	RowCount  int         `json:"rowCount"`   //Data若是数组，算其长度
	Time      int64       `json:"time"`       //请求响应时间，毫秒
	CheckTime int64       `json:"check_time"` //检测时间，毫秒
}

//前端请求数据通讯协议
type ReqProto struct {
	Action   string      `json:"action"` //请求类型GET/POST/PUT/DELETE
	Data     interface{} `json:"data"`   //请求数据
	Sets     []string    `json:"sets"`
	OrderBy  string      `json:"orderBy"`  //排序要求
	Filter   string      `json:"filter"`   //筛选条件
	Page     int         `json:"page"`     //分页
	PageSize int         `json:"pageSize"` //分页大小
}

/*
	以下两个结构体没用到
	// 注册信息结构
	type RegisterInformation struct {
		RegisterPhone    string `json:"registerPhone"`
		RegisterEmail    string `json:"registerEmail"`
		RegisterPassword string `json:"registerPassword"`
	}

	// 登陆信息结构
	type LoginInformation struct {
		LoginPhone    string `json:"loginPhone"`
		LoginEmail    string `json:"loginEmail"`
		LoginPassword string `json:"loginPassword"`
	}
*/

// 用户账户信息结构体
type UserAccountInformation struct {
	UserId            int64  `json:"userId"`
	UserPhone         string `json:"userPhone"`
	UserEmail         string `json:"userEmail"`
	UserPassword      string `json:"userPassword"`
	UserType          int64  `json:"userType"`
	UserRegisterTime  int64  `json:"userRegisterTime"`
	UserLastLoginTime int64  `json:"userLastLoginTime"`
}

// 用户个人信息结构体
type UserPersonalInformation struct {
	UserId           int64  `json:"userId"`
	UserPhoto        string `json:"userPhoto"`
	UserName         string `json:"userName"`
	UserSex          string    `json:"userSex"`
	UserContactPhone string `json:"userContactPhone"`
	UserContactEmail string `json:"userContactEmail"`
	UserBirthday     int64  `json:"userBirthday"`
}
