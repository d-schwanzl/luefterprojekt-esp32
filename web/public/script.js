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