# arguments "inspired by" (taken from):
# https://stackoverflow.com/a/44989219
# thanks, community wiki!

valgrind --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --verbose \
    --log-file=valgrind.log \
    $@

