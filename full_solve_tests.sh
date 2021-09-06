#!/usr/bin/env bash
# kill the process if it doesn't finish within 1 s - usually i see ~0.4s
timeout 1 time cat inputs/seed2_cmds.txt | ./solitaire play 2 --autosolve=false
timeout 1 time cat inputs/seed2_autosolve_cmds.txt | ./solitaire play 2 --autosolve=true
