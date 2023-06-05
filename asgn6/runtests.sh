#!/bin/bash

# Run tests
#
# Inputs:               Any file whose name matches 'files/*.txt'.
# Intermediate Files:   File names found above with 'txt' changed to 'huff'.
# Outputs:              File names found above with 'txt' changed to 'dehuff'.

# How it works:
#
# The "for" line creates a list of files to process.
# Each of those filenames is presented as $x to the './huff' command,
# which uses it an an input filename.
#
# But there's a curious part:
#
#     ${x%txt}huff
#
# This means "take $x and delete the 'txt' at the end".
# Then the 'huff' appended to it (look below) creates the desired
# output filename.
#
# This construction is repeated for the ./dehuff command, which reads a
# .huff file and makes a .dehuff file.

for x in temp/*.txt; do
  ./huff -i $x -o ${x%txt}huff
  if [ $? -ne 0 ];
  then
    echo "./huff returned error code" $?
    exit 1
  fi
  ./dehuff -i ${x%txt}huff -o ${x%txt}dehuff
  if [ $? -ne 0 ];
  then
    echo "./dehuff returned error code" $?
    exit 1
  fi
  diff $x ${x%txt}dehuff
  if [ $? -ne 0 ];
  then
    echo "$x: there's a difference"
    exit 1
  fi
done

echo "The script runtests.sh was executed successfully."
