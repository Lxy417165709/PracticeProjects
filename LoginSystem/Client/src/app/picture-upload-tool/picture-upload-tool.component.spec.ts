import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { PictureUploadToolComponent } from './picture-upload-tool.component';

describe('PictureUploadToolComponent', () => {
  let component: PictureUploadToolComponent;
  let fixture: ComponentFixture<PictureUploadToolComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ PictureUploadToolComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(PictureUploadToolComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
