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
    l_gyro_p = data.pitchLeft;
    r_gyro_p = data.pitchRight;

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

        // // rightForeArm
        // if (flex2Angle > 0 && flex2Angle <= 36) {
        //   rightForeArm.rotation.z = -0.4;
        // } else if (flex2Angle > 36 && flex2Angle <= 72) {
        //   rightForeArm.rotation.z = -0.8;
        // } else if (flex2Angle > 72 && flex2Angle <= 108) {
        //   rightForeArm.rotation.z = -1.2;
        // } else if (flex2Angle > 108 && flex2Angle <= 144) {
        //   rightForeArm.rotation.z = -1.6;
        // } else if (flex2Angle > 144 && flex2Angle <= 180) {
        //   rightForeArm.rotation.z = -2.0;
        // }

        // // leftForeArm
        // if (flex1Angle > 0 && flex1Angle <= 36) {
        //   leftForeArm.rotation.z = 0.4;
        // } else if (flex1Angle > 36 && flex1Angle <= 72) {
        //   leftForeArm.rotation.z = 0.8;
        // } else if (flex1Angle > 72 && flex1Angle <= 108) {
        //   leftForeArm.rotation.z = 1.2;
        // } else if (flex1Angle > 108 && flex1Angle <= 144) {
        //   leftForeArm.rotation.z = 1.6;
        // } else if (flex1Angle > 144 && flex1Angle <= 180) {
        //   leftForeArm.rotation.z = 2;
        // }

        // // left arm
        // if (l_gyro_p > -75 && l_gyro_p <= -60) {
        //   leftArm.rotation.x = -1.0;
        // } else if (l_gyro_p > -60 && l_gyro_p <= -45) {
        //   leftArm.rotation.x = -0.8;
        // } else if (l_gyro_p > -45 && l_gyro_p <= -30) {
        //   leftArm.rotation.x = -0.6;
        // } else if (l_gyro_p > -30 && l_gyro_p <= -15) {
        //   leftArm.rotation.x = -0.4;
        // } else if (l_gyro_p > -15 && l_gyro_p < 0) {
        //   leftArm.rotation.x = -0.2;
        // } else if (l_gyro_p == 0) {
        //   leftArm.rotation.x = 0.0;
        // } else if (l_gyro_p > 0 && l_gyro_p <= 15) {
        //   leftArm.rotation.x = 0.2;
        // } else if (l_gyro_p > 15 && l_gyro_p <= 30) {
        //   leftArm.rotation.x = 0.4;
        // } else if (l_gyro_p > 30 && l_gyro_p <= 45) {
        //   leftArm.rotation.x = 0.6;
        // } else if (l_gyro_p > 45 && l_gyro_p <= 60) {
        //   leftArm.rotation.x = 0.8;
        // } else if (l_gyro_p > 60 && l_gyro_p <= 75) {
        //   leftArm.rotation.x = 1.0;
        // }

        // // right arm
        // if (r_gyro_p > -75 && r_gyro_p <= -60) {
        //   rightArm.rotation.x = -1.0;
        // } else if (r_gyro_p > -60 && r_gyro_p <= -45) {
        //   rightArm.rotation.x = -0.8;
        // } else if (r_gyro_p > -45 && r_gyro_p <= -30) {
        //   rightArm.rotation.x = -0.6;
        // } else if (r_gyro_p > -30 && r_gyro_p <= -15) {
        //   rightArm.rotation.x = -0.4;
        // } else if (r_gyro_p > -15 && r_gyro_p < 0) {
        //   rightArm.rotation.x = -0.2;
        // } else if (r_gyro_p == 0) {
        //   rightArm.rotation.x = 0.0;
        // } else if (r_gyro_p > 0 && r_gyro_p <= 15) {
        //   rightArm.rotation.x = 0.2;
        // } else if (r_gyro_p > 15 && r_gyro_p <= 30) {
        //   rightArm.rotation.x = 0.4;
        // } else if (r_gyro_p > 30 && r_gyro_p <= 45) {
        //   rightArm.rotation.x = 0.6;
        // } else if (r_gyro_p > 45 && r_gyro_p <= 60) {
        //   rightArm.rotation.x = 0.8;
        // } else if (r_gyro_p > 60 && r_gyro_p <= 75) {
        //   rightArm.rotation.x = 1.0;
        // }

        controls.update();

        renderer.render(scene, camera);
      }

      animate();
    }
  );

  const renderer = new THREE.WebGLRenderer({ canvas });
  renderer.setPixelRatio(window.devicePixelRatio);

  const controls = new OrbitControls(camera, renderer.domElement);
}

init();
