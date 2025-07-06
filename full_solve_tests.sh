#!/usr/bin/env bash
# kill the process if it doesn't finish within 1 s - usually i see ~0.4s
# Above is no longer true since the implementation of the hint command.
TIMEOUT_LIMIT=30
# timeout 1 time cat inputs/seed2_cmds.txt | ./solitaire play 4 --autosolve=false
timeout $TIMEOUT_LIMIT sh -c "time ./solitaire play 4 < inputs/seed4_another_input.txt"
# time ./solitaire play 4 < inputs/seed4_another_input.txt
TIMEOUT_RESULT=$?
if [[ $TIMEOUT_RESULT = 124 ]]; then
	echo "Timed out running example full test with time limit 10s";
else
	echo "Did not time out yay";
fi
exit $TIMEOUT_RESULT
