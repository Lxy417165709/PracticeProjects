import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { CommonService } from '../common.service';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { DomSanitizer } from '@angular/platform-browser';
import { ToastrService } from 'ngx-toastr';
@Component({
  selector: 'app-personal-information',
  templateUrl: './personal-information.component.html',
  styleUrls: ['./personal-information.component.css']
})
export class PersonalInformationComponent implements OnInit {
  openState = 0; // 用于显示选择照片子组件   0表示隐藏子组件,1表示打开

  constructor(
    private common: CommonService,
    private router: Router,
    private http: HttpClient,
    private sanitizer: DomSanitizer,
    private toast: ToastrService,
  ) { }

  ngOnInit() {
    // 初始化信息
    this.openState = 0;

    // 检测登录状态
    if (this.common.loginStateDetection() === false) {
      this.toast.error('您还没登录', '提示');
      this.router.navigate(['login']);
      return;
    }

    // 获取用户个人信息
    this.common.userPersonalInformation = this.common.getUserPersonalInformation();
  }

  // 打开选择照片的窗口(该窗口是一个组件)
  openSelectPictureWindow() {
    this.openState = 1;
  }

  // 从子组件中获取消息 (子组件会传送一个 0 的值过来，表示关闭子组件)
  getMsgFromSon(Msg) {
    // 表示关闭子组件
    if (Msg.type === 0) {
      this.openState = Msg.data;
      return ;
    }
    // 表示更新用户个人信息
    if (Msg.type === 1) {
      this.common.userPersonalInformation = Msg.data;  // 已规定好，子组件把更新的目的信息交予父组件
      this.askForChangingUserPersonalInformation();
      return ;
    }
  }


  // 由于用户个人信息的生日保存形式是时间戳，因此用户修改个人生日的时候也要修改相应的时间戳
  listenUserBirthdayChanged(e) {
    this.common.userPersonalInformation.userBirthday = new Date(e.target.value).getTime();
  }

  // 返回个人信息表单校验码，0表示正确
  personalUserFormIsOK() {
    if (!this.common.checkPersonalName(this.common.userPersonalInformation.userName)) {
      return -1;
    }
    if (!this.common.checkPersonalSex(this.common.userPersonalInformation.userSex)) {
      return -2;
    }
    if (!this.common.checkPersonalBirthday(this.common.userPersonalInformation.userBirthday)) {
      return -3;
    }
    if (!this.common.checkPhone(this.common.userPersonalInformation.userContactPhone)) {
      return -4;
    }
    if (!this.common.checkEmail(this.common.userPersonalInformation.userContactEmail)) {
      return -5;
    }
    return 0;
  }
  // 解析个人信息校验码,返回解析结果
  getPersonalUserJudgeResult() {
    let tipStr = '';
    const resultFlag = this.personalUserFormIsOK();
    if (resultFlag === 0) {
      tipStr = '';  // 表示没有错误
    }
    if (resultFlag === -1) {
      tipStr = '您输入的用户名格式有误!';
    }
    if (resultFlag === -2) {
      tipStr = '性别只能填写: 男、女或秘密!';
    }
    if (resultFlag === -3) {
      tipStr = '您选择的生日信息有误!';
    }
    if (resultFlag === -4) {
      tipStr = '您填写的手机格式有误!';
    }
    if (resultFlag === -5) {
      tipStr = '您填写的邮箱格式有误! ';
    }
    return tipStr;
  }

  // 请求修改用户个人信息 (半成品)(还有很多业务逻辑没有完成)
  askForChangingUserPersonalInformation() {
    // 前端信息格式检查
    const tipStr = this.getPersonalUserJudgeResult();
    if (tipStr !== '') {
      this.toast.error(tipStr, '提示');
      return;
    }

    // 发送修改个人信息请求
    const header = new HttpHeaders({
      'Content-Type': 'application/x-www-form-urlencoded;charset=UTF-8'
    });

    // 请求头
    const requestHead = { headers: header };

    // 请求协议 (请求体)
    this.common.reqProto = {
      action: 'POST',
      data: {
        userId: this.common.userPersonalInformation.userId,
        userPhoto: this.common.userPersonalInformation.userPhoto,
        userName: this.common.userPersonalInformation.userName,
        userSex: this.common.userPersonalInformation.userSex,
        userContactPhone: this.common.userPersonalInformation.userContactPhone,
        userContactEmail: this.common.userPersonalInformation.userContactEmail,
        userBirthday: this.common.userPersonalInformation.userBirthday,
          // 请求数据是用户的所有信息
      },
      // ---- 下面的字段都没用到
      sets: [],
      orderBy: '',  // 排序要求
      filter: '',   // 筛选条件
      page: 0,      // 分页
      pageSize: 0,  // 分页大小
    };
    this.http.post('/server/updateUserPersonalInformation', this.common.reqProto, requestHead).subscribe((res: any) => {

      // 返回逻辑还有很多没考虑
      this.common.replyProto = res;

      // 根据返回状态执行相应操作 (0 表示成功)
      if (this.common.replyProto.status === 0) {

        // 这里表示修改失败
        this.toast.success('修改成功!', '提示');
        // 用户的头像base64
        this.common.userPersonalInformation = res.data.UserPersonalInformation;

        // 获取dataUrl
        const dataURL = 'data:image/jpeg;base64,' + res.data.UserPhotoData ;

        // 获取blobURL对象
        const blobURLObject = this.sanitizer.bypassSecurityTrustUrl(URL.createObjectURL(this.common.dataURLtoBlob(dataURL)));

        // 获取blobUrl字符串并构成安全链接(不安全会报错)
        this.common.userPersonalInformation.userPhotoUrl = blobURLObject;

        // 存储用户个人信息到sessionStorage (成功了)
        sessionStorage.setItem('userPersonalInformation', JSON.stringify(this.common.userPersonalInformation));
      } else {
        // 这里表示修改失败
        this.toast.warning(res.msg, '提示');
      }
      // 获取用户个人信息(从sessionStorage中获取)
      this.common.userPersonalInformation = this.common.getUserPersonalInformation();

    });
  }
}

