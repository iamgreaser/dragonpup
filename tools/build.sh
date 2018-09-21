#!/bin/sh

# Pick a suitable TAP harness
if xpython3 -c "import tap.main" >/dev/null 2>&1; then
	tap_harness () { python3 -c 'import tap.main; tap.main.main(["-c", "-"])'; }
elif xpython -c "import tap.main" >/dev/null 2>&1; then
	tap_harness () { python -c 'import tap.main; tap.main.main(["-c", "-"])'; }
else
	# Fallback: Print failing tests
	tap_harness () { if grep '^not ok'; then false; else true; fi; }
fi

echo "dragonpup:"
cc -DSUPER_ZZT=0 -O1 -g -o bin/dragonpup src/*.c -I src/ && \
./bin/dragonpup --unit-tests | tap_harness && \
echo "OK!"

echo "super-dragonpup:"
cc -DSUPER_ZZT=1 -O1 -g -o bin/super-dragonpup src/*.c -I src/ && \
./bin/super-dragonpup --unit-tests | tap_harness && \
echo "OK!"

