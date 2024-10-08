import * as THREE from "three";
import { GLTFLoader } from "jsm/loaders/GLTFLoader.js";
import { OrbitControls } from "jsm/controls/OrbitControls.js";

const canvas = document.getElementById("stat_model_canvas");

var connection = new WebSocket("ws://" + location.hostname + ":8080/");

let model, rightArm, leftArm, rightForeArm, leftForeArm;
let flex1Angle,
  flex2Angle,
  l_gyro_p,
  r_gyro_p,
  l_gyro_y,
  r_gyro_y,
  m_gyro_y,
  spine;

connection.onmessage = function (e) {
  e.data.text().then((text) => {
    var data = JSON.parse(text);

    flex1Angle = data.flex1Angle;
    flex2Angle = data.flex2Angle;
    l_gyro_p = data.pitchLeft;
    l_gyro_y = data.yawLeft;
    r_gyro_p = data.pitchRight;
    r_gyro_y = data.yawRight;
    m_gyro_y = data.yawCenter;
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
      spine = model.getObjectByName("mixamorigSpine");

      rightArm.rotation.x = leftArm.rotation.x = 0;
      // leftArm.rotation.z = -1;
      // spine.rotation.z = -1;

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

        // rightForeArm
        if (flex2Angle > 0 && flex2Angle <= 15) {
          rightForeArm.rotation.z = -0.2;
        } else if (flex2Angle > 15 && flex2Angle <= 30) {
          rightForeArm.rotation.z = -0.4;
        } else if (flex2Angle > 30 && flex2Angle <= 45) {
          rightForeArm.rotation.z = -0.6;
        } else if (flex2Angle > 45 && flex2Angle <= 60) {
          rightForeArm.rotation.z = -0.8;
        } else if (flex2Angle > 60 && flex2Angle <= 75) {
          rightForeArm.rotation.z = -1.0;
        } else if (flex2Angle > 75 && flex2Angle <= 90) {
          rightForeArm.rotation.z = -1.2;
        } else if (flex2Angle > 90 && flex2Angle <= 105) {
          rightForeArm.rotation.z = -1.4;
        } else if (flex2Angle > 105 && flex2Angle <= 120) {
          rightForeArm.rotation.z = -1.6;
        } else if (flex2Angle > 120 && flex2Angle <= 135) {
          rightForeArm.rotation.z = -1.8;
        } else if (flex2Angle > 135 && flex2Angle <= 150) {
          rightForeArm.rotation.z = -2.0;
        }

        leftForeArm;
        if (flex1Angle > 0 && flex1Angle <= 15) {
          leftForeArm.rotation.z = 0.2;
        } else if (flex1Angle > 15 && flex1Angle <= 30) {
          leftForeArm.rotation.z = 0.4;
        } else if (flex1Angle > 30 && flex1Angle <= 45) {
          leftForeArm.rotation.z = 0.6;
        } else if (flex1Angle > 45 && flex1Angle <= 60) {
          leftForeArm.rotation.z = 0.8;
        } else if (flex1Angle > 60 && flex1Angle <= 75) {
          leftForeArm.rotation.z = 1.0;
        } else if (flex1Angle > 75 && flex1Angle <= 90) {
          leftForeArm.rotation.z = 1.2;
        } else if (flex1Angle > 90 && flex1Angle <= 105) {
          leftForeArm.rotation.z = 1.4;
        } else if (flex1Angle > 105 && flex1Angle <= 120) {
          leftForeArm.rotation.z = 1.6;
        } else if (flex1Angle > 120 && flex1Angle <= 135) {
          leftForeArm.rotation.z = 1.8;
        } else if (flex1Angle > 135 && flex1Angle <= 150) {
          leftForeArm.rotation.z = 2.0;
        }

        // left arm ------------------------------------------------------------------------------------
        // pitch
        if (l_gyro_p <= -60) {
          leftArm.rotation.x = 1.0;
        } else if (l_gyro_p > -60 && l_gyro_p <= -45) {
          leftArm.rotation.x = 0.8;
        } else if (l_gyro_p > -45 && l_gyro_p <= -30) {
          leftArm.rotation.x = 0.6;
        } else if (l_gyro_p > -30 && l_gyro_p <= -15) {
          leftArm.rotation.x = 0.4;
        } else if (l_gyro_p > -15 && l_gyro_p <= -5) {
          leftArm.rotation.x = 0.2;
        } else if (l_gyro_p > -5 && l_gyro_p <= 5) {
          leftArm.rotation.x = 0.0;
        } else if (l_gyro_p > 5 && l_gyro_p <= 15) {
          leftArm.rotation.x = -0.2;
        } else if (l_gyro_p > 15 && l_gyro_p <= 30) {
          leftArm.rotation.x = -0.4;
        } else if (l_gyro_p > 30 && l_gyro_p <= 45) {
          leftArm.rotation.x = -0.6;
        } else if (l_gyro_p > 45 && l_gyro_p <= 60) {
          leftArm.rotation.x = -0.8;
        } else if (l_gyro_p < 60) {
          leftArm.rotation.x = -1.0;
        }
        // // ---------------------------------------------------------------------------

        // right arm------------------------------------------------------------------
        // pitch
        if (r_gyro_p <= -60) {
          rightArm.rotation.x = 1.0;
        } else if (r_gyro_p > -60 && r_gyro_p <= -45) {
          rightArm.rotation.x = 0.8;
        } else if (r_gyro_p > -45 && r_gyro_p <= -30) {
          rightArm.rotation.x = 0.6;
        } else if (r_gyro_p > -30 && r_gyro_p <= -15) {
          rightArm.rotation.x = 0.4;
        } else if (r_gyro_p > -15 && r_gyro_p < -5) {
          rightArm.rotation.x = 0.2;
        } else if (r_gyro_p > -5 && r_gyro_p <= 5) {
          rightArm.rotation.x = 0.0;
        } else if (r_gyro_p > 0 && r_gyro_p <= 15) {
          rightArm.rotation.x = -0.2;
        } else if (r_gyro_p > 15 && r_gyro_p <= 30) {
          rightArm.rotation.x = -0.4;
        } else if (r_gyro_p > 30 && r_gyro_p <= 45) {
          rightArm.rotation.x = -0.6;
        } else if (r_gyro_p > 45 && r_gyro_p <= 60) {
          rightArm.rotation.x = -0.8;
        } else if (r_gyro_p > 60) {
          rightArm.rotation.x = -1.0;
        }
        // // -------------------------------------------------------------------------------

        // spine
        if (m_gyro_y <= 58 && m_gyro_y > 40) {
          spine.rotation.z = 0.7;
        } else if (m_gyro_y <= 40 && m_gyro_y > 22.5) {
          spine.rotation.z = 0.46;
        } else if (m_gyro_y <= 22.5 && m_gyro_y > 10) {
          spine.rotation.z = 0.23;
        } else if (m_gyro_y <= 10 && m_gyro_y > -10) {
          spine.rotation.z = 0;
        } else if (m_gyro_y <= -10 && m_gyro_y > -22.5) {
          spine.rotation.z = -0.23;
        } else if (m_gyro_y <= -22.5 && m_gyro_y > -40) {
          spine.rotation.z = -0.46;
        } else if (m_gyro_y <= -40 && m_gyro_y > -58) {
          spine.rotation.z = -0.7;
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
