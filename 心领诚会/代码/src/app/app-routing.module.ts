import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { LoginPageComponent } from './login-page/login-page.component';
import { SelfPageComponent } from './self-page/self-page.component';
import { EnterPageComponent} from './enter-page/enter-page.component';
import { MeetingPageComponent } from './meeting-page/meeting-page.component';

const routes: Routes = [
  { path: '',
    component: LoginPageComponent
  },
  { path: 'login',
    component: LoginPageComponent
  },
  { path: 'self',
    component: SelfPageComponent
  },
  { path: 'enter',
    component: EnterPageComponent
  },
  { path: 'meeting',
  component: MeetingPageComponent
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
