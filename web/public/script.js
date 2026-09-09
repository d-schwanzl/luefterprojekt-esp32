document.addEventListener('DOMContentLoaded', () => {
  const inputFeld = document.getElementById('zifferInput');
  const anzeigenBtn = document.getElementById('btnAnzeigen');
  const matrixDisplay = document.querySelector('.matrix-display');

  function zeigeZiffer() {
    const wert = parseInt(inputFeld.value, 10);

    // Prüfen, ob eine gültige Zahl zwischen 0 und 9 eingegeben wurde
    if (!isNaN(wert) && wert >= 0 && wert <= 9) {
      // Entfernt alle alten Ziffern-Klassen (n0 bis n9)
      for (let i = 0; i <= 9; i++) {
        matrixDisplay.classList.remove(`n${i}`);
      }

      // Fügt die neue Klasse hinzu (z. B. "n3")
      matrixDisplay.classList.add(`n${wert}`);
    }
  }

  // Ausführen bei Klick auf den Button "Anzeigen"
  if (anzeigenBtn) {
    anzeigenBtn.addEventListener('click', zeigeZiffer);
  }

  // Ausführen direkt beim Ändern des Werts im Eingabefeld
  if (inputFeld) {
    inputFeld.addEventListener('input', zeigeZiffer);
  }
});