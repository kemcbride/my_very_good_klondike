#!/usr/bin/env bash
# kill the process if it doesn't finish within 60 s - this is pretty generous
timeout 1 cat inputs/seed2_cmds.txt | ./solitaire play 2
