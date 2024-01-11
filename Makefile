# Set default path for HTSlib
HTSLIB_PATH ?= /default/path/to/htslib

CC=gcc
CFLAGS=-std=c99 -O3 -I$(HTSLIB_PATH)
LDFLAGS=-L$(HTSLIB_PATH) -lhts

# Target rule for bamSCPercent
bamSCPercent: bamSCPercent.c
	$(CC) $(CFLAGS) -o bamSCPercent bamSCPercent.c $(LDFLAGS)
