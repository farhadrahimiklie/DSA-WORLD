#!/bin/bash

# Colors
RED="\033[31m"
GREEN="\033[32m"
RESET="\033[0m"

print_time(){
    local title="$1"
    local DATE=$(date "+%A, %d %B %Y")
    local TIME=$(date "+%I:%M:%S %p")
    echo -e "$title $DATE | $TIME"
}

print_time "Compilation ${GREEN}Started${RESET} at"

# ======================
# 1. COMPILE
# ======================
COMPILE_OUTPUT=$(gcc -Wall -Wextra -Wpedantic -Werror -g vector.c -o main 2>&1)
    COMPILE_STATUS=$?

    [ -n "$COMPILE_OUTPUT" ] && echo "$COMPILE_OUTPUT"

    if [ $COMPILE_STATUS -ne 0 ]; then

    if echo "$COMPILE_OUTPUT" | grep -q "unused variable"; then
    MSG="Unused Variable (Error)"

    elif echo "$COMPILE_OUTPUT" | grep -q "undeclared"; then
    MSG="Undeclared Variable"

    elif echo "$COMPILE_OUTPUT" | grep -q "undefined reference"; then
    MSG="Linker Error"

    else
    MSG="Syntax Error"
    fi

    print_time "Compilation ${RED}${MSG}${RESET} at"
    exit 1
    fi

# ======================
# 2. RUN PROGRAM
# ======================
RUN_OUTPUT=$(./main 2>&1)
    RUN_STATUS=$?

    [ -n "$RUN_OUTPUT" ] && echo "$RUN_OUTPUT"

    if [ $RUN_STATUS -ne 0 ]; then

    case $RUN_STATUS in
    139)
    MSG="Segmentation Fault"
    ;;
    136)
    MSG="Divide by Zero"
    ;;
    134)
    MSG="Aborted (abort())"
    ;;
    130)
    MSG="Interrupted (Ctrl+C / Infinite Loop)"
    ;;
    *)
    MSG="Runtime Error (code: $RUN_STATUS)"
    ;;
    esac

    print_time "Execution ${RED}${MSG}${RESET} at"
    exit 1
    fi

# ======================
# 3. SUCCESS
# ======================
    print_time "Compilation ${RED}Finished Successfully${RESET} at"
