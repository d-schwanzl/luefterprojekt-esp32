// Wartet, bis das gesamte HTML geladen ist
document.addEventListener("DOMContentLoaded", () => {
    
    // 1. Wir suchen das Matrix-Display im HTML
    const display = document.querySelector(".matrix-display");
    
    // Startwert für den Zähler (wir fangen bei 0 an)
    let aktuelleZahl = 0;

    // 2. Diese Funktion wechselt die Zahl auf dem Display
    function zeigeNaechsteZahl() {
        // Wir entfernen zuerst alle alten Zahlen-Klassen (n0 bis n9)
        for (let i = 0; i <= 9; i++) {
            display.classList.remove(`n${i}`);
        }

        // Wir fügen die neue Klasse hinzu (z.B. "n0", "n1" etc.)
        display.classList.add(`n${aktuelleZahl}`);

        // Zähler um 1 erhöhen für den nächsten Schritt
        aktuelleZahl++;

        // Wenn wir über die 9 kommen (also bei 10 sind), fangen wir wieder bei 0 an
        if (aktuelleZahl > 9) {
            aktuelleZahl = 0;
        }
    }

    // 3. Den Taktgeber (Timer) starten
    // 1000 Millisekunden = genau 1 Sekunde
    setInterval(zeigeNaechsteZahl, 1000);

    // Einmal direkt beim Start ausführen, damit nicht 1 Sekunde gewartet werden muss
    zeigeNaechsteZahl();
});
