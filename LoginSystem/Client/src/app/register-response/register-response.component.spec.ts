import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { RegisterResponseComponent } from './register-response.component';

describe('RegisterResponseComponent', () => {
  let component: RegisterResponseComponent;
  let fixture: ComponentFixture<RegisterResponseComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ RegisterResponseComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(RegisterResponseComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
