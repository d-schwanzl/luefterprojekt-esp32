//Komplette bidirektionale Verbindung, quasi der Postbote zwischen Frontend: script.js und GatewaySerial.pp des ESPs

const express = require('express');
const path = require('path');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const { WebSocketServer, WebSocket } = require('ws');

const app = express();

// 1. Webserver-Ordner "public" freigeben (hier liegt deine index.html & script.js)
app.use(express.static(path.join(__dirname, 'public')));

// 2. HTTP-Webserver auf Port 3000 starten
app.listen(3000, () => {
    console.log('Webinterface erreichbar unter http://localhost:3000');
});

// 3. Serielle Verbindung zum ESP32-H2 initialisieren
const port = new SerialPort({
  path: 'COM3', // Passe deinen COM-Port an, falls nötig
  baudRate: 115200
});

port.on('error', (err) => {
  console.error('Serieller Schnittstellenfehler:', err.message);
});

// 4. Parser an den Port hängen (Liest Zeile für Zeile bei \n)
const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

// 5. WebSocket-Server auf Port 8080 starten
const wss = new WebSocketServer({ port: 8080 });
console.log('WebSocket Server läuft auf ws://localhost:8080');

parser.on('data', (line) => {

  const cleanLine = line.trim();

  console.log("VOM ESP32:", cleanLine);

  if (!cleanLine) return;

  try {
    const data = JSON.parse(cleanLine);

    console.log("JSON ERKANNT:", data);

    wss.clients.forEach((client) => {

      console.log("CLIENT STATUS:", client.readyState);

      if (client.readyState === WebSocket.OPEN) {
        console.log("SENDE AN BROWSER:", JSON.stringify(data));
        client.send(JSON.stringify(data));
      }

    });

  } catch (err) {
    console.log("JSON FEHLER:", cleanLine);
    console.log(err.message);
  }
});

// 7. Befehle vom Browser empfangen und an den ESP32 weiterreichen
wss.on('connection', (ws) => {
  console.log('Browser verbunden.');

  ws.on('message', (message) => {
    const cleanMessage = message.toString().trim();
    console.log('Vom Browser empfangen:', cleanMessage);

    // Direkt an den ESP32 über die serielle Schnittstelle weiterreichen
    port.write(cleanMessage + '\n');
  });

  ws.on('close', () => {
    console.log('Browser getrennt.');
  });
});

























