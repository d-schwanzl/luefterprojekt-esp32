const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const { WebSocketServer } = require('ws');

// 1. Serielle Verbindung öffnen (COM-Port an deine Umgebung anpassen!)
const port = new SerialPort({
  path: 'COM3', // Z. B. 'COM3' unter Windows oder '/dev/ttyUSB0' unter Linux
  baudRate: 115200
});

const parser = port.pipe(new ReadlineParser({ delimiter: '\n' }));

// 2. WebSocket-Server auf Port 8080 starten
const wss = new WebSocketServer({ port: 8080 });

console.log('WebSocket Server läuft auf ws://localhost:8080');

// 3. Daten vom ESP32 empfangen und per WebSocket an das Webinterface senden
parser.on('data', (line) => {
  try {
    const data = JSON.parse(line.trim());
    
    // Wir filtern gezielt nach dem Typ "pwmStufe"
    if (data.type === 'pwmStufe') {
      console.log(`Empfangene pwmStufe vom ESP32: ${data.value}`);
      
      // An alle verbundenen Browser-Clients weiterleiten
      wss.clients.forEach((client) => {
        if (client.readyState === 1) { // 1 = OPEN
          client.send(JSON.stringify(data));
        }
      });
    }
  } catch (err) {
    // Ignoriere Zeilen, die kein gültiges JSON sind
  }
});