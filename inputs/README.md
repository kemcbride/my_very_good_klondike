# Recording game inputs

How did I create the files that live in this folder?

These "integration tests" really save time for things that I haven't bothered to write proper unit tests for.

I did.. these things!

1. `script {testcasename}`
2. decide on a seed you want to use for the testcase so it can be reproducible in the test script! (ie. `./solitaire play {one-character seed}`
3. play the game through
4. close the game process (via normal exit command or via ctrl-c or ctrl-d)
5. kill the script process! (`killall script -s SIGUSR1` or something like that. perhaps kill -9 if you can't get SIGUSR1 to work.. like me)
6. inspect the file that script created - `{testcasename}` - it should contain both the inputs you gave as well as the program outputs
7. I filtered it down to the input commands with: `grep "cmd: " {testcasename} > {intermediate_filename}`
8. I filtered again - OK, not so much filtering, but sed - `sed 's/cmd: //' {intermediate_filename} > {next_intermediate_filename}`
9. OK, for me to get it to work properly, apparently the output of `script` was using DOS-style line-endings. So I used vim to convert it to unix-style. In vim with `{next_intermediate_filename}` open, doing: `:set fileformat=unix` and then saving the file in place.
10. OK, that actually makes `{next_intermediate_filename}` your final file. I guess this last step is manually examining the file! Make sure it looks like the actual sequence of commands you sent! For example, you might want to remove commands that were pretty much no-ops, and also clean up mistakes you had. NOTENOTENOTE NOTE!!! Script keeps EVERYTHING you did - and that means that it keeps your backspaces, so you'll have to manually clean those up in this step. Beware!


## Running game inputs

Checkout the script at the repo's top level, `full_solve_tests.sh` for how to run them. You don't need any of the time/timeout stuff, I just put it in there to force it to eventually die in case of.. infinite loops... :/
