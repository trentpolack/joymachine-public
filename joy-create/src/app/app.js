const { shell, ipcRenderer } = require('electron');
const path = require('path')
const THREE = require('three');
const _ = require('lodash');

const rq = require('electron-require');

//const viewport = rq('./src/viewport/viewport.js');
let viewport = require('../viewport/viewport');
//import Lsystem, { LinkedListToString } from './pcg/lsystem.js'
//import City from './pcg/city.js'
//import ShapeGrammar from './pcg/grammar.js'

var directionalLight;
var sun;

window.addEventListener('load', () => {
    const THREE = require('three');
    const OrbitControls = require('three-orbit-controls')(THREE)
    const Stats = require('stats-js');
    const DAT = require('dat-gui');

    var gui = new DAT.GUI()

    var ambientLight = new THREE.AmbientLight(0xffffff, 0.4);
    directionalLight = new THREE.DirectionalLight(0xffffff, 1);

    var geometry = new THREE.BoxGeometry(10, 10, 10);
    var material = new THREE.MeshBasicMaterial({ color: 0xf8e997 });
    sun = new THREE.Mesh(geometry, material);


    var viewport = {
        gui: gui,
    };

    /*
        ipcRenderer.on('load-mesh', function(event, meshPath) {
            meshDirectory = path.dirname(meshPath) + "/" // HACK, probably should write a better function here
            meshFileName = path.basename(meshPath)
            BABYLON.SceneLoader.Load(meshDirectory, meshFileName, engine, function(newScene) {
                var camera = getDefaultCamera(newScene);
                newScene.render()
            })
        })
    */

    //    // run this function after the window loads
    //    window.addEventListener('load', function() {
    //    var container = document.createElement("div");
    container = document.querySelector("#viewport"); //.appendChild(container);

    /*
    // Stats.
    var stats1, stats2, stats3;
    stats1 = new Stats();
    stats1.showPanel(0); // Panel 0 = fps
    stats1.domElement.style.cssText = "position:absolute;top:0px;left:0px;";
    container.appendChild(stats1.dom);
    stats2 = new Stats();
    stats2.showPanel(1); // Panel 0 = fps
    stats2.domElement.style.cssText = "position:absolute;top:0px;left:80px;";
    container.appendChild(stats2.dom);
    stats3 = new Stats();
    stats3.showPanel(2); // Panel 0 = fps
    stats3.domElement.style.cssText = "position:absolute;top:0px;left:160px;";
    container.appendChild(stats3.dom);

    viewport.stats1 = stats1;
    viewport.stats2 = stats2;
    viewport.stats3 = stats3;
*/
    viewport.container = container;

    var scene = new THREE.Scene();
    scene.background = new THREE.Color(0x101010);

    var camera = new THREE.PerspectiveCamera(40, window.innerWidth / window.innerHeight, 1, 2000);

    renderer = new THREE.WebGLRenderer({ antialias: false });
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.setFaceCulling(THREE.CullFaceNone);
    renderer.gammaInput = true;
    renderer.gammaOutput = false;
    renderer.shadowMap.enabled = true;

    var controls = new OrbitControls(camera, renderer.domElement);
    controls.enableDamping = true;
    controls.enableZoom = true;
    controls.target.set(0, 0, 0);
    controls.rotateSpeed = 0.3;
    controls.zoomSpeed = 1.0;
    controls.panSpeed = 2.0;

    viewport.container.appendChild(renderer.domElement);

    // resize the canvas when the window changes
    window.addEventListener('resize', function() {
        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
        renderer.setSize(window.innerWidth, window.innerHeight);
    }, false);

    // assign THREE.js objects to the object we will return
    viewport.scene = scene;
    viewport.camera = camera;
    viewport.renderer = renderer;

    // begin the animation loop
    (function tick() {
        /*
                stats1.begin();
                stats2.begin();
                stats3.begin();
        */
        onUpdate(renderer);
        renderer.render(scene, camera);
        /*
                stats1.end();
                stats2.end();
                stats3.end();
        */
        requestAnimationFrame(tick);
    })();
});

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

function onLoad(renderer) {
    var { scene, camera, renderer, gui, stats1, stats2, stats3 } = renderer;

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

    directionalLight.position.applyAxisAngle(z, Math.PI / 180);
    sun.position.applyAxisAngle(z, Math.PI / 180);
}