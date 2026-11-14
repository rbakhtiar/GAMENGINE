
#!/usr/bin/env bash

set -e

ENGINE="GAMENGINE"
VERSION="0.1.0"

case "${1:-help}" in
  version)
    echo "$ENGINE $VERSION"
    ;;
  info)
    echo "Independent Multi-Language Game Engine"
    echo "Core: C++"
    echo "Editor: Web"
    ;;
  check)
    echo "Checking project structure..."
    for folder in src native languages; do
      if [ -d "$folder" ]; then
        echo "[OK] $folder"
      else
        echo "[MISSING] $folder"
      fi
    done
    ;;
  help)
    echo "GAMENGINE CLI"
    echo "Commands: version, info, check, help"
    ;;
  *)
    echo "Unknown command: $1"
    exit 1
    ;;
esac
