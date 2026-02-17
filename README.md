# Conway's Game of Life

**Language:** [English](#) | [Deutsch](README.de.md)

---

Conway's cellular automaton: cells are born with 3 neighbors, survive with 2–3, else they die. The grid is toroidal (edges wrap). The game develops patterns by itself; you set the initial cells and run or step.

**Cross-platform:**

- **Web:** HTML/CSS/JS – runs in any browser (including GitHub Pages).
- **Desktop:** C++ with raylib + CMake – one codebase for **Windows, macOS, and Linux** (including WSL2).

- **Web app:** https://benjarogit.github.io/the-old-game/ (after first release)
- **Downloads (Desktop):** https://github.com/benjarogit/the-old-game/releases

---

## Run locally

### Web (browser)

Open `index.html` in your browser – no server required.

### Desktop (Windows, macOS, Linux)

**Requirements:** CMake, a C++17 compiler, and (for raylib, fetched via FetchContent):

- **Linux (Ubuntu/Debian):**  
  `sudo apt install build-essential cmake libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev`
- **macOS:** Xcode Command Line Tools (or Xcode).
- **Windows:** Visual Studio 2022 (or Build Tools) with C++ workload.

Install CMake locally, then:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/spiel-des-lebens          # Linux/macOS
# Windows: build\Release\spiel-des-lebens.exe
```

On **WSL2**, build and run the Linux binary as above.

---

## Controls

### Web

- **Language:** German (DE) and English (EN). The app uses your browser language; you can switch anytime with **DE | EN** in the header if detection is wrong. Your choice is stored locally.  
- **Mouse/touch:** Click or drag on the grid to set or clear cells.  
- **Buttons:** Start, Step, Clear. **Slider:** simulation speed.  
- **Space:** One step (one generation) when the simulation is **paused** – same as the Step button.  
- **Keys 1–7:** Presets (same as Desktop: 1=Block, 2=Blinker, 3=Beacon, 4=Toad, 5=Pulsar, 6=Glider, 7=LWSS).  
- **Presets:** Still Lives (e.g. Block), Oscillators (Blinker, Beacon, Toad, Pulsar), Spaceships (Glider, LWSS), and Random.

### Desktop (C++)

- **Language:** German and English. At startup the app uses the **OS UI language** (Windows: display language; Linux/macOS: `LANG` / `LC_ALL`). Press **L** anytime to switch if detection is wrong.  
- **Mouse:** Click/drag on the grid to set or clear cells.  
- **R:** Start / Pause. **Space:** One step (one generation) when **paused**. **C:** Clear grid.  
- **1–7:** Presets (1=Block, 2=Blinker, 3=Beacon, 4=Toad, 5=Pulsar, 6=Glider, 7=LWSS). **L:** Toggle language.  
- Sidebar: current generation.

### Presets and behavior

- **Still Lives** (e.g. Block) do not change over time – by design.  
- **Oscillators** (Blinker, Beacon, Toad, Pulsar) repeat a cycle. **Spaceships** (Glider, LWSS) move across the grid.  
- **Random** fills the grid randomly; some patterns stabilize, others move – both are normal.

---

## Release (for maintainers)

Releases are created by pushing a version tag. The GitHub Actions workflow then builds the desktop binaries (Linux, Windows, macOS) and deploys the web app to GitHub Pages.

- **Create a release:** Update `CHANGELOG.md` if needed, then:
  ```bash
  git tag v1.0.0
  git push origin v1.0.0
  ```
- The workflow creates the GitHub Release with all artifacts and deploys the web app. No manual uploads.

---

## GitHub Pages (web app)

One-time: **Settings → Pages → Source: GitHub Actions.**  
After the first tag release: **https://benjarogit.github.io/the-old-game/**

---

## Desktop builds on release

On tag push (e.g. `v1.0.0`) the workflow builds native binaries:

| Platform | File in release |
|----------|------------------|
| **Windows** | `spiel-des-lebens-windows.exe` |
| **macOS** | `spiel-des-lebens-macos` |
| **Linux** | `spiel-des-lebens-linux` |

One codebase (C++ + raylib), CMake – best practice for cross-platform desktop.

---

## Tech stack

- **Web:** HTML/CSS/JavaScript, Canvas, touch events  
- **Desktop:** C++17, [raylib](https://www.raylib.com/) (FetchContent), CMake  
- **Rules:** Birth with 3 neighbors, survive with 2–3, else death; toroidal boundary  

---

© 2025–2026 Sunny C. · [GitHub](https://github.com/benjarogit/the-old-game)
