// Gateway zwischen Browser (script.js) und ESP32 (GatewaySerial.cpp)

const express = require("express");
const path = require("path");
const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");
const { WebSocketServer, WebSocket } = require("ws");

const app = express();
app.use(express.static(path.join(__dirname, "public")));
app.listen(3000, () => {
  console.log("Webinterface erreichbar unter http://localhost:3000");
});

// WebSocket-Server zuerst anlegen
const wss = new WebSocketServer({ port: 8080 });
console.log("WebSocket Server läuft auf ws://localhost:8080");

let port = null;

function verarbeiteZeile(line) {
  const cleanLine = line.trim();
  if (!cleanLine) return;
  try {
    const data = JSON.parse(cleanLine);
    wss.clients.forEach((client) => {
      if (client.readyState === WebSocket.OPEN) {
        client.send(JSON.stringify(data));
      }
    });
  } catch (err) {
    // Debug-Zeile vom ESP, kein JSON: ignorieren
  }
}

async function verbinde() {
  const ports = await SerialPort.list();
  console.log(
    "Gefundene Ports:",
    ports.map((p) => `${p.path} (VID ${p.vendorId}, PID ${p.productId})`).join(", ") || "keine"
  );

  // Optional fest vorgeben: $env:COM_PORT="COM6"; node web/server.js
  const gefunden = ports.find((p) => p.vendorId && p.vendorId.toUpperCase() === "303A");
  const pfad = process.env.COM_PORT || (gefunden && gefunden.path);

  if (!pfad) {
    console.log("Kein ESP gefunden, neuer Versuch in 3 s");
    setTimeout(verbinde, 3000);
    return;
  }

  port = new SerialPort({ path: pfad, baudRate: 115200 });
  port.on("open", () => console.log("Serielle Verbindung offen:", pfad));
  port.on("error", (err) => console.error("Serieller Schnittstellenfehler:", err.message));
  port.on("close", () => {
    console.log("Port geschlossen, neuer Versuch in 3 s");
    setTimeout(verbinde, 3000);
  });

  const parser = port.pipe(new ReadlineParser({ delimiter: "\n" }));
  parser.on("data", verarbeiteZeile);
}

// Befehle vom Browser an den ESP32 weiterreichen
wss.on("connection", (ws) => {
  console.log("Browser verbunden.");

  ws.on("message", (message) => {
    const cleanMessage = message.toString().trim();
    console.log("Vom Browser empfangen:", cleanMessage);
    if (port && port.isOpen) {
      port.write(cleanMessage + "\n");
    } else {
      console.log("Port nicht offen, Befehl verworfen");
    }
  });

  ws.on("close", () => console.log("Browser getrennt."));
});

verbinde();