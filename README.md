# SA.VehStaticShadUnlock

Enables built-in static vehicle shadows at all visual quality settings. The game normally draws them only on the lowest setting.

GTA San Andreas 1.0 US. Output: `output/Release/VehStaticShadUnlock.asi`.

## Prerequisites

- Xcode Command Line Tools: `make` and `/usr/bin/clangd`
- Homebrew: `mingw-w64`, `bear`, `premake` (`premake5` on `PATH`)
- A built [Plugin-SDK](https://github.com/DK22Pac/plugin-sdk) for GTA SA:
  - `$PLUGIN_SDK_DIR/output/lib/Plugin.a`
  - `$PLUGIN_SDK_DIR/output/lib/Plugin_d.a`

```sh
xcode-select --install
brew install mingw-w64 bear premake
```

## generate.sh

Set `PLUGIN_SDK_DIR` in `generate.sh` to your Plugin-SDK checkout, then:

```sh
./generate.sh
```

That script is:

```sh
export PLUGIN_SDK_DIR="/path/to/plugin-sdk"

rm -rf output
rm -rf build
premake5 gmake --file=premake5.lua
```

It deletes `build/` and `output/`, then writes Makefiles into `build/`.

## build.sh

Run this after `./generate.sh`:

```sh
./build.sh
```

That script is:

```sh
LC_ALL=C make -C build config=debug -Bnw | bear parse-sh
make -C build config=release
```

1. Dry-runs Debug and sends the commands to Bear. Bear writes `compile_commands.json` in the repo root. `LC_ALL=C` is required so Bear can parse Make.
2. Builds Release. The `.asi` is `output/Release/VehStaticShadUnlock.asi`.

`compile_commands.json` is gitignored. Run `./build.sh` again after changing includes, defines, or `premake5.lua`.

## clangd

`.clangd` tells clangd where the compilation database is:

```yaml
CompileFlags:
  CompilationDatabase: .
```

`./build.sh` must have run, so `compile_commands.json` exists.

The database calls `i686-w64-mingw32-g++`. clangd skips that compiler unless `--query-driver` matches it. Without the flag, MinGW and SDK headers do not resolve.

Zed (`.zed/settings.json`):

```json
{
  "lsp": {
    "clangd": {
      "binary": {
        "path": "/usr/bin/clangd",
        "arguments": [
          "--query-driver=/opt/homebrew/bin/i686-w64-mingw32-g++"
        ]
      }
    }
  }
}
```

Other editors: set the clangd binary to `/usr/bin/clangd` and add the same `--query-driver` argument.

Restart clangd after `compile_commands.json` changes.
