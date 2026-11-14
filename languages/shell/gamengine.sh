
#!/usr/bin/env bash

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
CLI="$ROOT/native/build/gamengine-cli"

if [[ ! -x "$CLI" ]]; then
    echo "GAMENGINE CLI is not built."
    echo "Build native/ first."
    exit 1
fi

case "${1:-help}" in
    version)
        echo "GAMENGINE 0.2.0"
        ;;
    info)
        echo "GAMENGINE Language Bridge"
        echo "Native core: C++20"
        echo "Public interface: C ABI"
        ;;
    run)
        exec "$CLI"
        ;;
    build)
        cmake -S "$ROOT/native" -B "$ROOT/native/build"
        cmake --build "$ROOT/native/build"
        ;;
    *)
        echo "GAMENGINE Shell Interface"
        echo "Commands:"
        echo "  version"
        echo "  info"
        echo "  build"
        echo "  run"
        ;;
esac
