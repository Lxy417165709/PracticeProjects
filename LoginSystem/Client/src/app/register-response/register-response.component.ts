import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { ToastrService } from 'ngx-toastr';

@Component({
  selector: 'app-register-response',
  templateUrl: './register-response.component.html',
  styleUrls: ['./register-response.component.css']
})
export class RegisterResponseComponent implements OnInit {

  constructor(
    private router: Router,
    private toast: ToastrService
  ) { }

  ngOnInit() {
    // 设置定时器，准备跳转到用户个人主页
    let i = 3;
    const timer = setInterval(() => {
      this.toast.info(i + '秒后跳转到个人主页~', '提示', {timeOut: 2000});
      i--;
      if (i === 0) {
        this.router.navigate(['home']);
        clearInterval(timer);
      }
    }, 1000);
  }
}
