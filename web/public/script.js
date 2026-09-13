// WebSocket-Verbindung zum Node.js Gateway aufbauen
const socket = new WebSocket('ws://localhost:8080');

socket.onopen = () => {
  console.log('Erfolgreich mit dem Server verbunden.');
};



socket.onmessage = (event) => {
  try {
    const data = JSON.parse(event.data);

    if (data.type === 'balkenWert') {
      updateLeds(data.value);
    }
    if (data.type === 'SegmentWert') {
      zeigeZifferAufMatrix(data.value);   // <- hier rufst du deine neue Funktion auf
    }
    if (data.type === 'currentRpm') {
      zeigeRpmAufTacho(data.value);   // <- hier rufst du deine neue Funktion auf
    }

  } catch (e) {
    console.error('Fehler beim Verarbeiten der Nachricht:', e);
  }
};

/**
 * Aktualisiert die LEDs 1 bis 4 basierend auf dem Balkenwert (0 bis 4)
 * @param {number} level - Wert von 0 bis 4
 */
function updateLeds(level) {
  console.log(`Aktualisiere Lüfterstufen-LEDs auf Balkenwert: ${level}`);

  for (let i = 1; i <= 4; i++) {
    const ledElement = document.getElementById(`led${i}`); 
    if (ledElement) {
      if (i <= level) {
        ledElement.classList.add('active');   // Schaltet LED 1 bis 'level' ein
      } else {
        ledElement.classList.remove('active'); // Schaltet restliche LEDs aus
      }
    }
  }
}


/* Aktualisiert die Matrixanzeige, die die 7‑Segmentanzeige am Anzeigeteil betrifft.*/

function zeigeZifferAufMatrix(number) {     //Number muss nicht definiert werden; es ist ein Funktionsparameter und allein bereits die Definition in JavaScript, Nicht so in C oder C++.
  const element = document.querySelector('.matrix-display');

  for (let i = 0; i <= 9; i++) {
    element.classList.remove(`n${i}`);
  }

  element.classList.add(`n${number}`);
}


// Ändert die Zahl auf der Tachoanzeige.
function  zeigeRpmAufTacho(RpmNumber) {

  const element = document.getElementById('rpm-value');
  if (element) {
    element.textContent = RpmNumber;
  }

}



