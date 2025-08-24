#!/bin/bash

# --- 설정 ---
MINISHELL_EXEC="./minishell"
BASH_EXEC="bash"

# --- 색상 코드 ---
COLOR_RESET='\033[0m'
COLOR_GREEN='\033[0;32m'
COLOR_RED='\033[0;31m'
COLOR_YELLOW='\033[0;33m'
COLOR_CYAN='\033[0;36m'
COLOR_GRAY='\033[0;90m'

# --- 임시 파일 ---
MINISHELL_OUT="minishell_out.tmp"
MINISHELL_ERR="minishell_err.tmp"
BASH_OUT="bash_out.tmp"
BASH_ERR="bash_err.tmp"

# --- 카운터 ---
test_count=0
pass_count=0

# --- 함수 ---

print_header() {
    echo -e "\n${COLOR_CYAN}========== $1 ==========${COLOR_RESET}"
}

setup() {
    echo -e "${COLOR_CYAN}--- 테스트 환경 설정 중... ---${COLOR_RESET}"
    echo "hello world" > infile.txt
    echo "this is a test file" >> infile.txt
    echo "another line with hello" >> infile.txt
    
    make
    if [ ! -f "$MINISHELL_EXEC" ]; then
        echo -e "${COLOR_RED}오류: minishell 실행 파일이 없습니다. 컴파일을 확인하세요.${COLOR_RESET}"
        exit 1
    fi
    echo -e "${COLOR_CYAN}--- 설정 완료 ---"
}

cleanup() {
    echo -e "\n${COLOR_CYAN}--- 테스트 환경 정리 중... ---${COLOR_RESET}"
    rm -f $MINISHELL_OUT $MINISHELL_ERR $BASH_OUT $BASH_ERR
    rm -f infile.txt outfile.txt append_file.txt empty_file.txt
    rm -rf test_dir
    echo -e "${COLOR_CYAN}--- 정리 완료 ---"
}

check_result() {
    local minishell_status=$1
    local bash_status=$2
    diff_out=$(diff -u $MINISHELL_OUT $BASH_OUT)
    diff_err=$(diff -u $MINISHELL_ERR $BASH_ERR)

    if [ -z "$diff_out" ] && [ -z "$diff_err" ] && [ "$minishell_status" -eq "$bash_status" ]; then
        echo -e "  ${COLOR_GREEN}✅ PASS${COLOR_RESET}\n"
        ((pass_count++))
    else
        echo -e "  ${COLOR_RED}❌ FAIL${COLOR_RESET}"
        if [ "$minishell_status" -ne "$bash_status" ]; then
            echo -e "  ${COLOR_GRAY}--- 종료 코드 다름 ---${COLOR_RESET}"
            echo -e "  Minishell: ${COLOR_RED}${minishell_status}${COLOR_RESET}, Bash (예상): ${COLOR_GREEN}${bash_status}${COLOR_RESET}"
        fi
        if [ -n "$diff_out" ]; then
            echo -e "  ${COLOR_GRAY}--- STDOUT 다름 ---${COLOR_RESET}"
            echo -e "${diff_out}"
        fi
        if [ -n "$diff_err" ]; then
            echo -e "  ${COLOR_GRAY}--- STDERR 다름 ---${COLOR_RESET}"
            echo -e "${diff_err}"
        fi
        echo ""
    fi
}

# 쉘 스크립트 문법 오류를 피하기 위해 명령어를 함수 인자로 전달
run_test() {
    local cmd="$1"
    local description="$2"
    ((test_count++))

    echo -e "${COLOR_YELLOW}테스트 ${test_count}: ${description}${COLOR_RESET}"
    echo -e "  ${COLOR_CYAN}명령어: ${cmd}${COLOR_RESET}"

    # Minishell 실행
    echo "${cmd}" | $MINISHELL_EXEC > $MINISHELL_OUT 2> $MINISHELL_ERR
    minishell_status=$?

    # Bash 실행 (과제 표준에 맞추기 위해 POSIX 모드 사용)
    echo "${cmd}" | $BASH_EXEC --posix > $BASH_OUT 2> $BASH_ERR
    bash_status=$?

    check_result $minishell_status $bash_status
}


