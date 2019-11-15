import { Component, OnInit } from '@angular/core';
import {Router} from '@angular/router';

@Component({
  selector: 'app-enter-page',
  templateUrl: './enter-page.component.html',
  styleUrls: ['./enter-page.component.css']
})
export class EnterPageComponent implements OnInit {

  constructor(private router: Router) { }
  addBtn;
  enterWindow;
  cancelBtn;
  intoBtn;
  returnBtn;

  ngOnInit() {
        this.enterWindow = document.getElementById('enter_window');

        // console.log(enterWindow);
        // intoBtn.onclick = function(){
        //     window.location.href='finally.html';
        // }
        // addBtn.onclick = function(){
        //     enterWindow.style.display = "block";
        //     // console.log(1);
        // }
        // cancelBtn.onclick = function(){
        //     enterWindow.style.display = "none";
        //     // console.log(1);
        // }
        // var loginBtn = document.getElementById("loginIn");
        // returnBtn.onclick = function () {
        //     window.location.href='self.html';
        // }


  }
  showEnterWindow() {
    this.enterWindow.style.display = 'block';
  }
  hideEnterWindow() {
    this.enterWindow.style.display = 'none';
  }
  toPage (url: string) {
    this.router.navigate([url]);
  }
}
