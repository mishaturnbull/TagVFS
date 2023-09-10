# arguments "inspired by" (taken from):
# https://stackoverflow.com/a/44989219
# thanks, community wiki!

valgrind --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --verbose \
    --log-file=valgrind.log \
    $@

