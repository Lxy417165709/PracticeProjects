import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponent } from './login/login.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { ToastrModule } from 'ngx-toastr';
import { RegisterComponent } from './register/register.component';
import { RegisterResponseComponent } from './register-response/register-response.component';
import { PersonalInformationComponent } from './personal-information/personal-information.component';
import { PictureUploadToolComponent } from './picture-upload-tool/picture-upload-tool.component';
import { HomeComponent } from './home/home.component';
import { HttpClientModule } from '@angular/common/http';
import { FormsModule} from '@angular/forms';
// import { FileUploadModule } from 'ng2-file-upload';
import { CommonService } from './common.service';
import { FileUploadModule } from 'ng2-file-upload';
@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    RegisterComponent,
    RegisterResponseComponent,
    PersonalInformationComponent,
    PictureUploadToolComponent,
    HomeComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    FormsModule,
    FileUploadModule,
    BrowserAnimationsModule,
    ToastrModule.forRoot({
      timeOut: 3000,
      positionClass: 'toast-top-center',
      preventDuplicates: true,
    }),
  ],
  providers: [
    CommonService,
  ],
  bootstrap: [AppComponent]
})
export class AppModule { }
