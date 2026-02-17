/**
 * Conway's Game of Life – cross-platform web version
 * Rules: birth with 3 neighbors, survive with 2–3, else death
 */

/** i18n: detect browser language, allow switch; strings DE/EN */
const i18n = {
  de: {
    title: "Conways Spiel des Lebens",
    heading: "Spiel des Lebens",
    subtitle: "John Conways zellulärer Automat · 1970",
    btnRun: "▶ Start",
    btnPause: "⏸ Pause",
    btnStep: "⏭ Schritt",
    btnClear: "🗑 Löschen",
    speed: "Geschwindigkeit",
    presets: "Presets",
    presetHint: "Tasten 1–7: Presets (wie Desktop). Leertaste: ein Schritt, wenn pausiert.",
    catStill: "Still Lives",
    catOsc: "Oscillators",
    catShip: "Spaceships",
    catOther: "Sonst",
    presetBlock: "Block",
    presetBlinker: "Blinker",
    presetBeacon: "Beacon",
    presetToad: "Toad",
    presetPulsar: "Pulsar",
    presetGlider: "Glider",
    presetLwss: "LWSS",
    presetRandom: "Zufall",
    genLabel: "Generation:",
    footerHint: "Klicke oder ziehe auf dem Feld, um Zellen zu setzen. Läuft auf allen Geräten im Browser.",
    rulesHint: "Still Lifes (z. B. Block) bewegen sich nicht. Viele Muster werden mit der Zeit stabil oder erlöschen – das gehört zu den Regeln.",
    rulesShort: "Regeln: 3 Nachbarn = Geburt, 2–3 = Überleben, sonst Tod. Feld ringförmig (raus = gegenüber rein).",
    presetAddHint: "Presets werden in die Mitte gesetzt und löschen das Feld nicht (nur „Zufall“ ersetzt alles).",
    howToStart: "Start: Simulation läuft weiter (Pause stoppt). Beim Start ist ein Glider vorgegeben – wie in der Desktop-Version.",
    howToStep: "Schritt (oder Leertaste): genau eine Generation weiter – nur wenn pausiert. So siehst du, wie sich ein Muster entwickelt.",
    wrapHint: "Das Feld ist ringförmig: Figuren, die rauswandern, erscheinen gegenüber wieder. Wenn sie auf andere treffen, können sie verschwinden – das ist Teil der Regeln.",
    startWithPatterns: "Du startest mit einem Muster (Glider). Weitere mit Presets 1–7 oder durch Zeichnen hinzufügen.",
    mouseHint: "Links: 1 Klick = 1 Pixel. Ziehen = malen. Rechts halten + Links = nur setzen (leben)."
  },
  en: {
    title: "Conway's Game of Life",
    heading: "Game of Life",
    subtitle: "John Conway's cellular automaton · 1970",
    btnRun: "▶ Start",
    btnPause: "⏸ Pause",
    btnStep: "⏭ Step",
    btnClear: "🗑 Clear",
    speed: "Speed",
    presets: "Presets",
    presetHint: "Keys 1–7: Presets (same as desktop). Space: one step when paused.",
    catStill: "Still Lives",
    catOsc: "Oscillators",
    catShip: "Spaceships",
    catOther: "Other",
    presetBlock: "Block",
    presetBlinker: "Blinker",
    presetBeacon: "Beacon",
    presetToad: "Toad",
    presetPulsar: "Pulsar",
    presetGlider: "Glider",
    presetLwss: "LWSS",
    presetRandom: "Random",
    genLabel: "Generation:",
    footerHint: "Click or drag on the grid to set or clear cells. Runs in any browser.",
    rulesHint: "Still Lifes (e.g. Block) don't move. Many patterns stabilize or die out over time – that's part of the rules.",
    rulesShort: "Rules: 3 neighbors = birth, 2–3 = survive, else death. Grid wraps (exit = enter opposite).",
    presetAddHint: "Presets are placed in the center and do not clear the grid (only „Random“ replaces all).",
    howToStart: "Start: run the simulation (Pause stops it). A Glider is shown at start – same as the desktop version.",
    howToStep: "Step (or Space): advance exactly one generation – only when paused. Use it to see how a pattern evolves.",
    wrapHint: "The grid wraps: patterns that leave one edge reappear on the opposite side. When they hit others, they can disappear – that's part of the rules.",
    startWithPatterns: "You start with one pattern (Glider). Add more with presets 1–7 or by drawing.",
    mouseHint: "Left: 1 click = 1 pixel. Drag = draw. Right hold + Left = set only (alive)."
  }
};

function getPreferredLang() {
  try {
    const saved = localStorage.getItem("the-old-game-lang");
    if (saved === "de" || saved === "en") return saved;
  } catch (_) {}
  const browser = typeof navigator !== "undefined" && navigator.language ? navigator.language.toLowerCase() : "";
  return browser.startsWith("de") ? "de" : "en";
}

let currentLang = getPreferredLang();

