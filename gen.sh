#!/bin/bash

SRC=static
DST=data

rm -rf $DST
mkdir -p $DST
cp -r "$SRC/"* "$DST/"
find "$DST" -type f \( -name "*.htm" -o -name "*.css" -o -name "*.js" -o -name "*.ico" \) -exec gzip -f -k -9 {} +
find "$DST" -type f \( -name "*.htm" -o -name "*.css" -o -name "*.js" -o -name "*.ico" \) -exec rm -rf {} +
cp -r "$SRC/index.htm" "$DST/index.htm"
