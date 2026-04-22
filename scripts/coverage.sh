#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build"
COVERAGE_DIR="${ROOT_DIR}/coverage"
TEST_BIN="${ROOT_DIR}/raytracer_unit_tests"

cd "${ROOT_DIR}"

echo "[coverage] Configure CMake (ENABLE_COVERAGE=ON)"
cmake -S . -B "${BUILD_DIR}" -DENABLE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug

echo "[coverage] Build project"
cmake --build "${BUILD_DIR}" -j

if [[ ! -x "${TEST_BIN}" ]]; then
    echo "[coverage] No test binary found (${TEST_BIN})."
    echo "[coverage] Unit tests are likely skipped (for example if GoogleTest fetch failed)."
    exit 0
fi

echo "[coverage] Run unit tests"
"${TEST_BIN}"

echo "[coverage] Drop main coverage artifacts"
find "${BUILD_DIR}" \
    \( -path '*/CMakeFiles/raytracer.dir/main.cpp.gcda' \
    -o -path '*/CMakeFiles/raytracer.dir/main.cpp.gcno' \
    -o -name 'main.cpp.gcov.json.gz' \
    -o -name 'main.cpp##*.gcov.json.gz' \) \
    -delete

echo "[coverage] Generate text summary"
gcovr -r "${ROOT_DIR}" --txt --exclude 'build/_deps' --exclude 'tests' --exclude '(^|.*/)main\.cpp$' --gcov-ignore-errors output_error

echo "[coverage] Generate HTML report"
mkdir -p "${COVERAGE_DIR}"
gcovr -r "${ROOT_DIR}" --html-details "${COVERAGE_DIR}/index.html" --exclude 'build/_deps' --exclude 'tests' --exclude '(^|.*/)main\.cpp$' --gcov-ignore-errors output_error

echo "[coverage] Report generated: ${COVERAGE_DIR}/index.html"

if [[ "${1:-}" == "--open" ]]; then
    if command -v xdg-open >/dev/null 2>&1; then
        xdg-open "${COVERAGE_DIR}/index.html" >/dev/null 2>&1 || true
    else
        echo "[coverage] xdg-open not found, open the file manually."
    fi
fi
