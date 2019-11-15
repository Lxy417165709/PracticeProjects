import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginPageComponent } from './login-page/login-page.component';
import { SelfPageComponent } from './self-page/self-page.component';
import { EnterPageComponent } from './enter-page/enter-page.component';
import { MeetingPageComponent } from './meeting-page/meeting-page.component';

@NgModule({
  declarations: [
    AppComponent,
    LoginPageComponent,
    SelfPageComponent,
    EnterPageComponent,
    MeetingPageComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
