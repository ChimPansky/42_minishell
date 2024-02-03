#!/bin/bash

NC='\033[0m' # No Color
BLACK='\033[0;30m'
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
GREY_L='\033[0;37m'
GREY_D='\033[1;30m'
RED_L='\033[1;31m'
GREEN_L='\033[1;32m'
YELLOW_I='\033[1;33m'
BLUE_L='\033[1;43m'
PURPLE_L='\033[1;35m'
CYAN_L='\033[1;36m'
WHITE='\033[1;37m'

MSH=$(pwd)/minishell
echo -e "minishell executable: ${PURPLE}$MSH${NC}"

cd tests

## for TEST_CASE : test_cases

TEST_CASE=test.msh

TEST_SCRIPT=test_cases/$TEST_CASE
TEST_OUTPUT_DIR=tests_output/$TEST_CASE
mkdir -p $TEST_OUTPUT_DIR
echo -e "\ntesting: ${CYAN}$TEST_CASE${NC}"
$MSH <$TEST_SCRIPT >$TEST_OUTPUT_DIR/msh.out 2> $TEST_OUTPUT_DIR/msh.err
MSH_EXIT=$?
bash <$TEST_SCRIPT >$TEST_OUTPUT_DIR/bash.out 2> $TEST_OUTPUT_DIR/bash.err
BASH_EXIT=$?
OUT_DIFF=$(diff --color=always $TEST_OUTPUT_DIR/msh.out $TEST_OUTPUT_DIR/bash.out)
ERR_DIFF=$(diff --color=always $TEST_OUTPUT_DIR/msh.err $TEST_OUTPUT_DIR/bash.err)
TEST_OK=true
if [[ $OUT_DIFF ]]; then
	TEST_OK=false
	echo -e "${RED}out diff:${NC}"
	echo -e $OUT_DIFF
fi
if [[ $ERR_DIFF ]]; then
	TEST_OK=false
	echo -e "${RED}err diff:${NC}"
	echo -e $ERR_DIFF
fi
if [[ $BASH_EXIT != $MSH_EXIT ]]; then
	TEST_OK=false
	echo -e "got ${YELLOW}$MSH_EXIT${NC} exit code, ${YELLOW}$BASH_EXIT${NC} was expected"
fi
if [[ TEST_OK == true ]]; then
	echo -e "${GREEN}OK${NC}"
else
	echo -e "${RED}KO${NC}"
fi