function setLanguage(lang) {
  if (lang !== "de" && lang !== "en") return;
  currentLang = lang;
  document.documentElement.lang = lang;
  try { localStorage.setItem("the-old-game-lang", lang); } catch (_) {}
  document.querySelectorAll("[data-i18n]").forEach(el => {
    const key = el.getAttribute("data-i18n");
    if (key && i18n[lang][key] !== undefined) {
      if (el.tagName === "TITLE") document.title = i18n[lang][key];
      else el.textContent = i18n[lang][key];
    }
  });
  const runBtn = document.getElementById("btn-run");
  if (runBtn) runBtn.textContent = running ? i18n[lang].btnPause : i18n[lang].btnRun;
}

const canvas = document.getElementById('grid');
const ctx = canvas.getContext('2d');

/** Cell size in pixels; grid dimensions derived from canvas */
const cellSize = 8;
let cols = Math.floor(canvas.width / cellSize);
let rows = Math.floor(canvas.height / cellSize);

/** Double buffer: current and next generation */
let grid = [];
let next = [];

let running = false;
let generation = 0;
let intervalId = null;

/** Steps per second from slider (1–30) */
const speedInput = document.getElementById('speed');
function stepsPerSecond() {
  return Math.max(1, Number(speedInput.value));
}

function initGrid() {
  grid = Array.from({ length: rows }, () => Array(cols).fill(0));
  next = Array.from({ length: rows }, () => Array(cols).fill(0));
}

function countNeighbors(r, c) {
  let n = 0;
  for (let dr = -1; dr <= 1; dr++) {
    for (let dc = -1; dc <= 1; dc++) {
      if (dr === 0 && dc === 0) continue;
      const nr = (r + dr + rows) % rows;
      const nc = (c + dc + cols) % cols;
      n += grid[nr][nc];
    }
  }
  return n;
}

function step() {
  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      const n = countNeighbors(r, c);
      const alive = grid[r][c];
      if (alive) {
        next[r][c] = n === 2 || n === 3 ? 1 : 0;
      } else {
        next[r][c] = n === 3 ? 1 : 0;
      }
    }
  }
  [grid, next] = [next, grid];
  generation++;
  document.getElementById('gen').textContent = generation;
}

function draw() {
  ctx.fillStyle = '#18181f';
  ctx.fillRect(0, 0, canvas.width, canvas.height);

  ctx.fillStyle = '#5dd4a8';
  for (let r = 0; r < rows; r++) {
    for (let c = 0; c < cols; c++) {
      if (grid[r][c]) {
        ctx.fillRect(c * cellSize, r * cellSize, cellSize - 1, cellSize - 1);
      }
    }
  }
}

function tick() {
  step();
  draw();
}

function startLoop() {
  if (intervalId) clearInterval(intervalId);
  const ms = 1000 / stepsPerSecond();
  intervalId = setInterval(() => {
    if (!running) {
      clearInterval(intervalId);
      intervalId = null;
      return;
    }
    tick();
  }, ms);
}

/** Map pointer event to grid cell (handles canvas scaling). Returns null if canvas has no size. */
function getCellCoords(e) {
  const rect = canvas.getBoundingClientRect();
  if (rect.width <= 0 || rect.height <= 0) return null;
  const scaleX = canvas.width / rect.width;
  const scaleY = canvas.height / rect.height;
  const x = Math.floor((e.clientX - rect.left) * scaleX / cellSize);
  const y = Math.floor((e.clientY - rect.top) * scaleY / cellSize);
  return { c: Math.max(0, Math.min(x, cols - 1)), r: Math.max(0, Math.min(y, rows - 1)) };
}

let isDrawing = false;
let drawValue = 1;
let startCellR = -1, startCellC = -1;

canvas.addEventListener('contextmenu', (e) => e.preventDefault());

canvas.addEventListener('mousedown', (e) => {
  if (running || e.button !== 0) return;
  const coords = getCellCoords(e);
  if (!coords) return;
  const { r, c } = coords;
  const rightHeld = (e.buttons & 2) !== 0;
  drawValue = rightHeld ? 1 : (grid[r][c] ? 0 : 1);
  grid[r][c] = drawValue;
  draw();
  startCellR = r;
  startCellC = c;
  isDrawing = false;
});

canvas.addEventListener('mousemove', (e) => {
  if (!(e.buttons & 1) || running) return;
  const coords = getCellCoords(e);
  if (!coords) return;
  const { r, c } = coords;
  if (startCellR >= 0 && (r !== startCellR || c !== startCellC)) isDrawing = true;
  if (isDrawing) {
    grid[r][c] = drawValue;
    draw();
  }
});

canvas.addEventListener('mouseup', (e) => {
  if (e.button === 0) { isDrawing = false; startCellR = -1; startCellC = -1; }
});
canvas.addEventListener('mouseleave', () => { isDrawing = false; startCellR = -1; startCellC = -1; });

