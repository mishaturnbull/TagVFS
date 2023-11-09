# This script adapted from https://stackoverflow.com/a/68236609 .  Thanks,
# StackOverflow user Sessile Computing & Cody Guldner!

# Clean up old runtime data and previous coverage
find . -name '*.gcda' -exec rm {} \;
if [ -d "build/coverage" ]; then
    rm -rf build/coverage
fi

# Run test command (specified as script arguments)
# This can be anything that runs program(s) built by the project 
"${@}"

# Generate reports (.gcov files). All .gcda files in the build directory are
# passed as gcov args.
find . -name '*.gcda' | xargs gcov

# collect all the coverage files and stash them in build/coverage
mkdir -p build/coverage
mv *.gcov build/coverage/

# generate info files
lcov --directory build \
    -c \
    --output-file build/coverage/lcov.info \
    --rc lcov_branch_coverage=1

mkdir -p build/coverage/html
genhtml -o build/coverage/html build/coverage/lcov.info \
    --rc lcov_branch_coverage=1

if [ -d "doc/_build/html" ]; then
	mv build/coverage/html doc/_build/html/cov
fi

