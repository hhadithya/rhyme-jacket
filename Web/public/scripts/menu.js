// Get the modal
var modal = document.getElementById("myModal");
var btn = document.getElementById("exit-btn");
var span = document.getElementsByClassName("close")[0];
var noBtn = document.getElementById("no");

// Get the sound on/off buttons
const soundOn = document.getElementById("sound-on");
const soundOff = document.getElementById("sound-off");

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

// Sound on/off
soundOn.addEventListener("click", () => {
  soundOn.classList.add("hidden");
  soundOff.classList.remove("hidden");
});

soundOff.addEventListener("click", () => {
  soundOn.classList.remove("hidden");
  soundOff.classList.add("hidden");
});
