#!/bin/sh

export PATH=$PATH:./bin/
rle -d <$1 | mtf -d | bwt -d >$1.uncompress
