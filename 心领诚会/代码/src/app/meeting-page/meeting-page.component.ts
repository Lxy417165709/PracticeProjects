import { Component, OnInit } from '@angular/core';
import {Router} from '@angular/router';
import * as BABYLON from 'babylonjs';
import { Engine, Scene, ArcRotateCamera, HemisphericLight, Vector3, MeshBuilder, Mesh } from 'babylonjs';
@Component({
  selector: 'app-meeting-page',
  templateUrl: './meeting-page.component.html',
  styleUrls: ['./meeting-page.component.css']
})
export class MeetingPageComponent implements OnInit {

  constructor(private router: Router) { }
  ngOnInit() {
    const canvas = <HTMLCanvasElement>document.getElementById('renderCanvas');
    const engine = new BABYLON.Engine(canvas, true);
    const createScene = function() {
      // 创建一个基本的Scene对象，用来容纳所有其他对象
      const scene = new BABYLON.Scene(engine);
      scene.clearColor = new BABYLON.Color4(0, 0, 0, 0);
      // scene.ambientColor = new BABYLON.Color3(100, 0, 0);
      // 创建一个相机，设置其位置为(x:0, y:5, z:-10)
      // const camera = new BABYLON.FreeCamera('camera1', new BABYLON.Vector3(0, 1, 100), scene);
      // 相机聚焦在场景原点位置
      // camera.setTarget(BABYLON.Vector3.Zero());
      // 参数：纵向旋转角度alpha、横向旋转角度beta、半径、目标位置、所属场景
      const camera = new BABYLON.ArcRotateCamera('Camera', 0, 0, 10, new BABYLON.Vector3(0, 150, 0), scene);
      // 这是相机的位置，覆盖相机的alpha、beta、半径值
      camera.setPosition(new BABYLON.Vector3(550, 100, 50));
      // 将相机和画布关联
      camera.attachControl(canvas, true);
      // 创建一个半球形光源,朝向为天空
      const light = new BABYLON.HemisphericLight('light1', new BABYLON.Vector3(0, 100, 0), scene);
      // // 创建一个内置的“球”体；其构造函数的参数：名称、宽度、深度、精度，场景，其中精度表示表面细分数。
      // const sphere = BABYLON.Mesh.CreateSphere('sphere1', 16, 2, scene);
      // // 设置球体位置，使其位于平面之上
      // sphere.position.y = 1;
      // // 创建一个内置的“地面”形状；其构造函数的5个参数和球体一样
      // const ground = BABYLON.Mesh.CreateGround('ground1', 6, 6, 2, scene);
      return scene;
    };
    const myScene = createScene();
    const car = BABYLON.SceneLoader.ImportMesh('', 'assets/babylon/car/', 'car.babylon', myScene
    , function (newMeshes, particleSystems, skeletons) {
    });
    
    // BABYLON.SceneLoader.Append('../../assets/car/', 'car.max', myScene);
    engine.runRenderLoop(function() {
      myScene.render();
    });
  }
 
  toPage(url: string) {
    this.router.navigate([url]);
  }

}
