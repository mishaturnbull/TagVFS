valgrind --tool=callgrind \
    --dump-instr=yes \
    --collect-jumps=yes \
    $@
