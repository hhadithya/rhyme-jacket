import * as THREE from "three";
import { GLTFLoader } from "jsm/loaders/GLTFLoader.js";
// import { OrbitControls } from "jsm/controls/OrbitControls.js";

const canvas = document.getElementById("stat_model_canvas");

let model;

function init() {
  const scene = new THREE.Scene();
  scene.background = new THREE.Color(0xB0DEEE);
  const camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    1000
  );
  camera.position.z = 18;
  camera.position.x = 0;
  camera.position.y = 12;

  const light = new THREE.HemisphereLight(0xffffff, 0xffffff, 6);
  light.position.set(0, 1, 0);
  scene.add(light);

  const loader = new GLTFLoader();
  loader.load(
    "https://threejs.org/examples/models/gltf/Michelle.glb",
    function (gltf) {
      model = gltf.scene;

      model.scale.set(12, 12, 12);
      model.position.y = 3;

      scene.add(model);

      function animate() {
        requestAnimationFrame(animate);

        // controls.update();

        renderer.render(scene, camera);
      }

      animate();
    }
  );

  const renderer = new THREE.WebGLRenderer({ canvas });
  renderer.setPixelRatio(window.devicePixelRatio);

  //   const controls = new OrbitControls(camera, renderer.domElement);
}

init();
