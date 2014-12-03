#!/bin/bash

# Piero Toffanin 2012 Raytracer Test Script 
# http://www.pierotoffanin.com

# Usage: ./runtests.sh <directory>
# Examples
#		 ./runtests.sh all
#		 ./runtests.sh spheres
#
# Output: images will be stored in results


# Override this command to execute your raytracer
COMMAND="../../bin/raytracer -w 600 -h 600 -r 2"

if [ "$1" == "" ]; then
	echo "Usage: ./runtests.sh <directory>"
	echo "Example: ./runtests.sh all"
	exit 1
fi

if [ ! -d "$1" ]; then
	echo "Invalid directory $1"
	echo "Exiting..."
	exit 1
fi

TESTS_DIRECTORY="$1"

# Touch anything below only if you know what you're doing
echo "Start testing..."

eval "rm results/*"
eval "rm failed.log"

failed_tests=( )
num_tests_failed=0

for testfile in `ls -a1 --color=none ${TESTS_DIRECTORY}/*.xml | sed s/"${TESTS_DIRECTORY}\/"/''/g | sed s/'\.xml'/''/g` ; do
	eval "${COMMAND} -o results/${testfile}.png -i ${TESTS_DIRECTORY}/${testfile}.xml"

	# Failed?
	if [ $? -ne 0 ]; then
		failed_tests[${num_tests_failed}]=${testfile}
		num_tests_failed=$(($num_tests_failed + 1))
	fi
done

for failedtest in ${failed_tests[@]}
do
	echo "${failedtest}.${TESTS_DIRECTORY} failed (output below):" >> failed.log
	echo "---------" >> failed.log
	eval "${COMMAND} -o results/${failedtest}.png -i ${TESTS_DIRECTORY}/${failedtest}.xml" >> failed.log
	echo "---------" >> failed.log
	echo "" >> failed.log
done

echo "Testing completed."

if [ ${num_tests_failed} -eq 0 ]; then
	echo "All files were processed with no errors."
else
	echo "${num_tests_failed} file(s) reported errors (check failed.log)"
fi
