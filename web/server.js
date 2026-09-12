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

// 6. Empfangene Daten vom ESP32 verarbeiten und an Browser schicken
parser.on('data', (line) => {
  const cleanLine = line.trim();
  if (!cleanLine) return;

  try {
    const data = JSON.parse(cleanLine);
    
    if (data.type === 'balkenWert') {
      console.log(`Empfangener balkenWert vom ESP32: ${data.value}`);

      // Per WebSocket live an alle verbundenen Browser-Clients senden
      wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
          client.send(JSON.stringify(data));
        }
      });
    }
  } catch (err) {
    // Ungültiges JSON ignorieren
  }
});