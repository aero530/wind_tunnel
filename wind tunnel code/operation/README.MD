# Wind Tunnel

This code runs a profile on the wind tunnel and collects data from the thermal load card and the hot wire anemometer.  It is super hacky and disgusting code that controls the actual wind tunnel using a python script and collects data from the hot wire anemometer and thermal load card using javascript.

## To run, please:
1. Install python3
2. Install python modules
	`pip install pyserial`
3. Install node/npm
4. Install the required packages by navigating to the containing directory and running `npm install`
5. Configure the settings for COM and test parameters in runTest.py and windTunnel.js
6. From the containing directory, execute `node windTunnel`