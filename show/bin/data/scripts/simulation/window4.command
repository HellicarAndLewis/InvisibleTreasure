#!/bin/bash

# =========================================================================
# DESCRIPTION
# starts up MicroMacro app with certain params

# =========================================================================
# PARAMETERS
 
# initial wait time before starting commands of this script in seconds
# to make sure that OS X does whatever it needs to do after startup
initial_wait_time=1

# =========================================================================

echo "Starting WINDOW"
#change to working directory
dir=${0%/*}
if [ -d "$dir" ]; then
  cd "$dir"
fi
echo "Waiting to start script..."
# wait before starting commands of this script in seconds
sleep $initial_wait_time
# start master application
echo "Launching application"
# logLevel: 0=verbose, 1=notice, 2=warning, 3=error, 4=fatal, 5=silent
../../../show.app/Contents/MacOS/show --args  -mode="WINDOW" -id=5 -windowId=4 -width=320 -height=240 -fullScreen=false -logLevel=1 -logToFile=false -appendToLog=false

exit 0