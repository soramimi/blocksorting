#!/bin/sh

export PATH=$PATH:./bin/
bwt -e <$1 | mtf -e | rle -e >$1.mybz
