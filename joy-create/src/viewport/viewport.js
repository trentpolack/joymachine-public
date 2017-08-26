'use strict';

const THREE = require('three');
const OrbitControls = require('three-orbit-controls')(THREE)
const DAT = require('dat-gui');

const Stats = require('stats-js');

//function initViewport(callback, update) {
export default (callback, update) => {
    var gui = new DAT.GUI()

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
    var container = document.createElement("div");
    viewport.container = document.querySelector("#viewport").appendChild(container);

    // Stats.
    viewport.stats1 = new Stats();
    viewport.stats1.setMode(0); // Panel 0 = fps
    viewport.stats1.domElement.style.cssText = "position:absolute;top:0px;left:0px;";
    viewport.container.appendChild(viewport.stats1.domElement);
    viewport.stats2 = new Stats();
    viewport.stats2.setMode(1); // Panel 0 = fps
    viewport.stats2.domElement.style.cssText = "position:absolute;top:0px;left:80px;";
    viewport.container.appendChild(viewport.stats2.domElement);
    viewport.stats3 = new Stats();
    viewport.stats3.setMode(2); // Panel 0 = fps
    viewport.stats3.domElement.style.cssText = "position:absolute;top:0px;left:160px;";
    viewport.container.appendChild(viewport.stats3.domElement);

    viewport.scene = new THREE.Scene();
    viewport.scene.background = new THREE.Color(0x101010);

    viewport.camera = new THREE.PerspectiveCamera(40, window.innerWidth / window.innerHeight, 1, 2000);
    viewport.camera.position.set(0.0, 35, 35 * 3.5);

    viewport.renderer = new THREE.WebGLRenderer({ antialias: false });
    viewport.renderer.setPixelRatio(window.devicePixelRatio);
    viewport.renderer.setSize(window.innerWidth, window.innerHeight);
    viewport.renderer.setFaceCulling(THREE.CullFaceNone);
    viewport.renderer.gammaInput = true;
    viewport.renderer.gammaOutput = false;
    viewport.renderer.shadowMap.enabled = true;

    var controls = new OrbitControls(viewport.camera, viewport.renderer.domElement);
    controls.enableDamping = true;
    controls.enableZoom = true;
    controls.target.set(0, 0, 0);
    controls.rotateSpeed = 0.3;
    controls.zoomSpeed = 1.0;
    controls.panSpeed = 2.0;

    viewport.container.appendChild(viewport.renderer.domElement);

    // resize the canvas when the window changes
    window.addEventListener('resize', function() {
        viewport.camera.aspect = window.innerWidth / window.innerHeight;
        viewport.camera.updateProjectionMatrix();
        viewport.renderer.setSize(window.innerWidth, window.innerHeight);
    }, false);

    // begin the animation loop
    (function tick() {
        viewport.stats1.begin();
        viewport.stats2.begin();
        viewport.stats3.begin();

        update(viewport.renderer);
        viewport.renderer.render(viewport.scene, viewport.camera);

        viewport.stats1.end();
        viewport.stats2.end();
        viewport.stats3.end();
        requestAnimationFrame(tick);
    })();

    // Pass all of the necessary renderer/scene/gui references to the callback for this.
    return callback(viewport);
}

//module.exports = {
//    initViewport: initViewport
//};
