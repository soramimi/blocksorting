#!/bin/sh

mtf -e <LICENSE | mtf -d | diff -s -- LICENSE -
bwt -e <LICENSE | bwt -d | diff -s -- LICENSE -
rle -e <LICENSE | rle -d | diff -s -- LICENSE -

bwt -e <LICENSE | mtf -d | mtf -e | bwt -d | diff -s -- LICENSE -
bwt -e <LICENSE | mtf -d | mtf -e | rle -e | rle -d | bwt -d | diff -s -- LICENSE -
