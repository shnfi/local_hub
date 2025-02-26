# Local Hub
local hub is gonna be a cool LAN messenger with a pretty cute UI design only for linux!

## what the hell is LAN messenger ?
LAN messengers are some kind of messengers that transmits data between two devices within a network. [LAN messenger](https://en.wikipedia.org/wiki/LAN_messenger)

## How to run ?
1. clone the project into your computer .
2. navigate to the project's directory .
3. first of all make the project .
```bash
./first_make.sh
```
4. modify the `local_hub.pro` file, and add this line under `# Input` :
```
QT += core gui widgets network
```
5. then build the project .
```bash
./bulid.sh
```

## Additional information about Local Hub
| Connection type | Port | Linux | windows | Mac |
| :---: | :---: | :---: | :---: | :---: |
|p2p connection | 50000 | true | true | not tested |
