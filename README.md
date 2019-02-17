# Block Sorting

Information de ma plateforme:

```
Intel(R) Core(TM) i5-4200M CPU @ 2.50GHz

$ uname -a
Linux pclt 4.17.0-3-amd64 #1 SMP Debian 4.17.17-1 (2018-08-18) x86_64 GNU/Linux

$ gcc --version
gcc (Debian 8.2.0-4) 8.2.0

$ make --version
GNU Make 4.2.1
```

## Rendu

Le rendu contient le codage et decodage de BWT, MTF et la version specifique de RLE.

Un script de demo a ete fourni qui test que nous obtenons bien le meme fichier apres different codage et decodage.
Ce script de demo necessite perf pour le bench.

L'equilibreur de probabilite n'a pas ete code par manque de temps.
Je pense que le codage arithmetique aurait ete le plus adapte dans notre cas.

Arborescence du rendu:
```
.
├── bin
├── compress.sh     # compresseur utilisant la suite bwt: mtf: rle
├── ctc.txt         # le cours
├── demo.sh         # demo montrant la consistence des resultats, suivi d'un bench avec perf
├── LICENSE
├── Makefile
├── README.md
├── src
│   ├── bwt.cpp     # code source de la transformee de burros wheeler
│   ├── mtf.cpp     # move to front
│   ├── parse.h     # petit utilitaire pour la mise en forme de l'entree
│   └── rle.cpp     # version specifique de rle comme demande
└── uncompress.sh   # decompresseur utilisant la suite inverse du compresseur rle: mtf: bwt
```
## Utilisation des programmes

Pour utiliser bwt, rle et mtf :
- utiliser l'option -e pour l'Encodage
- utiliser l'otion -d pour le Decodage

## Resultats

Compression des fichiers ctc.txt et LICENSE avec bwt:mtf:rle
```
        avant   apres   taux de compression
ctc.txt 117K    77K     0.65
LICENSE 35K     25K     0.71
```
J'ai essaye avec une image de Lena.
Le resultat est mauvais (258K avant compression vs 260K apres compression).

Je pense que cela est du au fait qu'il y a peu de similitude en prenant octet par octet pour une ligne.
Il faudrait introduire le parcours sous forme de courbe de Peano durant la phase de BWT, ainsi mtf et rle seront efficaces.

## Bench

Ce que j'obtiens sur ma machine lors de l'execution de demo.sh
```
	[Bench test]

command executed: perf_4.18 stat -r 5 -- ./compress.sh ctc.txt

 Performance counter stats for './compress.sh ctc.txt' (5 runs):

        238.204720      task-clock (msec)         #    1.013 CPUs utilized            ( +-  0.54% )
                68      context-switches          #    0.285 K/sec                    ( +-  0.86% )
                 2      cpu-migrations            #    0.010 K/sec                    ( +- 10.21% )
               809      page-faults               #    0.003 M/sec                    ( +-  0.17% )
       708,485,761      cycles                    #    2.974 GHz                      ( +-  0.09% )
       276,536,977      instructions              #    0.39  insn per cycle           ( +-  0.01% )
        50,130,367      branches                  #  210.451 M/sec                    ( +-  0.01% )
         2,144,981      branch-misses             #    4.28% of all branches          ( +-  0.03% )

          0.235096 +- 0.000893 seconds time elapsed  ( +-  0.38% )

command executed: perf_4.18 stat -r 5 -- ./uncompress.sh ctc.txt.mybz

 Performance counter stats for './uncompress.sh ctc.txt.mybz' (5 runs):

         25.423751      task-clock (msec)         #    1.122 CPUs utilized            ( +-  3.74% )
                66      context-switches          #    0.003 M/sec                    ( +-  1.13% )
                 2      cpu-migrations            #    0.087 K/sec                    ( +- 36.36% )
               896      page-faults               #    0.035 M/sec                    ( +-  0.69% )
        63,425,412      cycles                    #    2.495 GHz                      ( +-  0.57% )
       128,611,993      instructions              #    2.03  insn per cycle           ( +-  0.01% )
        27,383,713      branches                  # 1077.092 M/sec                    ( +-  0.01% )
           378,674      branch-misses             #    1.38% of all branches          ( +-  0.15% )

          0.022663 +- 0.000724 seconds time elapsed  ( +-  3.19% )
```
On remarque que la compression prend bien plus de temps que la decompression, tel qu'on s'y attend (pour bwt)

## Triturer les algos

Une chose que j'aurais aime tester est de passer par un parcours en courbe de Peano pour la bwt sur des images.

Concernant le texte, il aurait ete possible d'essayer de passer par une suite de compression differente apres la bwt classique.
Par exemple, au lieu de passer par un mtf suivi du rle specifique, essayer de passer par un rle classique (qui ne soit pas restreint a compresser les suites de zero).
