# Local Hub
local hub is gonna be a cool LAN messenger with a pretty cute UI design only for linux!

## what the hell is LAN messenger ?
LAN messengers are some kind of messengers that transmits data between two devices within a network. [LAN messenger](https://en.wikipedia.org/wiki/LAN_messenger)

## How to run ?
1. clone the project into your computer .
2. navigate to the project's directory .
3. make the project .
```bash
qmake -project
```
4. edit the `local_hub.pro` file .
```bash
vi local_hub.pro
```
5. add this line under the `# Input` section .
```bash
QT += core gui widgets
```
6. add the `window.cpp` to the `SOURCES` section .
```bash
SOURCES += main.cpp window.cpp
```
7. then simply run the `make.sh` script to compile the project .
```bash
./build.sh
```
8. run the application .
```bash
./local_hub
```

## Additional information about Local Hub
| **Connection type** | p2p connection |
|      **Port**       |     50000      |
|      **Linux**      |     true       |
|     **windows**     |     true       |
|       **Mac**       |   not tested   |
