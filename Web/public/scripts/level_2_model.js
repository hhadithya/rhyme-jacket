import * as THREE from "three";
import { GLTFLoader } from "jsm/loaders/GLTFLoader.js";
import { OrbitControls } from "jsm/controls/OrbitControls.js";

document.addEventListener("DOMContentLoaded", function () {
  const canvas = document.getElementById("three-canvas");
  let spine_1;
  let audioBuffer, audioContext, audioSource;
  let partDuration;
  let currentPart = 0;
  const music2Url = "../music/level_2_music.mp3"; // Replace with the actual path to your audio file

  async function loadAudio() {
    try {
      const response = await fetch(music2Url);
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const arrayBuffer = await response.arrayBuffer();
      audioContext = new (window.AudioContext || window.webkitAudioContext)();
      audioContext.decodeAudioData(
        arrayBuffer,
        (buffer) => {
          audioBuffer = buffer;
          partDuration = audioBuffer.duration / 20;
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

    currentPart = (currentPart + 1) % 20;
  }

  loadAudio();

  const scene = new THREE.Scene();
  scene.background = new THREE.Color(0xace1af);

  const clock = new THREE.Clock();
  const camera = new THREE.PerspectiveCamera(
    75,
    window.innerWidth / window.innerHeight,
    0.1,
    1000
  );
  camera.position.z = 15;
  camera.position.x = 0;
  camera.position.y = 6;

  const light = new THREE.HemisphereLight(0xffffff, 0xffffff, 6);
  light.position.set(0, 1, 0);
  scene.add(light);

  const loader = new GLTFLoader();
  loader.load(
    "https://threejs.org/examples/models/gltf/Michelle.glb",
    function (gltf) {
      const model1 = gltf.scene;
      spine_1 = model1.getObjectByName("mixamorigSpine");

      model1.scale.set(11, 11, 11);
      model1.position.y = -8;

      scene.add(model1);

      function animate() {
        requestAnimationFrame(animate);

        const t = clock.getElapsedTime();
        const angle = Math.sin(1.4 * t) * 0.72;

        if (angle < 0.7 && angle > -0.7) {
          spine_1.rotation.z = angle;
        }

        renderer.render(scene, camera);
      }

      animate();
    }
  );

  const renderer = new THREE.WebGLRenderer({ canvas: canvas });
  renderer.setPixelRatio(window.devicePixelRatio);

  const controls = new OrbitControls(camera, renderer.domElement);
  controls.target.set(0, 1, 0);
  controls.update();

  const floorGeometry = new THREE.PlaneGeometry(5000, 5000, 1, 1);
  const floorMaterial = new THREE.MeshPhongMaterial({
    color: 0xaaaaaa,
    shininess: 0,
  });
  const floor = new THREE.Mesh(floorGeometry, floorMaterial);
  floor.rotation.x = -0.5 * Math.PI;
  floor.receiveShadow = true;
  floor.position.y = -11;
  scene.add(floor);

  setInterval(playNextPart, 2350);
});
