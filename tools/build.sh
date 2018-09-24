#!/bin/sh

USE_SDL=0

if [ "x${USE_SDL}" = "x1" ]; then
	SDL_FLAGS=`sdl2-config --cflags --libs`
else
	SDL_FLAGS=
fi

# Pick a suitable TAP harness
if python3 -c "import tap.main" >/dev/null 2>&1; then
	tap_harness () { python3 -c 'import tap.main; tap.main.main(["-c", "-"])'; }
elif python -c "import tap.main" >/dev/null 2>&1; then
	tap_harness () { python -c 'import tap.main; tap.main.main(["-c", "-"])'; }
else
	# Fallback: Print failing tests
	tap_harness () { if grep '^not ok'; then false; else echo "OK!"; true; fi; }
fi

echo "dragonpup:"
cc -DSUPER_ZZT=0 -DUSE_SDL=${USE_SDL} -O0 -g -o bin/dragonpup src/*.c -I src/ ${SDL_FLAGS} && \
./bin/dragonpup --unit-tests | tap_harness

echo "super-dragonpup:"
cc -DSUPER_ZZT=1 -DUSE_SDL=${USE_SDL} -O1 -g -o bin/super-dragonpup src/*.c -I src/ ${SDL_FLAGS} && \
./bin/super-dragonpup --unit-tests | tap_harness

