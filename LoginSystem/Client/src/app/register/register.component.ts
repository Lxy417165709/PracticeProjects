import { Component, OnInit, NgModule } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Router } from '@angular/router';
import { CommonService } from '../common.service';
import { flushMicrotasks } from '@angular/core/testing';
import { ToastrService } from 'ngx-toastr';

@Component({
  selector: 'app-register',
  templateUrl: './register.component.html',
  styleUrls: ['./register.component.css']
})
export class RegisterComponent implements OnInit {
  registerInformation = {
    registerPhone: '',
    registerEmail: '',
    registerPassword: '',
    registerRepeatPassword: '',
    registerType: 0,
    registerTime: 0,
    lastLoginTime: 0,
  };

  constructor(
    private http: HttpClient,
    private router: Router,
    private common: CommonService,
    private toast: ToastrService
  ) { }

  ngOnInit() {
  }

  // 返回注册表校验码，0表示正确
  registerFormIsOK() {
    if (!this.common.checkPhone(this.registerInformation.registerPhone)) {
      return -1;
    }
    if (!this.common.checkEmail(this.registerInformation.registerEmail)) {
      return -2;
    }
    if (!this.common.checkPassword(this.registerInformation.registerPassword)) {
      return -3;
    }
    if (!this.common.checkPassword(this.registerInformation.registerRepeatPassword)) {
      return -4;
    }
    if (this.registerInformation.registerPassword !== this.registerInformation.registerRepeatPassword) {
      return -5;
    }
    return 0;
  }
  // 解析注册表校验码
  getRegisterFormJudgeResult() {
    let tipStr = '';
    const resultFlag = this.registerFormIsOK();
    if (resultFlag === 0) {
      tipStr = '';  // 表示没有错误
    }
    if (resultFlag === -1) {
      tipStr = '您输入的手机格式有误!';
    }
    if (resultFlag === -2) {
      tipStr = '您输入的邮箱格式有误!';
    }
    if (resultFlag === -3) {
      tipStr = '您输入的密码格式有误!';
    }
    if (resultFlag === -4) {
      tipStr = '您输入的二次密码格式有误!';
    }
    if (resultFlag === -5) {
      tipStr = '您两次输入的密码不一致';
    }
    return tipStr;
  }


  // 发送Http注册请求 (半成品) (业务逻辑还没写)
  askForRegister() {
    // 先在前端进行合法性检测,获取提示信息
    const tipStr =  this.getRegisterFormJudgeResult();
    // 如果提示信息是空，则表示用户填写的格式是正确的
    if (tipStr !== '') {
      this.toast.error(tipStr, '提示', {timeOut: 4000});
      return;
    }
    const header = new HttpHeaders({
      'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8'
    });
    // 请求头
    const requestHead = { headers: header };

    // 请求协议
    const nowTime =  new Date().getTime();  // nowTime是13位的
    this.registerInformation.lastLoginTime = nowTime;
    this.registerInformation.registerTime = nowTime;
    this.common.reqProto = {
      action: 'POST',                   // 请求类型GET/POST/PUT/DELETE
      data: this.registerInformation,   // 请求数据
      // ---- 下面的字段都没用到
      sets: [],
      orderBy: '',  // 排序要求
      filter: '',   // 筛选条件
      page: 0,      // 分页
      pageSize: 0,  // 分页大小
    };

    // 发送注册请求
    this.http.post('/server/register', this.common.reqProto, requestHead).subscribe((res: any) => {
      // 成功了！ 但是这的业务逻辑还有好多
      this.common.replyProto = res;

      // 状态码为0表示成功
      if (res.status === 0) {
        this.toast.success('注册成功!', '提示', {timeOut: 4000});
        this.common.storeUserAccountInformation(this.common.replyProto.data);

        // 页面跳转
        this.router.navigate(['registerResponse']);
      } else {
        // 输出响应信息字段
        this.toast.warning(res.msg, '提示');
      }
    });
  }
}
