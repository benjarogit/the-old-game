/**
 * Conway's Game of Life – C++ desktop (Windows, macOS, Linux)
 * Rules: birth with 3 neighbors, survive with 2–3, else death
 * Sidebar: raygui for cross-platform, consistent GUI.
 */

#if defined(_WIN32) || defined(_WIN64)
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#include <windows.h>
#undef CloseWindow
#undef ShowCursor
#undef Rectangle
#endif

#include "raylib.h"
#include <cstdlib>
// raygui ruft TextToFloat auf, definiert es aber nur bei RAYGUI_STANDALONE – bei Nutzung mit raylib selbst bereitstellen
static float TextToFloat(const char *text) {
  return static_cast<float>(atof(text));
}
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <vector>
#include <ctime>
#include <cstring>
#include <cstdio>

const int CELL_SIZE = 8;
const int WIDTH = 800;
const int HEIGHT = 600;
const int SIDEBAR_W = 280;  /* Sidebar mit Buttons und Presets */
const int COLS = WIDTH / CELL_SIZE;
const int ROWS = HEIGHT / CELL_SIZE;

std::vector<std::vector<int>> grid(ROWS, std::vector<int>(COLS, 0));
std::vector<std::vector<int>> next(ROWS, std::vector<int>(COLS, 0));
bool running = false;
int generation = 0;
float runAccum = 0.f;
const float STEP_INTERVAL = 1.f / 8.f;  /* steps per second when running */

int mod(int a, int b) {
  int r = a % b;
  return r < 0 ? r + b : r;
}

int countNeighbors(int r, int c) {
  int n = 0;
  for (int dr = -1; dr <= 1; dr++)
    for (int dc = -1; dc <= 1; dc++) {
      if (dr == 0 && dc == 0) continue;
      int nr = mod(r + dr, ROWS);
      int nc = mod(c + dc, COLS);
      n += grid[nr][nc];
    }
  return n;
}

void step() {
  for (int r = 0; r < ROWS; r++)
    for (int c = 0; c < COLS; c++) {
      int n = countNeighbors(r, c);
      int alive = grid[r][c];
      next[r][c] = (alive && (n == 2 || n == 3)) || (!alive && n == 3) ? 1 : 0;
    }
  grid.swap(next);
  generation++;
}

void clearGrid() {
  for (int r = 0; r < ROWS; r++)
    for (int c = 0; c < COLS; c++)
      grid[r][c] = 0;
  generation = 0;
}

/** Place a pattern (row-major, pr x pc) centered on the grid */
void placePattern(const int* pattern, int pr, int pc) {
  int r0 = (ROWS - pr) / 2;
  int c0 = (COLS - pc) / 2;
  if (r0 < 0) r0 = 0;
  if (c0 < 0) c0 = 0;
  for (int r = 0; r < pr && r0 + r < ROWS; r++)
    for (int c = 0; c < pc && c0 + c < COLS; c++)
      grid[r0 + r][c0 + c] = pattern[r * pc + c];
}

