/**
 * Einzelne LED ansteuern
 * @param {string} ledId - ID der LED im HTML (z. B. 'led1', 'led2')
 * @param {boolean} status - true = an (grün), false = aus (grau)
 */
function setLed(ledId, status) {
    const led = document.getElementById(ledId);
    if (!led) {
        console.warn(`LED mit der ID "${ledId}" wurde im HTML nicht gefunden!`);
        return;
    }

    if (status) {
        led.classList.add('active');
    } else {
        led.classList.remove('active');
    }
}


// Reagiert sofort, wenn du auf der Tastatur eine Zahl drückst
document.addEventListener('keydown', (event) => {
    // Prüft, ob eine Zahl von 0 bis 4 gedrückt wurde
    if (['0', '1', '2', '3', '4'].includes(event.key)) {
        const zahl = parseInt(event.key, 10);
        console.log(`Taste ${zahl} gedrückt! Schalte LEDs...`);
        
        // Ruft dein switch-case auf
        updateDisplay(zahl);
    }
});

/**
 * Gesamtstatus der Anzeige setzen
 * @param {number} status - Zahl von 0 bis 4
 */
function updateDisplay(status) {
    // Zahl/Input sicherstellen
    const level = Number(status);


    switch (level) {
        case 0:
            setLed('led1', false);
            setLed('led2', false);
            setLed('led3', false);
            setLed('led4', false);
            break;
        case 1:
            setLed('led1', true);   // Schaltet LED 1 AN (Grün)
            setLed('led2', false);  // Schaltet LED 2 AUS
            setLed('led3', false);  // Schaltet LED 3 AUS
            setLed('led4', false);  // Schaltet LED 4 AUS
            break;
        case 2:
            setLed('led1', true);
            setLed('led2', true);
            setLed('led3', false);
            setLed('led4', false);
            break;
        case 3:
            setLed('led1', true);
            setLed('led2', true);
            setLed('led3', true);
            setLed('led4', false);
            break;
        case 4:
            setLed('led1', true);
            setLed('led2', true);
            setLed('led3', true);
            setLed('led4', true);
            break;
        default:
            console.warn("Ungültige Stufe! Verwende eine Zahl von 0 bis 4.");
            setLed('led1', false);
            setLed('led2', false);
            setLed('led3', false);
            setLed('led4', false);
            break;
    }
}

