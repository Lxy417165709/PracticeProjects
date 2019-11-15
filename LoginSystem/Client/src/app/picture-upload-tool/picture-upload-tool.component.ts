import { Component, OnInit, Output } from '@angular/core';
import { CommonService } from '../common.service';
import { FileUploader } from 'ng2-file-upload';
import { EventEmitter } from '@angular/core';
import { DomSanitizer } from '@angular/platform-browser';
import { ToastrService } from 'ngx-toastr';

@Component({
  selector: 'app-picture-upload-tool',
  templateUrl: './picture-upload-tool.component.html',
  styleUrls: ['./picture-upload-tool.component.css']
})
export class PictureUploadToolComponent implements OnInit {

  // 临时url,用于展示用户选择的图片
  tmpUrl: any;
  // 一个传值器，用于通知父组件关闭该子组件
  @Output() emitter = new EventEmitter();


  // 头像上传器
  uploader: FileUploader = new FileUploader({
    url: '/server/uploadPhoto',
    method: 'POST',
    itemAlias: 'file'
  });

  constructor(
    private common: CommonService,
    private sanitizer: DomSanitizer,
    private toast: ToastrService,
  ) { }

  ngOnInit() {
    // this.userPersonalInformation 在这里现在好像没啥用
    this.common.userAccountInformation = this.common.getUserAccountInformation();
    this.common.userPersonalInformation = this.common.getUserPersonalInformation();
    this.tmpUrl = this.common.userPersonalInformation.userPhotoUrl;
  }

  // 向父组件传值，通知父组件关闭该子组件 (传送0: 表示关闭该子组件)
  callFatherExecClose() {
    this.emitter.emit({
      type: 0,
      data: 0,
    });
  }

  // 向父组件传值，通知父组件修改用户个人信息
  callFatherExecChange() {
    this.common.userPersonalInformation.userPhotoUrl = this.tmpUrl;
    this.emitter.emit({
      type: 1,
      data: this.common.userPersonalInformation,
    });
  }

  // 选择图片发生错误时执行的操作提示
  // 第一个参数是提示语，第二个是图片选择器主体
  selectError(tipStr, input) {
    this.tmpUrl =  this.common.userPersonalInformation.userPhotoUrl;
    this.toast.error(tipStr, '提示');
    input.value = null;
    this.uploader.queue = [];
  }

  // 选择图片后的操作
  listenFileChanged(e) {
    // 当上传器有照片时，我们要去除第一个
    if (this.uploader.queue.length === 2) {
      const wannaElement = this.uploader.queue[1];
      this.uploader.queue = [];
      this.uploader.queue.push(wannaElement);
    }
    if (e.target.files[0] === undefined) {
      this.selectError('您还没有选择头像!', e.target);
      return ;
    }
     // 判断是否是图片
    const isPhoto = this.common.isPhotoFile(e.target.files[0]);
    if (!isPhoto) {
      this.selectError('您选择的文件不是一个图片文件!', e.target);
      return ;
    }
     // 判断图片文件大小,限制图片大小只能在500KB内
    const size = e.target.files[0].size;
    if (size > 0.6 * 1024 * 1024) {
      this.selectError('图片大小只能在500KB(0.5MB)范围内!', e.target);
      return ;
    }

    const url = this.sanitizer.bypassSecurityTrustUrl(URL.createObjectURL(e.target.files[0]));
    this.tmpUrl = url;
    this.toast.success('新头像载入成功!', '提示');
  }

  // 上传图片 (成功了!)
  // 该函数的作用是改变用户头像
  askForChangingUserPhoto() {

    // 用户没有文件上传时
    if (this.uploader.queue[0] === undefined) {
      this.toast.error('您还没有选择任何新头像!', '提示');
      return;
    }
    // 设置上传文件名字 (规定是时间戳)
    this.uploader.queue[0].file.name = new Date().getTime() + '_' + this.common.userAccountInformation.userId
     + '.' + this.common.getFileSuffix(this.uploader.queue[0].file);

    // 开始上传
    this.uploader.queue[0].upload();

    // 成功回调
    this.uploader.queue[0].onSuccess = (res: any) => {
      this.common.replyProto = JSON.parse(res); // 把返回结果存在this.common.replyProto中

      // 这里表示成功了
      if (this.common.replyProto.status === 0) {
        this.toast.success('头像上传成功,即将更改个人信息!', '提示');
        // 修改头像名
        this.common.userPersonalInformation.userPhoto = this.uploader.queue[0].file.name;

        // 清空队列
        this.uploader.queue = [];

        // 通知父组件修改用户个人信息
        this.callFatherExecChange();

        // 通知父组件关闭该子组件
        this.callFatherExecClose();
      } else {
        // 上传失败提醒
        this.toast.warning('头像上传失败, 请重试!', '提示');
      }
    };
  }
}
