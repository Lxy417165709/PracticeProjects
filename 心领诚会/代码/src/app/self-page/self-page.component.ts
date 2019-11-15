import { Component, OnInit } from '@angular/core';
import {Router} from '@angular/router';
@Component({
  selector: 'app-self-page',
  templateUrl: './self-page.component.html',
  styleUrls: ['./self-page.component.css']
})
export class SelfPageComponent implements OnInit {

  constructor(private router: Router) { }

  ngOnInit() {
  }
  toPage (url: string) {
    this.router.navigate([url]);
  }

}
