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

${CLONE}kylemcdonald/ofxBlackmagic
cd ofxBlackmagic
git checkout bef9cb5547198fe842fce9d631b05d2be28a2322
cd ..

${CLONE}obviousjim/ofxSlitScan
cd ofxSlitScan
git checkout 78cdd0dc9fff4eac93406c13057064e5a3abb8b3
cd ..

# Remove old version before cloning new one!
rm -rf ofxCvOpticalFlowLK
${CLONE}outsidecontext/ofxCvOpticalFlowLK
cd ofxCvOpticalFlowLK
git checkout cbbf63e14e87133769fa183d1dd64133ef6e9fa2
cd ..

${CLONE}armadillu/ofxRemoteUI
cd ofxRemoteUI
git checkout 3c73fbed1e004c8bd0c542985088b14a6969a648
cd ..

${CLONE}memo/ofxMSAInteractiveObject
cd ofxMSAInteractiveObject
git checkout 13eb17852016e9aa2c2eaac0d20613709b609a10
cd ..

${CLONE}outsidecontext/ofxArgs
cd ofxArgs
git checkout 9dfbf491591ebe0f8a83fb4ab675116b9d1040ce
cd ..

${CLONE}julapy/ofxFFT
cd ofxFFT
git checkout f33c6d581b9a912eb3d24d2413c5aced866cf5b9
cd ..

${CLONE}outsidecontext/ofxPSLevels
cd ofxPSLevels
git checkout ed248e911fb51ae544acc4e59a497c44c2b5b422
cd ..


# final step is to cd back to the start dir
cd ../apps/MicroMacro