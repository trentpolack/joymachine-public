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
  var scene = makeScene(); // load the null scene
  scene.render();
});

window.addEventListener('resize', () => {
  engine.resize();
});

function getDefaultCamera(forScene) {
  return new BABYLON.FreeCamera("camera1", new BABYLON.Vector3(0, 5, -25), forScene);
}

function makeScene() {
  var scene = new BABYLON.Scene(engine);
  var camera = getDefaultCamera(scene);
  return scene;
}

ipcRenderer.on('load-mesh', function(event, meshPath) {
  meshDirectory = path.dirname(meshPath) + "/" // HACK, probably should write a better function here
  meshFileName = path.basename(meshPath)
  BABYLON.SceneLoader.Load(meshDirectory, meshFileName, engine, function(newScene) {
    var camera = getDefaultCamera(newScene);
    newScene.render()
  })
})
