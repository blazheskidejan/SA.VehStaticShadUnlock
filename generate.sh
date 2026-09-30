#!/usr/bin/env bash
set -euo pipefail

export PLUGIN_SDK_DIR="${PLUGIN_SDK_DIR}"

rm -rf output
rm -rf build
premake5 gmake --file=premake5.lua
