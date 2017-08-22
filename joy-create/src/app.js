const THREE = require('three');
const _ = require('lodash');

import Renderer from './renderer.js'
import Lsystem, { LinkedListToString } from './pcg/lsystem.js'
import City from './pcg/city.js'
import ShapeGrammar from './pcg/grammar.js'

var ambientLight = new THREE.AmbientLight(0xffffff, 0.4);
var directionalLight = new THREE.DirectionalLight(0xffffff, 1);

var geometry = new THREE.BoxGeometry(10, 10, 10);
var material = new THREE.MeshBasicMaterial({ color: 0xf8e997 });
var sun = new THREE.Mesh(geometry, material);

function renderLight(scene) {
    var directionalLightHelper = new THREE.DirectionalLightHelper(directionalLight, 5);
    var cameraHelper = new THREE.CameraHelper(directionalLight.shadow.camera);

    directionalLight.color.setHSL(0.1, 1, 0.95);
    directionalLight.position.set(-100, -100, 20);
    directionalLight.castShadow = true;
    directionalLight.shadow.camera.scale.multiplyScalar(2);

    sun.translateX(-100);
    sun.translateY(-100);
    sun.translateZ(20);

    scene.add(directionalLight);
    scene.add(ambientLight);
    scene.add(sun);
    // scene.add(directionalLightHelper);
    // scene.add(cameraHelper);

}

function setCamera(camera) {
    camera.position.set(0, -100, 30);
    camera.lookAt(new THREE.Vector3(0, 0, 0));
}

function setShadows(renderer, camera) {
    renderer.shadowMap.enabled = true;
    renderer.shadowMapSoft = false;
}

function onLoad(app) {
    var { scene, camera, renderer, gui, stats1, stats2, stats3 } = app;

    renderLight(scene);
    setCamera(camera);
    setShadows(renderer, camera);

    var shapeGrammar = new ShapeGrammar(scene);
    var city = new City(scene, shapeGrammar);

    city.renderBase();
    city.renderRings();
    city.renderDivisions();
    city.renderCells();
    city.renderBuildings();

}

function onUpdate(app) {
    var z = new THREE.Vector3(0, 0, 1);

    directionalLight.position.applyAxisAngle(z, Math.PI / 180);
    sun.position.applyAxisAngle(z, Math.PI / 180);
}

Renderer.init(onLoad, onUpdate);