/** Presets: 1=Block, 2=Blinker, 3=Beacon, 4=Toad, 5=Pulsar, 6=Glider, 7=LWSS. Place pattern centered; does not clear grid. */
void applyPreset(int id) {
  if (id == 1) { /* Block */
    const int p[] = { 1,1, 1,1 }; placePattern(p, 2, 2);
  } else if (id == 2) { /* Blinker */
    const int p[] = { 1,1,1 }; placePattern(p, 1, 3);
  } else if (id == 3) { /* Beacon */
    const int p[] = { 1,1,0,0, 1,1,0,0, 0,0,1,1, 0,0,1,1 }; placePattern(p, 4, 4);
  } else if (id == 4) { /* Toad */
    const int p[] = { 0,0,1,1,1,0, 1,1,1,0,0,0 }; placePattern(p, 2, 6);
  } else if (id == 5) { /* Pulsar */
    const int p[] = {
      0,0,1,1,1,0,0,0,1,1,1,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,
      1,0,0,0,0,1,0,1,0,0,0,0,1,
      1,0,0,0,0,1,0,1,0,0,0,0,1,
      1,0,0,0,0,1,0,1,0,0,0,0,1,
      0,0,1,1,1,0,0,0,1,1,1,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,1,1,1,0,0,0,1,1,1,0,0,
      1,0,0,0,0,1,0,1,0,0,0,0,1,
      1,0,0,0,0,1,0,1,0,0,0,0,1,
      1,0,0,0,0,1,0,1,0,0,0,0,1,
      0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,1,1,1,0,0,0,1,1,1,0,0
    };
    placePattern(p, 13, 13);
  } else if (id == 6) { /* Glider */
    const int p[] = { 0,1,0, 0,0,1, 1,1,1 }; placePattern(p, 3, 3);
  } else if (id == 7) { /* LWSS */
    const int p[] = { 0,1,1,0,0,0, 1,0,0,0,1,0, 1,0,0,0,0,1, 1,1,1,1,0,0 }; placePattern(p, 4, 6);
  }
}

void drawGrid(float scale, float offsetX, float offsetY, int contentW, int contentH) {
  Color bg = { 0x1a, 0x1a, 0x20, 255 };
  Color cell = { 0x7d, 0xd3, 0xb0, 255 };
  Color border = { 0x40, 0x40, 0x48, 255 };
  DrawRectangle(0, 0, contentW, contentH, bg);
  const float cs = (float)CELL_SIZE * scale;
  const float gap = scale > 1.f ? 1.f : (scale < 1.f ? 0.f : 0.5f);
  for (int r = 0; r < ROWS; r++)
    for (int c = 0; c < COLS; c++)
      if (grid[r][c])
        DrawRectangle(
          (int)(offsetX + c * cs),
          (int)(offsetY + r * cs),
          (int)(cs - gap) > 0 ? (int)(cs - gap) : 1,
          (int)(cs - gap) > 0 ? (int)(cs - gap) : 1,
          cell);
  DrawRectangleLines((int)offsetX, (int)offsetY, (int)(WIDTH * scale), (int)(HEIGHT * scale), border);
}

