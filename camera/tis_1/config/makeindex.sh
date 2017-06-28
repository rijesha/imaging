#!/bin/bash

# ----------------------------------------------------------------------------------
# Usage: ./makeindex.sh
# ----------------------------------------------------------------------------------
# Scans local folder in search for configuration files (.cfg), reads them and
# creates an index in a text file for easy reference
#
# Remember to give execute permission to the script by:
# $ chmod +x /path/to/script.sh
# ----------------------------------------------------------------------------------

{

echo "==============================================================================="
echo "Configuration files index (generated automatically using makeindex.sh)"
echo "Do not modify manually. If updates are required, run makeindex.sh"
echo "==============================================================================="
echo ""
echo "filename        brightness    gain    exp_abs"
echo "---------------------------------------------"

shopt -s nullglob

for f in *.cfg
do
	# load configuration file
	source ${f}
	printf "%s %10s %10s %10s\n" ${f} ${BRIGHTNESS} ${GAIN} ${EXPOSURE_ABSOLUTE}
done

} | tee "index.txt"
