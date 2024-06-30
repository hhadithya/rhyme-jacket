import * as THREE from "three";
import { GLTFLoader } from "jsm/loaders/GLTFLoader.js";
import { OrbitControls } from "jsm/controls/OrbitControls.js";

const canvas = document.getElementById("level1_model_canvas");
let rightArm, leftArm, model, rightArm2, leftArm2, model2;
let audioBuffer, audioContext, audioSource;
let partDuration;
let currentPart = 0;
let isRaised = true;
let previousIsRaised = isRaised;
let targetAngle = 0;
const music1_url = "../music/level_1_music.mp3"; // Adjust the path as necessary

async function loadAudio() {
  if (!music1_url) return;

  try {
    const response = await fetch(music1_url);
    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }
    const arrayBuffer = await response.arrayBuffer();
    audioContext = new (window.AudioContext || window.webkitAudioContext)();
    audioContext.decodeAudioData(
      arrayBuffer,
      (buffer) => {
        audioBuffer = buffer;
        partDuration = audioBuffer.duration / 21;
      },
      (error) => {
        console.error("decodeAudioData error:", error);
      }
    );
  } catch (error) {
    console.error("Error loading audio:", error);
  }
}

function playNextPart() {
  if (!audioBuffer) return;

  if (audioSource) {
    audioSource.stop();
  }

  const startTime = currentPart * partDuration;
  const endTime = partDuration;

  audioSource = audioContext.createBufferSource();
  audioSource.buffer = audioBuffer;
  audioSource.connect(audioContext.destination);

  audioSource.start(0, startTime, endTime);

  currentPart = (currentPart + 1) % 21;
}

function init() {
  loadAudio();

  const scene = new THREE.Scene();
  scene.background = new THREE.Color(0xdddddd);
  const clock = new THREE.Clock();

  const w = window.innerWidth;
  const h = window.innerHeight;
  const camera = new THREE.PerspectiveCamera(75, w / h, 0.1, 1000);
  camera.position.z = 18;
  camera.position.x = 0;
  camera.position.y = 11;

  const light = new THREE.HemisphereLight(0xffffff, 0xffffff);
  light.position.set(0, 1, 0);
  scene.add(light);

  const loader = new GLTFLoader();
  loader.load(
    "https://threejs.org/examples/models/gltf/Michelle.glb",
    function (gltf) {
      model = gltf.scene;

      rightArm = model.getObjectByName("mixamorigRightArm");
      leftArm = model.getObjectByName("mixamorigLeftArm");

      model.scale.set(12, 12, 12);
      model.position.y = 0;
      scene.add(model);

      function animate() {
        const delta = clock.getDelta(); // Get time passed since last frame

        // Smoothly interpolate towards the target angle
        if (rightArm) {
          rightArm.rotation.x +=
            (targetAngle - rightArm.rotation.x) * delta * 1.2; // Adjust the speed of movement here
        }

        if (leftArm) {
          leftArm.rotation.x = rightArm.rotation.x; // Mirror right arm movement to left arm
        }

        requestAnimationFrame(animate);
        renderer.render(scene, camera);
      }

      animate();
    }
  );

  loader.load(
    "https://threejs.org/examples/models/gltf/Michelle.glb",
    function (gltf) {
      const model2 = gltf.scene;

      rightArm2 = model2.getObjectByName("mixamorigRightArm");
      leftArm2 = model2.getObjectByName("mixamorigLeftArm");

      model2.scale.set(12, 12, 12);
      model2.position.y = 0;

      // Set model2 and its children to be transparent
      model2.traverse(function (child) {
        if (child.isMesh) {
          child.material.transparent = true;
          child.material.opacity = 0.5;
        }
      });

      scene.add(model2);

      function animate() {
        requestAnimationFrame(animate);

        const t = clock.getElapsedTime();
        const angle = Math.sin(t) * 1;

        if (rightArm2) {
          if (angle > -0.95 && angle < 0.95) {
            rightArm2.rotation.x = angle;
            leftArm2.rotation.x = rightArm2.rotation.x;
          }
        }
        renderer.render(scene, camera);
      }

      animate();
    }
  );

  const renderer = new THREE.WebGLRenderer({ canvas: canvas });
  renderer.setPixelRatio(window.devicePixelRatio);

  // const controls = new OrbitControls(camera, renderer.domElement);
  // controls.target.set(0, 1, 0);
  // controls.update();

  // floor
  let floorGeometry = new THREE.PlaneGeometry(5000, 5000, 1, 1);
  let floorMaterial = new THREE.MeshPhongMaterial({
    color: 0xeeeeee,
    shininess: 0,
  });
  let floor = new THREE.Mesh(floorGeometry, floorMaterial);
  floor.rotation.x = -0.5 * Math.PI; // This is 90 degrees by the way
  floor.receiveShadow = true;
  floor.position.y = -11;
  scene.add(floor);

  // -----------don't delete------------------------

  // function checkAndPlayNextPart(newIsRaised) {
  //   if (newIsRaised !== previousIsRaised) {
  //     playNextPart();
  //     previousIsRaised = newIsRaised; // Update previousIsRaised to the new value
  //   }
  // }

  // if(){
  //   isRaised = true; // Toggle the state
  //   checkAndPlayNextPart(isRaised);
  //   targetAngle =0.95; // Set the target angle based on the state
  // }

  // if(){
  // isRaised = false; // Toggle the state
  // checkAndPlayNextPart(isRaised);
  // targetAngle = -0.95; // Set the target angle based on the state
  // }

  // --------------------------------------------------
}

init();
