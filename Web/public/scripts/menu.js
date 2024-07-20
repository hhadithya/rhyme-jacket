// Get the modal
var modal = document.getElementById("myModal");
var btn = document.getElementById("exit-btn");
var span = document.getElementsByClassName("close")[0];
var noBtn = document.getElementById("no");

// Get the sound on/off buttons
const soundOn = document.getElementById("sound-on");
const soundOff = document.getElementById("sound-off");
const audio = document.getElementById("background-music");
const batteryPercentageElement = document.getElementById("battery-percentage");

var connection = new WebSocket("ws://" + location.hostname + ":8080/");

// get click sound to a constant
const clickSound = new Audio("../music/click.mp3");

// When the user clicks the button, open the modal
btn.onclick = function () {
  modal.style.display = "block";
};

// When the user clicks on <span> (x), close the modal
span.onclick = function () {
  modal.style.display = "none";
};

noBtn.onclick = function () {
  modal.style.display = "none";
};

// When the user clicks anywhere outside of the modal, close it
window.onclick = function (event) {
  if (event.target == modal) {
    modal.style.display = "none";
  }
};

// play music when page is loaded
audio.play();

// play music when sound is on
soundOn.addEventListener("click", () => {
  soundOn.classList.add("hidden");
  soundOff.classList.remove("hidden");
  audio.pause();
});

// stop music when sound is off
soundOff.addEventListener("click", () => {
  soundOff.classList.add("hidden");
  soundOn.classList.remove("hidden");
  audio.play();
});

// play a sound when #new-game, #levels and #exit buttons are clicked
const newGame = document.getElementById("new-game");
const levels = document.getElementById("levels");
const exit = document.getElementById("exit-btn");

// mouseover sound effect
newGame.addEventListener("mouseover", () => {
  clickSound.play();
});

levels.addEventListener("mouseover", () => {
  clickSound.play();
});

exit.addEventListener("mouseover", () => {
  clickSound.play();
});

connection.onmessage = function (event) {
  event.data.text().then((text) => {
    var data = JSON.parse(text);
    console.log(data);

    // Update the battery percentage element
    if (data.batteryPercentage !== undefined) {
      // round the battery percentage to 0 decimal places
      batteryPercentageElement.textContent =
        data.batteryPercentage.toFixed(0) + "%";
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
  } else {
    batteryImg[0].classList.add("hidden");
    batteryImg[1].classList.add("hidden");
    batteryImg[2].classList.add("hidden");
    batteryImg[3].classList.add("hidden");
    batteryImg[4].classList.add("hidden");
    batteryImg[5].classList.remove("hidden");
  }
}, 500);