/** UI strings: index 0 = German, 1 = English */
static const char* const TITLE[] = { "Spiel des Lebens", "Game of Life" };
static const char* const RULES_INTRO[] = { "Regeln: 3 Nachbarn = Geburt, 2-3 = Ueberleben. Feld ringfoermig.", "Rules: 3 neighbors = birth, 2-3 = survive. Grid wraps." };
static const char* const R_START[] = { "R = Start", "R = Start" };
static const char* const R_PAUSE[] = { "R = Pause", "R = Pause" };
static const char* const SPACE_STEP[] = { "Leertaste = 1 Schritt", "Space = 1 step" };
static const char* const C_CLEAR[] = { "C = Loeschen", "C = Clear" };
static const char* const CLICK_CELLS[] = { "Links: 1 Klick = 1 Pixel. Ziehen = malen.", "Left: 1 click = 1 pixel. Drag = draw." };
static const char* const RIGHT_MOUSE[] = { "Rechts halten + Links = nur setzen (leben).", "Right hold + Left = set only (alive)." };
static const char* const PRESETS[] = { "1-7 = Presets (fuegen hinzu)", "1-7 = Presets (add, don't clear)" };
static const char* const COPYRIGHT[] = { "(c) 2025-2026 Sunny C.", "(c) 2025-2026 Sunny C." };
static const char* const L_HINT[] = { "L = Sprache", "L = Language" };
static const char* const RULES_HINT[] = { "Still Lifes bewegen sich nicht; Muster koennen stabil werden.", "Still Lifes don't move; patterns can stabilize." };
static const char* const STEP_DESC[] = { "Schritt: 1 Gen. (nur pausiert) = Test, wie sich ein Muster bewegt.", "Step: 1 gen. (paused only) = test how a pattern moves." };
static const char* const WRAP_DESC[] = { "Feld ringfoermig. Raus = gegenuber rein. Stosse loesen auf.", "Grid wraps. Exit = enter opposite. Collisions dissolve." };
static const char* const START_DESC[] = { "Start: 1 Muster (Glider). Mehr: Presets 1-7 oder zeichnen.", "Start: 1 pattern (Glider). More: presets 1-7 or draw." };
/* Info dialog: title and long message (DE/EN) */
static const char* const INFO_TITLE[] = { "So funktioniert's", "How it works" };
/* Preset button labels */
static const char* const P_BLOCK[]  = { "Block", "Block" };
static const char* const P_BLINKER[] = { "Blinker", "Blinker" };
static const char* const P_BEACON[]  = { "Beacon", "Beacon" };
static const char* const P_TOAD[]    = { "Toad", "Toad" };
static const char* const P_PULSAR[]   = { "Pulsar", "Pulsar" };
static const char* const P_GLIDER[]   = { "Glider", "Glider" };
static const char* const P_LWSS[]     = { "LWSS", "LWSS" };
static const char* const BTN_START[]  = { "Start", "Start" };
static const char* const BTN_PAUSE[] = { "Pause", "Pause" };
static const char* const BTN_STEP[]   = { "Schritt", "Step" };
static const char* const BTN_CLEAR[] = { "Loeschen", "Clear" };
static const char* const BTN_INFO[]   = { "Info / Erklaerung", "Info / Help" };
static const char* const GRP_CTRL[]   = { "Steuerung", "Controls" };
static const char* const GRP_PRESETS[] = { "Presets", "Presets" };

/** Detect OS UI language: 1 = German, 0 = English. Windows: GetUserDefaultUILanguage; Unix: LANG/LC_ALL. */
static int detectOsLanguage() {
#if defined(_WIN32) || defined(_WIN64)
  LANGID langId = GetUserDefaultUILanguage();
  return (PRIMARYLANGID(langId) == LANG_GERMAN) ? 1 : 0;
#else
  const char* langEnv = getenv("LC_ALL");
  if (!langEnv || !langEnv[0]) langEnv = getenv("LANG");
  if (!langEnv || !langEnv[0]) langEnv = getenv("LC_MESSAGES");
  if (langEnv && strlen(langEnv) >= 2 && langEnv[0] == 'd' && langEnv[1] == 'e')
    return 1;
  return 0;
#endif
}

