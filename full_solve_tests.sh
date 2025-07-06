#!/usr/bin/env bash
# kill the process if it doesn't finish within 1 s - usually i see ~0.4s
# Above is no longer true since the implementation of the hint command.
TIMEOUT_LIMIT=120
timeout $TIMEOUT_LIMIT sh -c "time ./solitaire play 4 < inputs/seed4_another_input.txt"
TIMEOUT_RESULT=$?
if [[ $TIMEOUT_RESULT = 124 ]]; then
	echo "Timed out running example full test with time limit ${TIMEOUT_LIMIT}s";
	exit $TIMEOUT_RESULT
else
	echo "Did not time out yay";
fi

# Repeat for next test input - TODO should function-ize this
timeout $TIMEOUT_LIMIT sh -c "time ./solitaire play 7 < inputs/seed7_input.txt"
TIMEOUT_RESULT=$?
if [[ $TIMEOUT_RESULT = 124 ]]; then
	echo "Timed out running example full test with time limit ${TIMEOUT_LIMIT}s";
	exit $TIMEOUT_RESULT
else
	echo "Did not time out yay";
fi
