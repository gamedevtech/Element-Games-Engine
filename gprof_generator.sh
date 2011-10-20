gprof Applications/Tests/Sandbox/element_games_sandbox | python gprof2dot.py | dot -Tpng -o gprof.png
gprof Applications/Tests/Sandbox/element_games_sandbox gmon.out > gprof_output
