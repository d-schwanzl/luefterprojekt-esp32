// WebSocket-Verbindung zum Node.js Gateway aufbauen
const socket = new WebSocket('ws://localhost:8080');

socket.onopen = () => {
  console.log('Erfolgreich mit dem Server verbunden.');
};

// Zentrales Objekt, das die Zeitstempel für jede Variable speichert
const letzterUpdateZeiten = {};
const drosselIntervallMs = 250; // Einheits-Intervall für alle (250 ms)

// Universelle Drosselungsfunktion
function aktualisiereGedrosselt(schluessel, wert, ausfuehrungsFunktion) {
  const jetzt = Date.now();
  const letzterZeitpunkt = letzterUpdateZeiten[schluessel] || 0;

  // Prüfen, ob das Intervall für diesen spezifischen Schlüssel rum ist
  if (jetzt - letzterZeitpunkt < drosselIntervallMs) {
    return; // Zu früh, verwerfen
  }

  // Zeitstempel aktualisieren
  letzterUpdateZeiten[schluessel] = jetzt;

  // Die übergebene UI-Logik ausführen
  ausfuehrungsFunktion(wert);
}

socket.onmessage = (event) => {
  try {
    const data = JSON.parse(event.data);


        console.log("NACHRICHT ANGEKOMMEN:", data);
    console.log("TYPE:", data.type);
    console.log("WERT:", data.value);

    // 1. Balkenwert gedrosselt verarbeiten
    if (data.type === 'balkenWert') {
            console.log(">>> BALKENWERT ERKANNT:", data.value);

      aktualisiereGedrosselt('balken', data.value, (wert) => {
        updateLeds(wert);
      });
    }

    // 2. Segmentwert gedrosselt verarbeiten
    if (data.type === 'SegmentWert') {
       console.log(">>> SEGMENTWERT ERKANNT:", data.value);
      aktualisiereGedrosselt('segment', data.value, (wert) => {
        zeigeZifferAufMatrix(wert);
      });
    }

    // 3. RPM-Wert gedrosselt verarbeiten
    if (data.type === 'currentRpm') {
      aktualisiereGedrosselt('rpm', data.value, (wert) => {
        const element = document.getElementById('rpm-value');
        if (element) {
          element.textContent = wert;
        }
      });
    }

        // Lüfter-Statusanzeige LED AUS
if (data.type === 'LedLuefterAus') {
  const element = document.getElementById('symbol-led1');
  if (element) {
    if (data.value == 0) {
      element.classList.remove('active');
    } else {
      element.classList.add('active');
    }
  }
}

        // Lüfter-Statusanzeige LED EIN
if (data.type === 'LedLuefterAn') {
  const element = document.getElementById('symbol-led2');
  if (element) {
    if (data.value == 0) {
      element.classList.remove('active');
    } else {
      element.classList.add('active');
    }
  }
}

         // Lüfter-Statusanzeige LED STOERUNG
if (data.type === 'LedLuefterStoerung') {
  const element = document.getElementById('symbol-led3');
  if (element) {
    if (data.value == 0) {
      element.classList.remove('active');
    } else {
      element.classList.add('active');
    }
  }
}


  } catch (e) {
    console.error('Fehler beim Verarbeiten der Nachricht:', e);
  }
};

/**
 * Aktualisiert die LEDs 1 bis 4 basierend auf dem Balkenwert (0 bis 4)
 */
function updateLeds(level) {
  console.log(`Aktualisiere Lüfterstufen-LEDs auf Balkenwert: ${level}`);

  for (let i = 1; i <= 4; i++) {
    const ledElement = document.getElementById(`led${i}`); 
    if (ledElement) {
      if (i <= level) {
        ledElement.classList.add('active');
      } else {
        ledElement.classList.remove('active');
      }
    }
  }
}

/**
 * Aktualisiert die Matrixanzeige
 */
function zeigeZifferAufMatrix(number) {
  const element = document.querySelector('.matrix-display');
  if (!element) return;

  for (let i = 0; i <= 9; i++) {
    element.classList.remove(`n${i}`);
  }

  element.classList.add(`n${number}`);
}