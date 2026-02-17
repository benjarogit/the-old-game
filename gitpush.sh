#!/usr/bin/env bash
# gitpush.sh – Commit, optional Changelog, Tag, Push
# Repo: https://github.com/benjarogit/the-old-game
# Nutzung:
#   ./gitpush.sh "Fix: Button-Label"              # nur commit + push
#   ./gitpush.sh "Release v1.2.0" "1.2.0" "…"   # Changelog-Eintrag, Commit, Tag v1.2.0, Push (→ Workflow baut Release)

set -e
REPO="${REPO:-origin}"
CHANGELOG="CHANGELOG.md"

if [[ $# -lt 1 ]]; then
  echo "Verwendung: $0 \"Commit-Nachricht\" [Version] [Changelog-Text]"
  echo "            $0 --tag v1.2.0   # nur Tag setzen + pushen (CHANGELOG unveraendert)"
  echo "  Nur Commit:  $0 \"Fix: etwas geändert\""
  echo "  Mit Release: $0 \"Release v1.2.0\" \"1.2.0\" \"Kurzbeschreibung\"  # + Tag, startet Workflow"
  exit 1
fi

# Nur Tag setzen und pushen (z. B. wenn CHANGELOG schon gepflegt ist)
if [[ "$1" == "--tag" && -n "$2" ]]; then
  TAG="${2#v}"
  TAG="v${TAG}"
  git tag -a "$TAG" -m "Release $TAG"
  git push "$REPO" "$TAG"
  echo "→ Tag $TAG gepusht. Release-Workflow startet auf GitHub."
  exit 0
fi

COMMIT_MSG="$1"
VERSION=""
CHANGELOG_DESC=""

if [[ -n "$2" ]]; then
  VERSION="${2#v}"
  CHANGELOG_DESC="${3:-Release $VERSION}"
fi

# Changelog aktualisieren wenn Version angegeben
if [[ -n "$VERSION" ]]; then
  TODAY=$(date +%Y-%m-%d)
  if [[ ! -f "$CHANGELOG" ]]; then
    echo "CHANGELOG.md nicht gefunden."
    exit 1
  fi
  # Neuen Release-Block nach [Unreleased] einfügen (portabel)
  {
    echo "## [Unreleased]"
    echo ""
    echo "## [$VERSION] - $TODAY"
    echo "### Hinzugefügt"
    echo "- $CHANGELOG_DESC"
    # Inhalt unter [Unreleased] übernehmen (ab Zeile 4: erste Listenpunkte)
    tail -n +4 "$CHANGELOG"
  } > "$CHANGELOG.tmp"
  mv "$CHANGELOG.tmp" "$CHANGELOG"
  git add "$CHANGELOG"
fi

git add -A
if git diff --staged --quiet && [[ -z "$VERSION" ]]; then
  echo "Keine Änderungen zum Committen."
  exit 0
fi

git commit -m "$COMMIT_MSG"
git push "$REPO" main 2>/dev/null || git push "$REPO" master 2>/dev/null || git push "$REPO" HEAD

if [[ -n "$VERSION" ]]; then
  TAG="v$VERSION"
  git tag -a "$TAG" -m "$COMMIT_MSG"
  git push "$REPO" "$TAG"
  echo "→ Tag $TAG erstellt und gepusht. Release-Workflow startet auf GitHub."
fi

echo "Fertig."
