const { shell, ipcRenderer } = require('electron');
const path = require('path')
const THREE = require('three');
const _ = require('lodash');

const rq = require('electron-require');

//const viewport = rq('./src/viewport/viewport.js');
var app = {};
var viewport = require('../viewport/viewport');
//import Lsystem, { LinkedListToString } from './pcg/lsystem.js'
//import City from './pcg/city.js'
//import ShapeGrammar from './pcg/grammar.js'

window.addEventListener('load', () => {
    app.ambientLight = new THREE.AmbientLight(0xffffff, 0.4);
    app.directionalLight = new THREE.DirectionalLight(0xffffff, 1);

    app.geometry = new THREE.BoxGeometry(10, 10, 10);
    var material = new THREE.MeshBasicMaterial({ color: 0xf8e997 });
    app.sun = new THREE.Mesh(app.geometry, material);

    // Let's toss a torus on there too.
    var standardMaterial = new THREE.MeshStandardMaterial({
        map: null,
        color: 0xffffff,
        metalness: 1.0
    });
    var torusGeometry = new THREE.TorusKnotGeometry(18, 8, 150, 20);
    app.torusMesh1 = new THREE.Mesh(torusGeometry, standardMaterial);
    app.torusMesh1.position.x = -18.0;
    app.torusMesh1.castShadow = true;
    app.torusMesh1.receiveShadow = true;

    viewport.initViewport(onLoad, onUpdate);
});

function renderLight(scene) {
    var directionalLightHelper = new THREE.DirectionalLightHelper(app.directionalLight, 5);
    var cameraHelper = new THREE.CameraHelper(app.directionalLight.shadow.camera);

    app.directionalLight.color.setHSL(0.1, 1, 0.95);
    app.directionalLight.position.set(-100, -100, 20);
    app.directionalLight.castShadow = true;
    app.directionalLight.shadow.camera.scale.multiplyScalar(2);

    app.sun.translateX(-100);
    app.sun.translateY(-100);
    app.sun.translateZ(20);

    scene.add(app.directionalLight);
    scene.add(app.ambientLight);
    scene.add(app.sun);
    scene.add(app.torusMesh1);
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

function onLoad(viewport) {
    var { scene, camera, renderer, gui, stats1, stats2, stats3 } = viewport;

    renderLight(scene);
    setCamera(camera);
    setShadows(renderer, camera);

    /*
        var shapeGrammar = new ShapeGrammar(scene);
        var city = new City(scene, shapeGrammar);

        city.renderBase();
        city.renderRings();
        city.renderDivisions();
        city.renderCells();
        city.renderBuildings();
    */
}

function onUpdate(renderer) {
    var z = new THREE.Vector3(0, 0, 1);

    app.directionalLight.position.applyAxisAngle(z, Math.PI / 180);
    app.sun.position.applyAxisAngle(z, Math.PI / 180);
}