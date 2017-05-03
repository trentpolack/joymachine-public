// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.

const { shell, ipcRenderer } = require('electron');
const BABYLON = require('./vendor/babylon/babylon.custom.js')
const path = require('path')
const canvas = document.querySelector("#viewport");
var engine = undefined; // ugly global FIXME

window.addEventListener('load', () => {
  engine = new BABYLON.Engine(canvas, true);
  var scene = makeScene();
  scene.render();
});

window.addEventListener('resize', () => {
  engine.resize();
});

function makeScene() {
  var scene = new BABYLON.Scene(engine);
  var camera = new BABYLON.FreeCamera("camera1", new BABYLON.Vector3(0, 5, -10), scene);
  return scene;
}

ipcRenderer.on('load-mesh', function(event, meshPath) {
  meshDirectory = path.dirname(meshPath)
  meshFileName = path.basename(meshPath)
  // TODO: Load mesh with Babylon
})
