# Conways Spiel des Lebens

**Sprache:** [English](README.md) | [Deutsch](#)

---

Conways zellulärer Automat: Zellen entstehen bei 3 Nachbarn, überleben bei 2–3, sonst sterben sie. Das Feld ist toroidal (Ränder verbinden sich). Das Spiel entwickelt Muster von selbst; du setzt die Startzellen und startest oder führst Schritte aus.

**Cross-Plattform:**

- **Web:** HTML/CSS/JS – läuft in jedem Browser (inkl. GitHub Pages).
- **Desktop:** C++ mit raylib + CMake – eine Codebasis für **Windows, macOS und Linux** (inkl. WSL2).

- **Web-App:** https://benjarogit.github.io/the-old-game/ (nach dem ersten Release)
- **Downloads (Desktop):** https://github.com/benjarogit/the-old-game/releases

---

## Lokal starten

### Web (Browser)

`index.html` im Browser öffnen – kein Server nötig.

### Desktop (Windows, macOS, Linux)

**Abhängigkeiten:** CMake, C++17-Compiler, und für raylib (wird per FetchContent geladen):

- **Linux (Ubuntu/Debian):**  
  `sudo apt install build-essential cmake libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev`
- **macOS:** Xcode Command Line Tools (oder Xcode).
- **Windows:** Visual Studio 2022 (oder Build Tools) mit C++-Workload.

CMake lokal installieren, dann:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/spiel-des-lebens          # Linux/macOS
# Windows: build\Release\spiel-des-lebens.exe
```

Unter **WSL2** den Linux-Build wie oben bauen und ausführen.

---

## Bedienung

### Web

- **Sprache:** Deutsch (DE) und Englisch (EN). Die App nutzt die Browsersprache; du kannst jederzeit mit **DE | EN** in der Kopfzeile wechseln, falls die Erkennung nicht stimmt. Die Wahl wird lokal gespeichert.  
- **Maus/Touch:** Klicken oder Ziehen auf dem Feld – Zellen setzen oder löschen.  
- **Buttons:** Start, Schritt, Löschen. **Slider:** Geschwindigkeit.  
- **Leertaste:** Ein Schritt (eine Generation), wenn die Simulation **pausiert** ist – wie der Schritt-Button.  
- **Tasten 1–7:** Presets (wie Desktop: 1=Block, 2=Blinker, 3=Beacon, 4=Toad, 5=Pulsar, 6=Glider, 7=LWSS).  
- **Presets:** Still Lives (z. B. Block), Oscillators (Blinker, Beacon, Toad, Pulsar), Spaceships (Glider, LWSS), Zufall.

### Desktop (C++)

- **Sprache:** Deutsch und Englisch. Beim Start wird die **Betriebssystem-Sprache** genutzt (Windows: Anzeigesprache; Linux/macOS: `LANG` / `LC_ALL`). Mit **L** jederzeit wechseln, falls die Erkennung nicht stimmt.  
- **Maus:** Klicken/Ziehen auf dem Feld – Zellen setzen oder löschen.  
- **R:** Start / Pause. **Leertaste:** Ein Schritt (eine Generation), wenn **pausiert**. **C:** Löschen.  
- **1–7:** Presets (1=Block, 2=Blinker, 3=Beacon, 4=Toad, 5=Pulsar, 6=Glider, 7=LWSS). **L:** Sprache umschalten.  
- Sidebar: aktuelle Generation.

### Presets und Verhalten

- **Still Lives** (z. B. Block) verändern sich nicht – so gewollt.  
- **Oscillators** (Blinker, Beacon, Toad, Pulsar) wiederholen einen Zyklus. **Spaceships** (Glider, LWSS) bewegen sich über das Feld.  
- **Zufall** füllt das Feld zufällig; manche Muster stabilisieren sich, andere bewegen sich – beides ist normal.

---

## Release (für Maintainer)

Releases entstehen durch Push eines Versions-Tags. Der GitHub-Actions-Workflow baut dann die Desktop-Binaries (Linux, Windows, macOS) und stellt die Web-App auf GitHub Pages bereit.

- **Release erstellen:** `CHANGELOG.md` bei Bedarf anpassen, dann:
  ```bash
  git tag v1.0.0
  git push origin v1.0.0
  ```
- Der Workflow erstellt das GitHub-Release mit allen Artefakten und deployed die Web-App. Kein manuelles Hochladen.

---

## GitHub Pages (Web-App)

Einmalig: **Settings → Pages → Source: GitHub Actions.**  
Ab dem ersten Tag-Release: **https://benjarogit.github.io/the-old-game/**

---

## Builds (Desktop) bei Release

Bei Tag-Push (z. B. `v1.0.0`) baut der Workflow native Binaries:

| Plattform | Datei im Release |
|-----------|-------------------|
| **Windows** | `spiel-des-lebens-windows.exe` |
| **macOS** | `spiel-des-lebens-macos` |
| **Linux** | `spiel-des-lebens-linux` |

Eine Codebasis (C++ + raylib), CMake – Best Practice für plattformübergreifenden Desktop.

---

## Technik

- **Web:** HTML/CSS/JavaScript, Canvas, Touch-Events  
- **Desktop:** C++17, [raylib](https://www.raylib.com/) (FetchContent), CMake  
- **Regeln:** Geburt bei 3 Nachbarn, Überleben bei 2–3, sonst Tod; Torus-Rand  

---

© 2025–2026 Sunny C. · [GitHub](https://github.com/benjarogit/the-old-game)
