# README

![image](https://github.com/DuGuYifei/DroneSimulatorMonitor/assets/79463006/4b8282fe-b4b7-4989-b789-2bcee75ccb6d)

## Description
Project structure:
1. UE + Cesium + Miscroft AirSim => Flight Simulator
2. Python (Airsim API + PyGame) => Flight Controller
3. Node.js (Express + Socket.io) => Server
4. Cesium.js => Client

## Installation

1. FlightSimulator.exe can be found in release.
2. Controller need `pip install -r requirements.txt`
3. Server and Client need `Node app.js` to run it.

## Usage
1. If you have XBox controller, you can use it to control the drone without python script. From my side, it is not work very well, but maybe because I use PS4 controller to simulate XBox controller.
2. If you use XBox controller or use PS4 controller to simulate XBox controller, you can use `python stick_controller_xbox.py.py` to start the controller.
   1. Click `X` in PS4 or `A` in XBox to take off the drone.
   2. Click `O` in PS4 or `B` in XBox will make the drone land.
3. If you use keyboard directly, which will not have different velocity, use `python controller.py`. Btw, it use opencv `im.show` to see some image from drone camera.
4. `Nodde app.js` to start the server, and the client will be start in the port `3000`.
5. When you start `FlightSimulator.exe`, it will load the tile map from google api, so you need wait 15 seconds to load the map.
6. In client, click `follow view` will follow the move or freely move the camera.
7. In client, click `google map` will show / hide the 3d tiles.

### Keyboard or Gamepad
1. `wasd` or `left stick` move forward/back or left/right.
2. `arrow key` or `right stick` move up/down or rotate in yaw.
3. `k` in keyboard / `x` in PS4 / `A` in XBOX take off
4. `l` in keyboard / `x` in PS4 / `A` in XBOX land
5. `esc` / `left spcial key` in game pad quit game.

## Other Information
1. `stick_controller.py` is used for PS4 controller, but I have no time to fix bug in it.
2. `Google tile map api` will be deprecated by me in the future, so if you want to use it in the future, you need rebuild the project.
