#!/usr/bin/env bash
# kill the process if it doesn't finish within 1 s - usually i see ~0.4s
# Above is no longer true since the implementation of the hint command.
NONOPT_TIMEOUT_LIMIT=120
OPT_TIMEOUT_LIMIT=5

function e2e_test () {
  BINARY=$1
  TIMEOUT_LIMIT=$2
  SEED=$3
  INPUT_SCRIPT=$4

  timeout $TIMEOUT_LIMIT sh -c "time ${BINARY} play ${SEED} < ${INPUT_SCRIPT}"
  TIMEOUT_RESULT=$?

  if [[ $TIMEOUT_RESULT = 124 ]]; then
    echo "Timed out running example full test with time limit ${TIMEOUT_LIMIT}s";
    exit $TIMEOUT_RESULT
  else
    echo "Did not time out yay";
  fi
  return $TIMEOUT_RESULT
}

e2e_test ./solitaire $NONOPT_TIMEOUT_LIMIT 4 inputs/seed4_another_input.txt

e2e_test ./solitaire $NONOPT_TIMEOUT_LIMIT 7 inputs/seed7_input.txt

e2e_test ./opt_solitaire $OPT_TIMEOUT_LIMIT 4 inputs/seed4_another_input.txt

e2e_test ./opt_solitaire $OPT_TIMEOUT_LIMIT 7 inputs/seed7_input.txt
