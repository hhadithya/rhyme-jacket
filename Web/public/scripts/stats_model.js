import * as THREE from "three";
import { GLTFLoader } from "jsm/loaders/GLTFLoader.js";
import { OrbitControls } from "jsm/controls/OrbitControls.js";

const canvas = document.getElementById("stat_model_canvas");

var connection = new WebSocket("ws://" + location.hostname + ":8080/");

let model, rightArm, leftArm, rightForeArm, leftForeArm;
let flex1Angle, flex2Angle;

connection.onmessage = function (e) {
  e.data.text().then((text) => {
    var data = JSON.parse(text);

    flex1Angle = data.flex1Angle;
    flex2Angle = data.flex2Angle;

    // console.log(flex1Angle, flex2Angle);
  });
};

function init() {
  const scene = new THREE.Scene();
  scene.background = new THREE.Color(0xb0deee);
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

      rightArm = model.getObjectByName("mixamorigRightArm");
      leftArm = model.getObjectByName("mixamorigLeftArm");
      rightForeArm = model.getObjectByName("mixamorigRightForeArm");
      leftForeArm = model.getObjectByName("mixamorigLeftForeArm");

      rightArm.rotation.x = leftArm.rotation.x = 0.95;

      model.scale.set(12, 12, 12);
      model.position.y = 3;

      // console log the names of the bones
      // model.traverse((o) => {
      //   if (o.isBone) {
      //     console.log(o.name);
      //   }
      // });

      scene.add(model);

      function animate() {
        requestAnimationFrame(animate);

        const delta = clock.getDelta(); // Get time passed since last frame

        if (flex2Angle > 0 && flex2Angle <= 36) {
          rightForeArm.rotation.z = -0.2;
        } else if (flex2Angle > 36 && flex2Angle <= 72) {
          rightForeArm.rotation.z = -0.4;
        } else if (flex2Angle > 72 && flex2Angle <= 108) {
          rightForeArm.rotation.z = -0.6;
        } else if (flex2Angle > 108 && flex2Angle <= 144) {
          rightForeArm.rotation.z = -0.8;
        } else if (flex2Angle > 144 && flex2Angle <= 180) {
          rightForeArm.rotation.z = -1;
        }

        // controls.update();

        renderer.render(scene, camera);
      }

      animate();
    }
  );

  const renderer = new THREE.WebGLRenderer({ canvas });
  renderer.setPixelRatio(window.devicePixelRatio);

  // const controls = new OrbitControls(camera, renderer.domElement);
}

init();
