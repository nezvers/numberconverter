# numberconverter
 A tool to see converted value from integer, hex and binary + ability to operate on it
- Place compiled executable reachable from cmd/powershell/terminal
- Enter expression as a string with single number (int, binary, hex) or [number operator number]
- Supports operators +, -, *, /, %, &, |, ^, <<, >>
- Represents output in all 3 formats - integer, binary, hex
- `numberconverter "420 << 2"`
- `numberconverter "0xff69"`
- `numberconverter "0b10110 & 1"`

## Build
```
git clone https://github.com/nezvers/numberconverter.git
cd numberconverter
```

##### Visual Studio
- Open folder with `Visual Studio`
- Wait until `Output` tab says `CMake generation finished`
- In dropdown menu next to "Play" button choose `numberconverter.exe`

##### GCC `".exe" for windows`
```
gcc src/main.c -o numberconverter.exe
```

##### Cmake
- Cmake should be installed and accessible from cmd/powershell/terminal
```
cmake -S . -B ./build -G "${YOUR_BUILD_TARGET_CHOICE}"
```

###### Example - Windows Mingw with optional debug build
- Mingw64 should be installed and `mingw64/bin` folder added to environment `PATH` variable
```
cmake -S . -B ./build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
mingw32-make -C ./build
./build/numberconverter.exe "0b10110 >> 1"
```