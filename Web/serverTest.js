const express = require("express");
const http = require("http");
const WebSocket = require("ws");
const path = require("path");

const app = express();

// Serve static files from the "public" directory
app.use(express.static(path.join(__dirname, "public")));

const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

const STABILITY_THRESHOLD = 20;
const VARIATION_LIMIT = 5; // Adjust based on your needs
let bpmValues = [];

wss.on("connection", (ws) => {
  console.log("Client connected");

  ws.on("message", (message) => {
    try {
      const data = JSON.parse(message);
      console.log("Received message:", data);
      // const { bpm } = JSON.parse(message);
      // const bpmInt = Math.round(bpm); // Ensure BPM is an integer
      // bpmValues.push(bpmInt);

      if (data.type === "redirect") {
        // Broadcast the redirect message to all clients
        wss.clients.forEach((client) => {
          if (client.readyState === WebSocket.OPEN) {
            client.send(
              JSON.stringify({
                type: "redirect",
                message: data.message,
              })
            );
          }
        });
      } if (data.type == "bpm"){
        const bpmInt = Math.round(data.value); // Ensure BPM is an integer
        bpmValues.push(bpmInt);

        // Keep only the latest STABILITY_THRESHOLD values
        if (bpmValues.length > STABILITY_THRESHOLD) {
        bpmValues.shift();
        }

        // Check for stability
        let isStable = false;
        if (bpmValues.length === STABILITY_THRESHOLD) {
            const maxBPM = Math.max(...bpmValues);
            const minBPM = Math.min(...bpmValues);
            isStable = (maxBPM - minBPM) <= VARIATION_LIMIT;
        }

        console.log(`Received BPM: ${bpmInt}, Stability: ${isStable}`);
        if (isStable) {
            ws.send("stop");
            isStable = false;
        }
      }else {
        // Handle other types of messages as before
        wss.clients.forEach((client) => {
          if (client !== ws && client.readyState === WebSocket.OPEN) {
            client.send(message);
          }
        });
      }

      // // Keep only the latest STABILITY_THRESHOLD values
      // if (bpmValues.length > STABILITY_THRESHOLD) {
      //   bpmValues.shift();
      // }

      // // Check for stability
      // let isStable = false;
      // if (bpmValues.length === STABILITY_THRESHOLD) {
      //   const maxBPM = Math.max(...bpmValues);
      //   const minBPM = Math.min(...bpmValues);
      //   isStable = maxBPM - minBPM <= VARIATION_LIMIT;
      // }

      // console.log(`Received BPM: ${bpmInt}, Stability: ${isStable}`);
      // if (isStable) {
      //   ws.send("stop");
      //   isStable = false;
      // }
    } catch (error) {
      console.error("Error parsing message:", error);
    }
  });

  ws.on("close", () => {
    console.log("Client disconnected");
  });
});

server.listen(8080, () => {
  console.log("http://localhost:8080");
});
