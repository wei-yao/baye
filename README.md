# 步步高三国霸业 windows 版

## Prerequisites

1. **Install Qt5**
   - Download and install [Qt 5.x](https://download.qt.io/archive/qt/5.15/5.15.2/) (or your preferred 5.x version).
   - Make sure to install the MinGW-compatible version.
   - Add the following directories to your system `PATH`:
     - The `bin` directory of your Qt installation (e.g., `C:\Qt\5.15.2\mingw81_64\bin`)
     - The MinGW bundled with Qt (e.g., `C:\Qt\Qt5.14.2\Tools\mingw730_32\bin`)

## Generate MOC Files

Before building, you need to generate Qt MOC (Meta-Object Compiler) files:

1. Open PowerShell in the project directory.
2. Edit `createMOC.ps1` and update the `$qtPath` variable to point to your Qt5 installation's `bin` directory if needed.
3. Run the script:
   ```powershell
   ./createMOC.ps1
   ```
   This will generate the necessary `moc_*.cpp` files in the `moc/` directory.

## Build Commands

Open a terminal (cmd or PowerShell) in the project directory:

### Clean
```sh
mingw32-make clean
```

### Build
```sh
mingw32-make
```

### Run
```
need to copy the following dll from your qt folder to the bin folder
Qt5Core.dll
Qt5Gui.dll
Qt5Network.dll
Qt5Widgets.dll

```

## Notes
- If you encounter missing MOC files, re-run `createMOC.ps1`.
- If you change Qt header usage, re-run `createMOC.ps1` before building.
- If you use a different Qt or MinGW version, update paths accordingly in your environment and scripts.
- I am using vscode, you can see the recommend debug and build task definition under .vscode folder

## key change
 - 增加自动内政，自动招降（降低招降难度）
 - 自动征兵，分配给带将数最高的10个人
 - 存储城市信息为json方便改动
 - convert all file to c++ and use vector to modernlize the code (in progress)

## Troubleshooting
- Ensure all required Qt DLLs are available in your `PATH` or next to the executable.
- If you see errors about missing `*.h` or `*.cpp` files, check that MOC generation completed successfully. 

## todo
 - add step for what static file needed(qt lib,dat.lib)
 - convert save format completely to json
   - persons
   - orders
- rewrite the UI using different engine,to enable show more information it's too terrible.