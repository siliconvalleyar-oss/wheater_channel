#!/bin/bash
set -euo pipefail

# Semantic version tag creator
# Usage: ./script_tools/version.sh [patch|minor|major]

LEVEL="${1:-patch}"

if ! command -v git &> /dev/null; then
    echo "Error: git no encontrado"
    exit 1
fi

LAST_TAG=$(git describe --tags --abbrev=0 2>/dev/null || echo "v0.0.0")

if [[ ! "$LAST_TAG" =~ ^v([0-9]+)\.([0-9]+)\.([0-9]+)$ ]]; then
    echo "Error: tag inválido '$LAST_TAG'. Esperado formato vX.Y.Z"
    exit 1
fi

MAJOR="${BASH_REMATCH[1]}"
MINOR="${BASH_REMATCH[2]}"
PATCH="${BASH_REMATCH[3]}"

case "$LEVEL" in
    major)
        MAJOR=$((MAJOR + 1))
        MINOR=0
        PATCH=0
        ;;
    minor)
        MINOR=$((MINOR + 1))
        PATCH=0
        ;;
    patch)
        PATCH=$((PATCH + 1))
        ;;
    *)
        echo "Uso: $0 [patch|minor|major]"
        exit 1
        ;;
esac

NEW_TAG="v${MAJOR}.${MINOR}.${PATCH}"

echo "Tag actual: $LAST_TAG"
echo "Nuevo tag:  $NEW_TAG"

git tag "$NEW_TAG"
git push origin "$NEW_TAG"

echo "Tag $NEW_TAG creado y pusheado exitosamente"
