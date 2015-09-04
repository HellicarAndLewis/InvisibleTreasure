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

open ./window.command
open ./window.command
open ./window.command
open ./window.command
open ./slave.command
open ./master.command

exit 0