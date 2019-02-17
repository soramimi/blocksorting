#!/bin/sh

GR='\033[0;32m'
NC='\033[0m'

runthis() {
    printf "${GR}command executed: $@${NC}\n"
    eval "$@"
}

export PATH=$PATH:./bin/

printf "\t[Test de la coherence des resultats]\n\n"
runthis "mtf -e <LICENSE | mtf -d | diff -s -- LICENSE -"
runthis "bwt -e <LICENSE | bwt -d | diff -s -- LICENSE -"
runthis "rle -e <LICENSE | rle -d | diff -s -- LICENSE -"

runthis "bwt -e <LICENSE | mtf -d | mtf -e | bwt -d | diff -s -- LICENSE -"
runthis "bwt -e <LICENSE | mtf -d | mtf -e | rle -e | rle -d | bwt -d | diff -s -- LICENSE -"

PERF='perf_4.18' # pour tester avec une version specifique si besoin

printf "\n\n\t[Bench test]\n\n"
runthis "perf_4.18 stat -r 5 -- ./compress.sh ctc.txt"
runthis "perf_4.18 stat -r 5 -- ./uncompress.sh ctc.txt.mybz"
