#!/bin/bash

# Setup
mkdir -p build/
gcc dedekind_eta.c -o build/dedekind_eta -lm

# Generate frames
for i in {1..30}; do
  echo $i;
  ./build/dedekind_eta -x 600 -y 600 -n $i > build/out$(($i+1000)).tga
done

# Merge frames
# -resize 50%
convert -delay 10 -loop 0 build/*.tga build/out.gif
