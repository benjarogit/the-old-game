# Changelog

Alle nennenswerten Änderungen an diesem Projekt werden hier dokumentiert.

Format: [Keep a Changelog](https://keepachangelog.com/de/1.0.0/).

## [Unreleased]

## [1.2.1] - 2026-02-17

### Behoben
- Windows-Build: Konflikt mit Windows-API (CloseWindow, ShowCursor) durch #undef vor raylib behoben
- Windows-Build: MSVC C++-Syntax (Color{ }, Rectangle{ } statt C-Compound-Literale) für Release-Workflow

## [1.2.0] - 2026-02-17

### Hinzugefügt
- Desktop: resizable Fenster, Grid skaliert mit (FLAG_WINDOW_RESIZABLE, SetWindowMinSize)
- Desktop: Sidebar mit klickbaren Buttons (Start/Pause, Schritt, Löschen, Presets 1–7, Info)
- Desktop: Info-Dialog („So funktioniert’s“) mit Regeln und Erklärungen
- gitpush.sh im Repo für Commit + optional Tag-Release

### Geändert
- Desktop: raygui-Farben auf 0xAARRGGBB umgestellt (Darstellungsfehler behoben)
- Desktop: Hover-Farbe Buttons dezent (Grau statt Grün)
- Web: Optik an Product-Sites angelehnt (Karten, DM Sans, Farben)

## [1.1.0] - 2026-02-17

### Hinzugefügt
- Copyright in README (EN/DE), index.html (Footer), Desktop-Sidebar (main.cpp) und LICENSE (MIT)
- Web: Leertaste = ein Schritt (eine Generation), nur wenn pausiert
- Web: Tasten 1–7 für Presets (Parität mit Desktop: 1=Block … 7=LWSS)
- READMEs: Spielregeln kurz, Steuerung getrennt Web/Desktop, Erklärung „Leertaste = Schritt“, Preset-Kategorien (Still Lives, Oscillators, Spaceships), Hinweis dass Still Lives sich nicht bewegen und Zufall gemischt sein kann
- UI-Hinweis in der Web-App: „Tasten 1–7: Presets (wie Desktop). Leertaste: ein Schritt, wenn pausiert.“

### Geändert
- Release-Anleitung nur noch per Tag-Push (`git tag` + `git push origin <tag>`), keine Erwähnung von gitpush.sh im Repo
- Desktop-Sidebar: größere Schrift, mehr Zeilenabstand, höherer Kontrast (bessere Lesbarkeit)
- Web: Canvas-CSS robuster (aspect-ratio, min-height), damit Canvas sichtbar und klickbar bleibt
- Web: Beim Klick auf „Start“ wird sofort ein erster Schritt angezeigt

### Behoben
- Web: „Start tut nichts“ / „keine Formen hinzufügen“ durch stabilere Canvas-Darstellung adressiert

### Sonstiges
- gitpush.sh wird mit ins Repo committed (für Release per Tag)
