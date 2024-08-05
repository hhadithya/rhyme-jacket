import * as THREE from "three";
import { GLTFLoader } from "jsm/loaders/GLTFLoader.js";
import { OrbitControls } from "jsm/controls/OrbitControls.js";

document.addEventListener("DOMContentLoaded", function () {
  const canvas = document.getElementById("three-canvas");
  const actionButton = document.getElementById("level_2_action");
  let model, spine, model1, spine_1;
  let targetAngle = 0;
  let isTilted = true;
  let previousIsTilted = isTilted;
  let audioBuffer, audioContext, audioSource;
  let partDuration;
  let currentPart = 0;
  const music2Url = "../music/sunshine.mp3"; // Replace with the actual path to your audio file

  // sensor data
  let left_gyro_y = 0;
  let left_gyro_p = 0;
  let left_gyro_r = 0;
  let right_gyro_y = 0;
  let right_gyro_p = 0;
  let right_gyro_r = 0;
  let middle_gyro_y = 0;
  let middle_gyro_p = 0;
  let middle_gyro_r = 0;
  let right_flex = 0;
  let left_flex = 0;
  let batteryPercentageElement = document.getElementById("battery-percentage");

  connection.onmessage = function (event) {
    event.data.text().then((text) => {
      const data = JSON.parse(text);
      console.log(data);

      // Update the battery percentage element
      if (data.batteryPercentage !== undefined) {
        // round the battery percentage to 0 decimal places
        batteryPercentageElement.textContent =
          data.batteryPercentage.toFixed(0) + "%";
      }

      if (data.flex1Angle !== undefined) {
        left_flex = data.flex1Angle;
      }
      if (data.flex2Angle !== undefined) {
        right_flex = data.flex2Angle;
      }

      if (data.yawRight !== undefined) {
        right_gyro_y = data.yawRight;
      }
      if (data.pitchRight !== undefined) {
        right_gyro_p = data.pitchRight;
      }
      if (data.rollRight !== undefined) {
        right_gyro_r = data.rollRight;
      }

      if (data.yawLeft !== undefined) {
        left_gyro_y = data.yawLeft;
      }
      if (data.pitchLeft !== undefined) {
        left_gyro_p = data.pitchLeft;
      }
      if (data.rollLeft !== undefined) {
        left_gyro_r = data.rollLeft;
      }

      if (data.yawCenter !== undefined) {
        middle_gyro_y = data.yawCenter;
      }
      if (data.pitchCenter !== undefined) {
        middle_gyro_p = data.pitchCenter;
      }
      if (data.rollCenter !== undefined) {
        middle_gyro_r = data.rollCenter;
      }
    });
  };

  // change battery image based on battery percentage
  setInterval(() => {
    var batteryPercentage = parseFloat(batteryPercentageElement.textContent);
    var batteryImg = document.getElementById("battery-img").children;

    if (batteryPercentage <= 20) {
      batteryImg[0].classList.add("hidden");
      batteryImg[1].classList.remove("hidden");
      batteryImg[2].classList.add("hidden");
      batteryImg[3].classList.add("hidden");
      batteryImg[4].classList.add("hidden");
      batteryImg[5].classList.add("hidden");
    } else if (batteryPercentage <= 40) {
      batteryImg[0].classList.add("hidden");
      batteryImg[1].classList.add("hidden");
      batteryImg[2].classList.remove("hidden");
      batteryImg[3].classList.add("hidden");
      batteryImg[4].classList.add("hidden");
      batteryImg[5].classList.add("hidden");
    } else if (batteryPercentage <= 60) {
      batteryImg[0].classList.add("hidden");
      batteryImg[1].classList.add("hidden");
      batteryImg[2].classList.add("hidden");
      batteryImg[3].classList.remove("hidden");
      batteryImg[4].classList.add("hidden");
      batteryImg[5].classList.add("hidden");
    } else if (batteryPercentage <= 80) {
      batteryImg[0].classList.add("hidden");
      batteryImg[1].classList.add("hidden");
      batteryImg[2].classList.add("hidden");
      batteryImg[3].classList.add("hidden");
      batteryImg[4].classList.remove("hidden");
      batteryImg[5].classList.add("hidden");
    } else if (batteryPercentage <= 100) {
      batteryImg[0].classList.add("hidden");
      batteryImg[1].classList.add("hidden");
      batteryImg[2].classList.add("hidden");
      batteryImg[3].classList.add("hidden");
      batteryImg[4].classList.add("hidden");
      batteryImg[5].classList.remove("hidden");
    } else {
      batteryImg[0].classList.remove("hidden");
      batteryImg[1].classList.add("hidden");
      batteryImg[2].classList.add("hidden");
      batteryImg[3].classList.add("hidden");
      batteryImg[4].classList.add("hidden");
      batteryImg[5].classList.add("hidden");
    }
  }, 500);

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

    currentPart = (currentPart + 1) % 16;
  }

  loadAudio();

  const scene = new THREE.Scene();
  scene.background = new THREE.Color(0xace1af);
  const clock = new THREE.Clock();

  const w = window.innerWidth;
  const h = window.innerHeight;
  const camera = new THREE.PerspectiveCamera(75, w / h, 0.1, 1000);
  camera.position.z = 18;
  camera.position.x = 0;
  camera.position.y = 11;

  const light = new THREE.HemisphereLight(0xffffff, 0xffffff, 6);
  light.position.set(0, 1, 0);
  scene.add(light);

  const loader = new GLTFLoader();
  loader.load(
    "https://threejs.org/examples/models/gltf/Michelle.glb",
    function (gltf) {
      model = gltf.scene;
      spine = model.getObjectByName("mixamorigSpine");

      model.scale.set(12, 12, 12);
      model.position.y = 0;
      scene.add(model);

      function animate() {
        const delta = clock.getDelta();
        if (spine) {
          spine.rotation.z += (targetAngle - spine.rotation.z) * delta * 1.6;
        }

        // -----------------------------------------------------------
        // if (
        //   left_flex > 0 &&
        //   left_flex <= 30 &&
        //   right_flex > 0 &&
        //   right_flex <= 30
        // ) {
        //   document.getElementById("correctMove").style.display = "block";
        //   document.getElementById("wrongMove").style.display = "none";
        // }

        // if (
        //   left_flex > 30 &&
        //   left_flex <= 100 &&
        //   right_flex > 30 &&
        //   right_flex <= 100
        // ) {
        //   document.getElementById("correctMove").style.display = "none";
        //   document.getElementById("wrongMove").style.display = "block";
        // }
        // -----------------------------------------------------------

        requestAnimationFrame(animate);
        renderer.render(scene, camera);
      }

      animate();
    }
  );

  loader.load(
    "https://threejs.org/examples/models/gltf/Michelle.glb",
    function (gltf) {
      model1 = gltf.scene;
      spine_1 = model1.getObjectByName("mixamorigSpine");

      model1.scale.set(12, 12, 12);
      model1.position.y = 0;

      // Set model2 and its children to be transparent
      model1.traverse(function (child) {
        if (child.isMesh) {
          child.material.transparent = true;
          child.material.opacity = 0.5;
        }
      });

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

  // const controls = new OrbitControls(camera, renderer.domElement);
  // controls.target.set(0, 1, 0);
  // controls.update();

  const floorGeometry = new THREE.PlaneGeometry(5000, 5000, 1, 1);
  const floorMaterial = new THREE.MeshPhongMaterial({
    color: 0xaaaaaa,
    shininess: 0,
  });
  const floor = new THREE.Mesh(floorGeometry, floorMaterial);
  floor.rotation.x = -0.55 * Math.PI;
  floor.receiveShadow = true;
  floor.position.y = -11;
  scene.add(floor);

  // actionButton.addEventListener("click", function () {
  //   playNextPart();
  //   isTilted = !isTilted;
  //   targetAngle = isTilted ? 0.6 : -0.6;
  // });

  function checkAndPlayNextPart(newIsTilted) {
    if (newIsTilted !== previousIsTilted) {
      console.log("state changed");
      playNextPart();
      previousIsTilted = newIsTilted;
    }
  }

  setInterval(() => {
    // if (
    //   // right_gyro_y > 60 &&
    //   // right_gyro_y < 120 &&
    //   // left_gyro_y > 60 &&
    //   // left_gyro_y < 120
    //   right_flex < 40 &&
    //   left_flex < 40
    // ) {
    if (middle_gyro_y > 40) {
      isTilted = true;
      checkAndPlayNextPart(isTilted);
      targetAngle = -0.7;
    } else if (middle_gyro_y > 22) {
      targetAngle = -0.46;
    } else if (middle_gyro_y > 5) {
      targetAngle = -0.23;
    } else if (middle_gyro_y >= -5 && middle_gyro_y <= 5) {
      targetAngle = 0;
    } else if (middle_gyro_y > -22) {
      targetAngle = 0.23;
    } else if (middle_gyro_y > -40) {
      targetAngle = 0.46;
    } else if (middle_gyro_y <= -40) {
      isTilted = false;
      checkAndPlayNextPart(isTilted);
      targetAngle = 0.7;
    }
    // }
  }, 50);

  setInterval(() => {
    if (
      left_flex >= 0 &&
      left_flex <= 50 &&
      right_flex >= 0 &&
      right_flex <= 50
      // middle_gyro_y > -30 &&
      // middle_gyro_y < 30 &&
      // middle_gyro_p > -30 &&
      // middle_gyro_p < 30 &&
      // right_gyro_y > -30 &&
      // right_gyro_y < 30 &&
      // left_gyro_y > -30 &&
      // left_gyro_y < 30
    ) {
      document.getElementById("correctMove").style.display = "block";
      document.getElementById("wrongMove").style.display = "none";
    } else {
      document.getElementById("correctMove").style.display = "none";
      document.getElementById("wrongMove").style.display = "block";
    }
  }, 100);
});
