const canvas = document.getElementById("fireworksCanvas");
const ctx = canvas.getContext("2d");
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;

class Firework {
  constructor(x, y) {
    this.x = x;
    this.y = y;
    this.length = Math.random() * 30 + 100; // Length of lightning bolt
    this.color = "white"; // Color of lightning bolt
    this.alpha = Math.random() * 0.1 + 0.3; // Random alpha value
    this.decay = Math.random() * 0.02 + 0.01;
  }

  update() {
    this.alpha -= this.decay;
  }

  draw() {
    ctx.save();
    ctx.globalAlpha = this.alpha;
    ctx.beginPath();
    ctx.moveTo(this.x, this.y);
    for (let i = 0; i < this.length; i++) {
      const angle = Math.random() * Math.PI * 2;
      const distance = Math.random() * 10;
      const endX = this.x + Math.cos(angle) * distance;
      const endY = this.y + Math.sin(angle) * distance;
      ctx.lineTo(endX, endY);
    }
    ctx.strokeStyle = this.color;
    ctx.stroke();
    ctx.restore();
  }
}

let fireworks = [];

function addFirework() {
  const numFireworks = 30; // Number of fireworks to add per frame
  for (let i = 0; i < numFireworks; i++) {
    const x = Math.random() * canvas.width;
    const y = Math.random() * canvas.height;
    fireworks.push(new Firework(x, y));
  }
}

function updateFireworks() {
  for (let i = fireworks.length - 1; i >= 0; i--) {
    fireworks[i].update();
    if (fireworks[i].alpha <= 0) {
      fireworks.splice(i, 1);
    }
  }
}

function drawFireworks() {
  for (let firework of fireworks) {
    firework.draw();
  }
}

function loop() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  addFirework();
  updateFireworks();
  drawFireworks();
  requestAnimationFrame(loop);
}

loop();

window.addEventListener("resize", () => {
  canvas.width = window.innerWidth;
  canvas.height = window.innerHeight;
});
