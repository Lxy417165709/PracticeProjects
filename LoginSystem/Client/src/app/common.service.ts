import { Injectable, OnInit } from '@angular/core';
import { AppPage } from '../../e2e/src/app.po';
import { DomSanitizer } from '@angular/platform-browser';

@Injectable({
  providedIn: 'root'
})
export class CommonService {
  // 后端响应数据通信协议
  replyProto = {
    status: 0,
    Msg: '',
    data: {},
    API: '',
    method: '',
    rowCount: 0,
    time: 0,
    checkTime: 0,
  };

  // 前端请求数据通信协议
  reqProto = {
    action: '', // 请求类型GET/POST/PUT/DELETE
    data: {},   // 请求数据
    sets: [],
    orderBy: '',  // 排序要求
    filter: '',   // 筛选条件
    page: 0,      // 分页
    pageSize: 0,  // 分页大小
  };

  // 用户账户信息结构
  userAccountInformation = {
    userId: -1,
    userPhone: '',
    userEmail: '',
    userPassword: '',
  };

  // 用户个人信息结构
  userPersonalInformation = {
    userId: -1,
    userPhoto: '',
    userName: '',
    userSex: '',
    userContactPhone: '',
    userContactEmail: '',
    userBirthday: 0,
    userPhotoUrl: undefined, // 与后端相比多了这个字段，用于存放用户头像url
  };

  // 关闭图标的url
  closeIcoUrl = '';

  // 这个是抄别人的，把dataUrl转换为Blob
  dataURLtoBlob(dataurl) {
    const arr = dataurl.split(',');
    const mime = arr[0].match(/:(.*?);/)[1];
    const bstr = atob(arr[1]);
    let n = bstr.length;
    const u8arr = new Uint8Array(n);
    while (n--) {
        u8arr[n] = bstr.charCodeAt(n);
    }
    return new Blob([u8arr], { type: mime });
  }

  // 获取文件后缀名 (不包括小数点)
  getFileSuffix(file) {
    const index = file.name.lastIndexOf('.');
    return file.name.substr(index + 1);
  }

  // 判断文件是否是图片文件
  isPhotoFile(file) {
    const suffix = this.getFileSuffix(file).toLowerCase();
    return suffix === 'gif' || suffix === 'bmp' || suffix === 'jpg' || suffix === 'jpeg' || suffix === 'png';
  }

  // 检查用户名格式是否合法
  checkUsername(username: string): boolean {
    return this.checkPhone(username) || this.checkEmail(username);
  }

  // 检查登录密码格式是否合法
  checkPassword(password: string) {
    const passWordReg = /^[a-zA-Z0-9_.]{8,15}$/;
    return passWordReg.test(password);
  }

  // 检查注册手机格式是否满足要求
  checkPhone(phone: string) {
    const phoneReg = /^((13[0-9])|(14[5,7])|(15[0-3,5-9])|(17[0,3,5-8])|(18[0-9])|166|198|199|(147))\d{8}$/;
    return phoneReg.test(phone);
  }

  // 检查注册邮箱格式是否满足要求
  checkEmail(email: string) {
    const EmailReg = /^[a-zA-Z0-9_.-]+@[a-zA-Z0-9-]+(\.[a-zA-Z0-9-]+)*\.[a-zA-Z0-9]{2,6}$/;
    return EmailReg.test(email) && email.length <= 30;
  }


  // 检查个人用户名格式是否满足要求
  checkPersonalName(personalName: string) {
    return personalName.length >= 1 && personalName.length <= 10;
  }

  // 检查个人性别是否满足要求
  checkPersonalSex(personalSex: string) {
    return personalSex === '男' || personalSex === '女' || personalSex === '秘密';
  }

  // 检查个人生日是否满足要求
  checkPersonalBirthday(personalBirthday) {
    return !isNaN(personalBirthday) && personalBirthday < new Date().getTime();
  }
  // 通过布尔值返回正确or错误的Ico的URL
  getIcoUrl(flag: boolean): string {
    if (flag === false) {
      return 'assets/img/incorrect.ico';
    } else {
      return 'assets/img/correct.ico';
    }
  }

  // 检测登录状态 (false表示没登陆,true表示已登录)
  loginStateDetection(): boolean {
    // 检测浏览器是否支持storage
    if (!sessionStorage || !localStorage) {
      console.warn('本浏览器不支持storage!\n');
      return false;
    }
    // 从session storage中解析获取用户账户信息
    const information = JSON.parse(sessionStorage.getItem('userAccountInformation'));
    if (information === null) {
      return false;
    }
    return true;
  }

  // 从session storage中解析中用户账户信息
  getUserAccountInformation() {
    const information = JSON.parse(sessionStorage.getItem('userAccountInformation'));
    return information;
  }

  // 从session storage中解析中用户个人信息 (包括了头像的安全链接获取)
  getUserPersonalInformation() {
    let information = JSON.parse(sessionStorage.getItem('userPersonalInformation'));

    // 判断information是否为null,为null则返回一个空信息
    if (information === null) {
      information =  {
        userId: -1,
        userPhoto: '',
        userName: '',
        userSex: '',
        userContactPhone: '',
        userContactEmail: '',
        userBirthday: 0,
        userPhotoUrl: undefined, // 与后端相比多了这个字段，用于存放用户头像url
      };
      return information;
    }
    // 构建安全链接
    if (information.userPhotoUrl === undefined) {
      information.userPhotoUrl = 'assets/img/default.jpg';
    } else {
      const urlKey = 'changingThisBreaksApplicationSecurity';
      information.userPhotoUrl = this.sanitizer.bypassSecurityTrustUrl(information.userPhotoUrl[urlKey]);
    }
    return information;
  }

  // 将用户账户信息存储到session storage中
  storeUserAccountInformation(userAccountInformation) {
    sessionStorage.setItem('userAccountInformation', JSON.stringify(userAccountInformation));
  }

  // 删除用户信息 (包括用户个人信息和用户账户信息)
  clearUserInformation() {
    sessionStorage.clear();
  }

  // 将时间戳转换为对应的时间字符串 (单位 unixnano)
  timestampToTimeString(transTime: number) {
    const date = new Date(transTime);
    const year = date.getFullYear();
    const month = date.getMonth() + 1;
    const d = date.getDate();
    let dString = '' + d;
    let mString = '' + month;
    if (month < 10) {
      mString = '0' + mString;
    }
    if (d < 10) {
      dString = '0' + dString;
    }

    return year + '-' + mString + '-' + dString ;
  }

  constructor(
    private sanitizer: DomSanitizer
  ) { }

  OnInit() {
    this.closeIcoUrl = '/assets/img/close.ico';
  }


}
