#!/usr/bin/env bash
# kill the process if it doesn't finish within 1 s - usually i see ~0.4s
# Above is no longer true since the implementation of the hint command.
# set -x
DBG_TIMEOUT_LIMIT=60
OPT_TIMEOUT_LIMIT=5

TOTAL_CORRECTNESS_RESULT=0
TOTAL_TIMEOUT_RESULT=0

function e2e_test () {
  BINARY=$1
  MODE=$2
  TIMEOUT_LIMIT=$3
  SEED=$4
  INPUT_SCRIPT=$5

  # Used when updating the diff correctness check outputs
  timeout $TIMEOUT_LIMIT sh -c "time ${BINARY} play ${SEED} < ${INPUT_SCRIPT} | tee outputs/${SEED}.out"
  timeout $TIMEOUT_LIMIT sh -c "time ${BINARY} play ${SEED} < ${INPUT_SCRIPT} | tee ${MODE}_${SEED}.out"
  TIMEOUT_RESULT=$?

  diff "${MODE}_${SEED}.out" "outputs/${SEED}.out"
  DIFF_RESULT=$?
  rm "${MODE}_${SEED}.out"

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

e2e_test ./bazel-out/k8-fastbuild-clang-opt/bin/solitaire opt $OPT_TIMEOUT_LIMIT 0 inputs/log_bazel_0_again.txt

e2e_test ./bazel-out/k8-fastbuild-clang-opt/bin/solitaire opt $OPT_TIMEOUT_LIMIT 1 inputs/log_bazel_1_again.txt

e2e_test ./bazel-out/k8-fastbuild-clang-opt/bin/solitaire opt $OPT_TIMEOUT_LIMIT 5 inputs/5.txt

e2e_test ./bazel-out/k8-dbg-clang-dbg/bin/solitaire dbg $DBG_TIMEOUT_LIMIT 0 inputs/log_bazel_0_again.txt

e2e_test ./bazel-out/k8-dbg-clang-dbg/bin/solitaire dbg $DBG_TIMEOUT_LIMIT 1 inputs/log_bazel_1_again.txt

e2e_test ./bazel-out/k8-dbg-clang-dbg/bin/solitaire dbg $DBG_TIMEOUT_LIMIT 5 inputs/5.txt

echo "Correctness: ${TOTAL_CORRECTNESS_RESULT}; Timeout: ${TOTAL_TIMEOUT_RESULT}";

[[ "$TOTAL_CORRECTNESS_RESULT" = "0" && "$TOTAL_TIMEOUT_RESULT" = "0" ]]
exit $?
