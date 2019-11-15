import { Component, OnInit, NgModule } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Router } from '@angular/router';
import { CommonService } from '../common.service';
import { ToastrService } from 'ngx-toastr';

@Component({
  selector: 'app-login',
  templateUrl: './login.component.html',
  styleUrls: ['./login.component.css']
})
export class LoginComponent implements OnInit {
  loginForm = {
    loginUsername: '',
    loginPassword: '',
  };
  constructor(
    private http: HttpClient,
    private router: Router,
    private common: CommonService,
    private toast: ToastrService,
  ) { }

  ngOnInit() {
    /*
    toast-top-left  顶端左边
    toast-top-right    顶端右边
    toast-top-center  顶端中间
    toast-top-full-width 顶端，宽度铺满整个屏幕
    toast-bottom-right
    toast-bottom-left
    toast-bottom-center
    toast-bottom-full-width
    */
    this.toast.info('欢迎登陆', '小提示');
    // 清空用户信息
    this.common.clearUserInformation();
    this.loginForm = {
      loginUsername: '',
      loginPassword: '',
    };
  }

  // 返回登录表单校验码，0表示正确
  loginFormIsOK() {
    if (!this.common.checkUsername(this.loginForm.loginUsername)) {
      return -1;
    }
    if (!this.common.checkPassword(this.loginForm.loginPassword)) {
      return -2;
    }
    return 0;
  }
  // 解析登录表校验码,返回解析结果
  getLoginFormJudgeResult() {
    let tipStr = '';
    const resultFlag = this.loginFormIsOK();
    if (resultFlag === 0) {
      tipStr = '';  // 表示没有错误
    }
    if (resultFlag === -1) {
      tipStr = '您输入的用户名格式有误!';
    }
    if (resultFlag === -2) {
      tipStr = '您输入的密码格式有误!';
    }
    return tipStr;
  }

  // 发送Http登录请求 (半成品) (业务逻辑还没写)
  askForLogin() {

    // 前端检测登录表单格式是否正确
    const tipStr = this.getLoginFormJudgeResult();
    if (tipStr !== '') {
      this.toast.error(tipStr, '错误提示');
      return ;
    }

    // 检测通过后就可以发送HTTP请求了
    const header = new HttpHeaders({
      'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8'
    });

    // 请求头
    const requestHead = { headers: header };

    // 请求协议 (请求体)
    this.common.reqProto = {
      action: 'POST',                   // 请求类型GET/POST/PUT/DELETE
      data: {
        loginPhone: this.loginForm.loginUsername,
        loginEmail: this.loginForm.loginUsername,
        loginPassword: this.loginForm.loginPassword,
      },      // 请求数据
      // ---- 下面的字段都没用到
      sets: [],
      orderBy: '',  // 排序要求
      filter: '',   // 筛选条件
      page: 0,      // 分页
      pageSize: 0,  // 分页大小
    };

    this.http.post('/server/login', this.common.reqProto, requestHead).subscribe((res: any) => {
      // 返回逻辑还没写
      this.common.replyProto = res;
      // console.log(this.common.replyProto);

      // 根据登录结果相应操作
      if (this.common.replyProto.status === 0) {

        // 存储用户账户信息
        this.common.storeUserAccountInformation(this.common.replyProto.data);
        this.toast.success('登录成功!', '登录提示', {positionClass: 'toast-bottom-right'});
        // 路由到主页
        this.router.navigate(['home']);
        // this.toastr.success('Hello world!', 'Toastr fun!', {timeOut: 3000});
      } else {
        // 登录失败时的提示
        this.toast.warning(res.msg, '登录提示');
      }

    });
  }
}