# --- 메인 스크립트 ---
setup

# --- 테스트 케이스 (Mandatory Part 기준) ---

print_header "기본 내장 명령어 (Built-ins)"
run_test 'echo hello world' "echo 기본"
run_test 'echo -n hello world' "echo -n 옵션"
run_test 'pwd' "pwd"
run_test 'env' "env (환경 변수 목록)"
run_test 'cd /tmp' "cd (절대 경로, 출력 없음, 상태 변경은 테스트 불가)"
run_test 'cd' "cd (인자 없음, 출력 없음, 상태 변경은 테스트 불가)"


print_header "export 및 unset"
run_test 'export TEST_VAR=123' "export (변수 설정, 출력 없음)"
run_test 'export | grep PWD' "export (인자 없음, 목록 출력)"
run_test 'export TEST_VAR="hello world"' "export (공백 포함 값, 출력 없음)"
run_test 'unset TEST_VAR' "unset (변수 해제, 출력 없음)"
run_test 'export 1TEST=fail' "export (잘못된 식별자)"
run_test 'unset 1TEST' "unset (잘못된 식별자)"

print_header "종료 코드 및 exit"
run_test 'ls > /dev/null' "성공 시 종료 코드 확인"
run_test 'ls non_existent_file' "실패 시 종료 코드 확인"
run_test 'exit 42' "exit (종료 코드 지정)"

print_header "인용 부호 및 환경 변수 확장"
run_test 'echo "$USER"' "큰따옴표 내 변수 확장"
run_test 'echo '$USER'' "작은따옴표 내 변수 확장 안 됨"

run_test 'echo $NON_EXISTENT_VAR' "존재하지 않는 변수 (빈 문자열)"
run_test 'echo ""' "빈 큰따옴표"
run_test "echo ''" "빈 작은따옴표"

print_header "리디렉션 (Redirections)"
run_test 'echo "overwrite" > outfile.txt' "출력 리디렉션 (덮어쓰기)"
run_test 'cat outfile.txt' "리디렉션 결과 확인 1"
run_test 'echo "append" >> outfile.txt' "출력 리디렉션 (추가)"
run_test 'cat outfile.txt' "리디렉션 결과 확인 2"
run_test 'cat < infile.txt' "입력 리디렉션"
run_test '> empty_file.txt' "명령어 없는 출력 (파일 생성)"
run_test 'cat < no_such_file.txt' "존재하지 않는 파일 입력"

print_header "파이프 (Pipes)"
run_test 'ls -l | grep srcs' "기본 파이프"
run_test 'cat infile.txt | sort | uniq' "다중 파이프"
run_test 'env | sort | grep PWD' "내장 명령어와 파이프"

print_header "구문 오류 및 엣지 케이스"
run_test '' "빈 입력"
run_test '   ' "공백만 있는 입력"
run_test 'non_existent_command' "존재하지 않는 명령어"
run_test '/bin/ls' "절대 경로로 명령어 실행"
run_test "ls | | wc" "연속된 파이프 (구문 오류)"
run_test "ls >" "파일명 없는 리디렉션 (구문 오류)"
run_test "< infile.txt" "명령어 없는 입력 리디렉션"
run_test "echo 'unclosed quote" "닫히지 않은 작은따옴표"
run_test 'echo "unclosed quote' "닫히지 않은 큰따옴표"


# --- 최종 결과 출력 ---
echo -e "${COLOR_CYAN}===============================================${COLOR_RESET}"
echo -e "${COLOR_YELLOW}테스트 결과: ${pass_count} / ${test_count} 통과${COLOR_RESET}"
echo -e "${COLOR_CYAN}===============================================${COLOR_RESET}"

cleanup

# 스크립트 종료 코드 설정
if [ "$pass_count" -eq "$test_count" ]; then
    exit 0
else
    exit 1
fi
