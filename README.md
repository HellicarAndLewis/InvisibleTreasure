# InvisibleTreasure
Show control, Computer Vision and AudioVisual cueing system for an experimental theatre project.

## Summary
Built using openFrameworks 0.84 on OSX 10.10. The main application can run in one of three modes: master, slave and show. Master controls others via OSC.


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
The app makes uses of the built in openFramweworks GUI. To hide/show it, hit space. The app uses multiple, nested GUI panels that can be toggled by their parent, allowing a large number of controls to be displayed in an elegant way.


## Dependencies
-	[openframeworks 0.84](http://www.openframeworks.cc/download/)
-   Various ofxaddons, see cloneaddons.sh for more info.
-   The master application makes use of the LANs broadcast address and will not work if this feature is disabled or blocked on the local network.