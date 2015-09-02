#!/bin/bash

# Clones dependencies
# !Run this script from the project root!
# In terminal cd to project root then "sh cloneaddons.sh"

# switch to addons directory
cd ../../addons

# passing no arguments means read only
if [ -z $1 ]; then
CLONE="git clone https://github.com/"
echo "Cloning read-only"
else
CLONE="git clone git@github.com:"
echo "Cloning writable"
fi

# git clone specific revision of the addons
# this ensures that things won't break if repositories are updated

${CLONE}elliotwoods/ofxLiquidEvent
cd ofxLiquidEvent
# git checkout ???
cd ..

${CLONE}gameoverhack/ofxBezierWarp
cd ofxBezierWarp
# git checkout ???
cd ..

${CLONE}bakercp/ofxIpVideoGrabber.git
cd ofxIpVideoGrabber
# git checkout ???
cd ..

${CLONE}kylemcdonald/ofxCv.git
cd ofxCv
# git checkout ???
cd ..

${CLONE}armadillu/ofxRemoteUI
cd ofxRemoteUI
# git checkout 3c73fbed1e004c8bd0c542985088b14a6969a648
cd ..

${CLONE}outsidecontext/ofxArgs
cd ofxArgs
# git checkout 9dfbf491591ebe0f8a83fb4ab675116b9d1040ce
cd ..

${CLONE}julapy/ofxFFT
cd ofxFFT
# git checkout f33c6d581b9a912eb3d24d2413c5aced866cf5b9
cd ..

${CLONE}arturoc/ofxTween
cd ofxTween
# git checkout ???
cd ..


# final step is to cd back to the start dir
cd ../apps/invisibleTreasure