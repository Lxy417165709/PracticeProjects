import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { EnterPageComponent } from './enter-page.component';

describe('EnterPageComponent', () => {
  let component: EnterPageComponent;
  let fixture: ComponentFixture<EnterPageComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ EnterPageComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(EnterPageComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
