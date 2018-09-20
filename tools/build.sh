#!/bin/sh

cc -DSUPER_ZZT=0 -O1 -g -o bin/dragonpup src/*.c -I src/ && \
./bin/dragonpup --unit-tests

cc -DSUPER_ZZT=1 -O1 -g -o bin/super-dragonpup src/*.c -I src/ && \
./bin/super-dragonpup --unit-tests