int main() {
  int useGerman = detectOsLanguage();

  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(WIDTH + SIDEBAR_W, HEIGHT, TITLE[useGerman]);
  SetWindowMinSize(400, 300);
  SetTargetFPS(60);

  GuiLoadStyleDefault();
  /* Dark theme: raygui erwartet 0xAARRGGBB (Alpha high byte) */
  GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0xFF1a1a20);
  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0xFF252530);
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xFFf0f0f5);
  GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0xFF404048);
  GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_LEFT);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 16);
  GuiSetStyle(DEFAULT, TEXT_SPACING, 2);
  /* Buttons: dezent heller beim Hover (kein grelles Gruen) */
  GuiSetStyle(BUTTON, BORDER_WIDTH, 1);
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0xFF35354a);
  GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0xFF48485a);
  GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0xFFf0f0f5);
  GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0xFF505058);
  GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0xFF606070);

  bool drawing = false;
  bool showInfoDialog = false;
  int drawValue = 1;
  int startCellX = -1, startCellY = -1;  /* for drag detection: only draw when moved to another cell */
  applyPreset(6);  /* Glider on start */

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    const int winW = GetScreenWidth();
    const int winH = GetScreenHeight();
    int contentW = winW - SIDEBAR_W;
    int contentH = winH;
    if (contentW < 80) contentW = 80;
    if (contentH < 60) contentH = 60;
    const float scale = (float)contentW / (float)WIDTH < (float)contentH / (float)HEIGHT
      ? (float)contentW / (float)WIDTH
      : (float)contentH / (float)HEIGHT;
    const float offsetX = (contentW - WIDTH * scale) * 0.5f;
    const float offsetY = (contentH - HEIGHT * scale) * 0.5f;

    if (running) {
      runAccum += dt;
      while (runAccum >= STEP_INTERVAL) {
        runAccum -= STEP_INTERVAL;
        step();
      }
    }

    Vector2 mouse = GetMousePosition();
    int cellX = -1, cellY = -1;
    if (mouse.x >= offsetX && mouse.x < offsetX + WIDTH * scale && mouse.y >= offsetY && mouse.y < offsetY + HEIGHT * scale) {
      cellX = (int)((mouse.x - offsetX) / ((float)CELL_SIZE * scale));
      cellY = (int)((mouse.y - offsetY) / ((float)CELL_SIZE * scale));
      if (cellX >= COLS) cellX = COLS - 1;
      if (cellY >= ROWS) cellY = ROWS - 1;
    }
    const bool rightHeld = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    if (cellX >= 0 && cellX < COLS && cellY >= 0 && cellY < ROWS) {
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        /* Single click = one pixel. Drag = draw only when moved to another cell. */
        drawValue = rightHeld ? 1 : (grid[cellY][cellX] ? 0 : 1);
        grid[cellY][cellX] = drawValue;
        startCellX = cellX;
        startCellY = cellY;
        drawing = false;
      } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (cellX != startCellX || cellY != startCellY)
          drawing = true;
        if (drawing)
          grid[cellY][cellX] = drawValue;
      }
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      drawing = false;
      startCellX = -1;
      startCellY = -1;
    }

    if (IsKeyPressed(KEY_SPACE) && !running)
      step();
    if (IsKeyPressed(KEY_R))
      running = !running;
    if (IsKeyPressed(KEY_C)) {
      clearGrid();
      running = false;
    }
    if (IsKeyPressed(KEY_ONE)) applyPreset(1);
    if (IsKeyPressed(KEY_TWO)) applyPreset(2);
    if (IsKeyPressed(KEY_THREE)) applyPreset(3);
    if (IsKeyPressed(KEY_FOUR)) applyPreset(4);
    if (IsKeyPressed(KEY_FIVE)) applyPreset(5);
    if (IsKeyPressed(KEY_SIX)) applyPreset(6);
    if (IsKeyPressed(KEY_SEVEN)) applyPreset(7);
    if (IsKeyPressed(KEY_L)) {
      useGerman = 1 - useGerman;
      SetWindowTitle(TITLE[useGerman]);
    }

    BeginDrawing();
    ClearBackground(Color{ 0x1a, 0x1a, 0x20, 255 });
    drawGrid(scale, offsetX, offsetY, contentW, contentH);
    Rectangle sidebarRect = { (float)contentW, 0, (float)SIDEBAR_W, (float)winH };
    GuiPanel(sidebarRect, NULL);
    const float pad = 12.f;
    float y = 10.f;
    const float lineH = 22.f;
    const float btnH = 26.f;
    const float gap = 4.f;
    const float w = (float)SIDEBAR_W - 2.f * pad;
    const float sx = (float)contentW + pad;

    /* Steuerung: Gen, Start/Pause, Schritt, Loeschen */
    const float ctrlH = 22.f + lineH + gap + btnH + 12.f;
    GuiGroupBox(Rectangle{ sx, y, w, ctrlH }, GRP_CTRL[useGerman]);
    y += 20.f;
    GuiLabel(Rectangle{ sx, y, w * 0.55f, lineH }, TextFormat("Gen: %d", generation));
    y += lineH + gap;
    float bw = (w - 2.f * gap) / 3.f;
    if (GuiButton(Rectangle{ sx, y, bw, btnH }, running ? BTN_PAUSE[useGerman] : BTN_START[useGerman]))
      running = !running;
    if (GuiButton(Rectangle{ sx + bw + gap, y, bw, btnH }, BTN_STEP[useGerman]) && !running)
      step();
    if (GuiButton(Rectangle{ sx + 2.f * (bw + gap), y, bw, btnH }, BTN_CLEAR[useGerman])) {
      clearGrid();
      running = false;
    }
    y += btnH + 14.f;

    /* Presets: 7 Buttons, einheitliche Hoehe */
    const float presetBtnH = 24.f;
    const float presetsH = 20.f + 7.f * (presetBtnH + 4.f) + 6.f;
    GuiGroupBox(Rectangle{ sx, y, w, presetsH }, GRP_PRESETS[useGerman]);
    y += 20.f;
    const float pw = w;
    if (GuiButton(Rectangle{ sx, y, pw, presetBtnH }, P_BLOCK[useGerman]))  applyPreset(1);
    y += presetBtnH + 4.f;
    if (GuiButton(Rectangle{ sx, y, pw, presetBtnH }, P_BLINKER[useGerman])) applyPreset(2);
    y += presetBtnH + 4.f;
    if (GuiButton(Rectangle{ sx, y, pw, presetBtnH }, P_BEACON[useGerman]))  applyPreset(3);
    y += presetBtnH + 4.f;
    if (GuiButton(Rectangle{ sx, y, pw, presetBtnH }, P_TOAD[useGerman]))    applyPreset(4);
    y += presetBtnH + 4.f;
    if (GuiButton(Rectangle{ sx, y, pw, presetBtnH }, P_PULSAR[useGerman]))  applyPreset(5);
    y += presetBtnH + 4.f;
    if (GuiButton(Rectangle{ sx, y, pw, presetBtnH }, P_GLIDER[useGerman])) applyPreset(6);
    y += presetBtnH + 4.f;
    if (GuiButton(Rectangle{ sx, y, pw, presetBtnH }, P_LWSS[useGerman]))   applyPreset(7);
    y += presetBtnH + 10.f;

    /* Info-Button: oeffnet Erklaerung */
    if (GuiButton(Rectangle{ sx, y, w, btnH }, BTN_INFO[useGerman]))
      showInfoDialog = true;
    y += btnH + 8.f;

    GuiLabel(Rectangle{ sx, y, w, lineH }, RULES_INTRO[useGerman]);
    y += lineH;
    GuiLabel(Rectangle{ sx, y, w, lineH }, TextFormat("%s  |  %s  |  1-7  |  %s", SPACE_STEP[useGerman], C_CLEAR[useGerman], L_HINT[useGerman]));
    y += lineH;
    GuiLabel(Rectangle{ sx, y, w, lineH * 1.1f }, CLICK_CELLS[useGerman]);
    y += lineH + 2.f;
    GuiLabel(Rectangle{ sx, y, w, lineH * 1.1f }, RIGHT_MOUSE[useGerman]);
    y += lineH + 8.f;
    GuiLabel(Rectangle{ sx, y, w, 18.f }, COPYRIGHT[useGerman]);

    /* Modal Info-Dialog */
    if (showInfoDialog) {
      static char infoBuf[1200];
      snprintf(infoBuf, sizeof(infoBuf),
        "%s\n\n%s\n\n%s\n\n%s\n\n%s\n%s",
        STEP_DESC[useGerman], WRAP_DESC[useGerman], START_DESC[useGerman],
        RULES_HINT[useGerman], CLICK_CELLS[useGerman], RIGHT_MOUSE[useGerman]);
      Rectangle msgRect{ winW / 2.f - 190.f, winH / 2.f - 140.f, 380.f, 280.f };
      int r = GuiMessageBox(msgRect, INFO_TITLE[useGerman], infoBuf, "OK");
      if (r == 0) showInfoDialog = false;
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
