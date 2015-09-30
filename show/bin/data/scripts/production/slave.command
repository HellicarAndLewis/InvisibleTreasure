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

echo "Starting SLAVE"
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
../../../show.app/Contents/MacOS/show --args  -mode="SLAVE" -id=1 -width=1024 -height=768 -fullScreen=true -logLevel=3 -logToFile=true -appendToLog=true

exit 0