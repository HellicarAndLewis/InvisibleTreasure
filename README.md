# InvisibleTreasure
Show control, Computer Vision and AudioVisual cueing system for an experimental theatre project.

## Summary
Built using openFrameworks 0.84 on OSX 10.10. The main application can run in one of three modes: master, slave and window. Master controls others via OSC.


## Local Dev
### Project
-   Download openframeworks 0.84 and extract it
-   cd into apps and clone this repo `git clone git@github.com:HellicarAndLewis/InvisibleTreasure.git`
-   download the video content from [github](https://github.com/HellicarAndLewis/InvisibleTreasure/releases) and unzip it into show/bin/data
-   cd into the repo root `cd InvisibleTreasure` and clone additional addon dependencies using the script provided `. ./cloneaddons.sh`

### Axis 
Connect the axis IP camera to the POE switch and the switch to your computer. Setup the camera as follows:
-   Download and install the [Camera Companion PC client](http://www.axis.com/my/en/products/axis-camera-companion#). (Windows only)
-   Run the client: follow the steps and update the firmware.
-   Setup the camera to expose the overview image as mjpeg at 1024x768.
-   Once setup, the motion jpg should then be accessible at http://{CAM_IP}/axis-cgi/mjpg/video.cgi. Note: you won’t be able to access this link in a browser. To check the feed, just visit the camera IP address. You should see the overview mjpeg.


### Settings & Control
The app makes uses of the built in openFramweworks GUI. To hide/show it, hit space. The app uses multiple, nested GUI panels that can be toggled by their parent, allowing a large number of controls to be displayed in an elegant way. Each panel saves to a separate XML file under bin/data/settings. OSC and IP cam connection settings cannot be changed at runtime: to change these, run the app once and click save on eac panel to generate the XML. You can then edit the XML to specify IP/port/URLs and then restart the app.

The app can run as master, slave or window. To change mode at runtime, hit 'm', 's' or 'w' respectively.

The app can be run from a shell script and accepts certain arguments e.g. `-mode="SLAVE" -width=640 -height=240 -fullScreen=false -logLevel=1 -logToFile=false -appendToLog=false`. See below for how this is used to achieve a local simulation.

### Local simulation
You can simulate the whole show on one machine by lauching multiple instances of the app in different modes. To do this, the app parses arguments that can be passed in via a shell script. Unders show/bin/data/scripts there are 4 launchable scripts. 3 launch the app in a certain mode, and all.command launches a full simulation: 4 windows, 1 master and 1 slave. Before running the simulation you'll need to have compiled in release mode and configured local settings.


## Dependencies
-	[openframeworks 0.84](http://www.openframeworks.cc/download/)
-   Various ofxaddons, see cloneaddons.sh for more info.
-   The master application makes use of the LANs broadcast address and will not work if this feature is disabled or blocked on the local network.