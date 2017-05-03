// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const { shell } = require('electron');
const BABYLON = require('./vendor/babylon/babylon.custom.js')
const canvas = document.querySelector("#viewport");

window.addEventListener('load', () => {
  var engine = new BABYLON.Engine(canvas, true);
  var scene = new BABYLON.Scene(engine);
  var camera = new BABYLON.FreeCamera("camera1", new BABYLON.Vector3(0, 5, -10), scene);
  scene.render();
});

window.addEventListener('resize', () => {
  engine.resize();
});