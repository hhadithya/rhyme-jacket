import * as THREE from "three";
import { GLTFLoader } from "jsm/loaders/GLTFLoader.js";
import { OrbitControls } from "jsm/controls/OrbitControls.js";

const canvas = document.getElementById("level1_model_canvas");
let rightArm, leftArm, model;
let audioBuffer, audioContext, audioSource;
let partDuration;
let currentPart = 0;
const music1_url = "../music/Twinkle Twinkle-1.mp3"; // Adjust the path as necessary
let isRaised = true;

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
        partDuration = audioBuffer.duration / 16;
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
  scene.background = new THREE.Color(0xace1af);
  const clock = new THREE.Clock();

  const w = window.innerWidth;
  const h = window.innerHeight;
  const camera = new THREE.PerspectiveCamera(75, w / h, 0.1, 1000);
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

      model.scale.set(12, 12, 12);
      model.position.y = 3;

      scene.add(model);

      function animate() {
        requestAnimationFrame(animate);

        const t = clock.getElapsedTime();
        const angle = Math.sin(1.5 * t) * 1;
        console.log(angle);

        if (rightArm) {
          if (angle > -0.95 && angle < 0.95) {
            rightArm.rotation.x = angle;
            leftArm.rotation.x = rightArm.rotation.x;
          }
        }

        renderer.render(scene, camera);
      }

      animate();
    }
  );

  const renderer = new THREE.WebGLRenderer({ canvas: canvas });
  renderer.setPixelRatio(window.devicePixelRatio);

  let floorGeometry = new THREE.PlaneGeometry(5000, 5000, 1, 1);
  let floorMaterial = new THREE.MeshPhongMaterial({
    color: 0xbbbbbb,
    shininess: 0,
  });
  let floor = new THREE.Mesh(floorGeometry, floorMaterial);
  floor.rotation.x = -0.5 * Math.PI;
  floor.receiveShadow = true;
  floor.position.y = -11;
  scene.add(floor);

  setInterval(playNextPart, 2250);
}

init();