/** Touch support for mobile */
canvas.addEventListener('touchstart', (e) => {
  e.preventDefault();
  if (running) return;
  const t = e.touches[0];
  const coords = getCellCoords(t);
  if (!coords) return;
  const { r, c } = coords;
  drawValue = grid[r][c] ? 0 : 1;
  grid[r][c] = drawValue;
  draw();
  isDrawing = true;
}, { passive: false });

canvas.addEventListener('touchmove', (e) => {
  e.preventDefault();
  if (!isDrawing || running) return;
  const t = e.touches[0];
  const coords = getCellCoords(t);
  if (!coords) return;
  const { r, c } = coords;
  grid[r][c] = drawValue;
  draw();
}, { passive: false });

canvas.addEventListener('touchend', () => { isDrawing = false; });

/** Button handlers */
document.getElementById('btn-run').addEventListener('click', () => {
  running = !running;
  const btn = document.getElementById('btn-run');
  btn.textContent = running ? i18n[currentLang].btnPause : i18n[currentLang].btnRun;
  if (running) {
    tick(); // first step visible immediately
    startLoop();
  } else if (intervalId) {
    clearInterval(intervalId);
    intervalId = null;
  }
});

document.getElementById('btn-step').addEventListener('click', () => {
  if (running) return;
  step();
  draw();
});

document.getElementById('btn-clear').addEventListener('click', () => {
  if (running) return;
  initGrid();
  generation = 0;
  document.getElementById('gen').textContent = 0;
  draw();
});

speedInput.addEventListener('input', () => {
  if (!running || !intervalId) return;
  clearInterval(intervalId);
  startLoop();
});

/** Presets: patterns centered on grid (Baeldung categories: Still Lives, Oscillators, Spaceships) */
const presets = {
  block: [[1,1],[1,1]],
  blinker: [[1,1,1]],
  beacon: [[1,1,0,0],[1,1,0,0],[0,0,1,1],[0,0,1,1]],
  toad: [[0,0,1,1,1,0],[1,1,1,0,0,0]],
  pulsar: [
    [0,0,1,1,1,0,0,0,1,1,1,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0],
    [1,0,0,0,0,1,0,1,0,0,0,0,1],
    [1,0,0,0,0,1,0,1,0,0,0,0,1],
    [1,0,0,0,0,1,0,1,0,0,0,0,1],
    [0,0,1,1,1,0,0,0,1,1,1,0,0],
    [0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,1,1,1,0,0,0,1,1,1,0,0],
    [1,0,0,0,0,1,0,1,0,0,0,0,1],
    [1,0,0,0,0,1,0,1,0,0,0,0,1],
    [1,0,0,0,0,1,0,1,0,0,0,0,1],
    [0,0,0,0,0,0,0,0,0,0,0,0,0],
    [0,0,1,1,1,0,0,0,1,1,1,0,0]
  ],
  glider: [[0,1,0],[0,0,1],[1,1,1]],
  lwss: [[0,1,1,0,0,0],[1,0,0,0,1,0],[1,0,0,0,0,1],[1,1,1,1,0,0]],
  random: null
};

/** Apply preset by name. Named presets are placed centered (grid unchanged otherwise). "random" clears and fills randomly. */
function applyPresetByName(name) {
  if (running) return;
  if (name === 'random') {
    initGrid();
    generation = 0;
    document.getElementById('gen').textContent = 0;
    for (let r = 0; r < rows; r++)
      for (let c = 0; c < cols; c++)
        grid[r][c] = Math.random() < 0.2 ? 1 : 0;
  } else {
    const pattern = presets[name];
    const pr = pattern.length;
    const pc = pattern[0].length;
    const r0 = Math.floor((rows - pr) / 2);
    const c0 = Math.floor((cols - pc) / 2);
    for (let r = 0; r < pr; r++)
      for (let c = 0; c < pc; c++)
        if (pattern[r][c]) grid[r0 + r][c0 + c] = 1;
  }
  draw();
}

/** Preset by number 1–7 (parity with desktop): 1=Block, 2=Blinker, 3=Beacon, 4=Toad, 5=Pulsar, 6=Glider, 7=LWSS */
const presetById = ['', 'block', 'blinker', 'beacon', 'toad', 'pulsar', 'glider', 'lwss'];
function applyPresetById(id) {
  if (id >= 1 && id <= 7) applyPresetByName(presetById[id]);
}

document.querySelectorAll('[data-preset]').forEach(btn => {
  btn.addEventListener('click', () => {
    applyPresetByName(btn.dataset.preset);
  });
});

document.addEventListener('keydown', (e) => {
  if (e.code === 'Space' && !running) {
    e.preventDefault();
    step();
    draw();
    return;
  }
  if (!running && e.code >= 'Digit1' && e.code <= 'Digit7') {
    applyPresetById(parseInt(e.code.slice(5), 10));
  }
});

document.getElementById('lang-de').addEventListener('click', () => setLanguage('de'));
document.getElementById('lang-en').addEventListener('click', () => setLanguage('en'));

setLanguage(currentLang);
initGrid();
/* Same start as desktop: one Glider in the center */
applyPresetByName('glider');
draw();
