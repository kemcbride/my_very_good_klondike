#!/usr/bin/env bash
# kill the process if it doesn't finish within 1 s - usually i see ~0.4s
# Above is no longer true since the implementation of the hint command.
# set -x
DBG_TIMEOUT_LIMIT=120
OPT_TIMEOUT_LIMIT=10

TOTAL_CORRECTNESS_RESULT=0
TOTAL_TIMEOUT_RESULT=0

function e2e_test () {
  BINARY=$1
  TIMEOUT_LIMIT=$2
  SEED=$3
  INPUT_SCRIPT=$4

  # Used when updating the diff correctness check outputs
  # timeout $TIMEOUT_LIMIT sh -c "time ${BINARY} play ${SEED} < ${INPUT_SCRIPT} | tee outputs/${BINARY}_${SEED}.out"
  timeout $TIMEOUT_LIMIT sh -c "time ${BINARY} play ${SEED} < ${INPUT_SCRIPT} | tee ${BINARY}_${SEED}.out"
  TIMEOUT_RESULT=$?

  diff "${BINARY}_${SEED}.out" "outputs/${BINARY}_${SEED}.out"
  DIFF_RESULT=$?
  rm "${BINARY}_${SEED}.out"

  [[ "$DIFF_RESULT" = "0" && "$TOTAL_CORRECTNESS_RESULT" = "0" ]]
  TOTAL_CORRECTNESS_RESULT=$?
  echo " Total correctness result? $TOTAL_CORRECTNESS_RESULT";
  if [[ $DIFF_RESULT != 0 ]]; then
    echo "Correctness diff check for ${BINARY} ${SEED} FAILED ❌    ";
    return $DIFF_RESULT
  else
    echo "Correctness diff check for ${BINARY} ${SEED} PASSED ✔️   ";
  fi

  [[ "$TIMEOUT_RESULT" = "0" && "$TOTAL_TIMEOUT_RESULT" = "0" ]]
  TOTAL_TIMEOUT_RESULT=$?
  if [[ $TIMEOUT_RESULT = 124 ]]; then
    echo "Timeout check ${BINARY} ${SEED} with time limit ${TIMEOUT_LIMIT}s FAILED ❌P   ";
    exit $TIMEOUT_RESULT
  else
    echo "Timeout check ${BINARY} ${SEED} for ${TIMEOUT_LIMIT} PASSED ✔️   ";
  fi
  return $TIMEOUT_RESULT
}

e2e_test ./solitaire $OPT_TIMEOUT_LIMIT 4 inputs/seed4_another_input.txt

e2e_test ./solitaire $OPT_TIMEOUT_LIMIT 7 inputs/seed7_input.txt

e2e_test ./dbg_solitaire $DBG_TIMEOUT_LIMIT 4 inputs/seed4_another_input.txt

e2e_test ./dbg_solitaire $DBG_TIMEOUT_LIMIT 7 inputs/seed7_input.txt

echo "Correctness: ${TOTAL_CORRECTNESS_RESULT}; Timeout: ${TOTAL_TIMEOUT_RESULT}";

[[ "$TOTAL_CORRECTNESS_RESULT" = "0" && "$TOTAL_TIMEOUT_RESULT" = "0" ]]
exit $?
