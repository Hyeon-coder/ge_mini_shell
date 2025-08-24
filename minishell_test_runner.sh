#!/bin/bash

# --- 설정 ---
MINISHELL_EXEC="./minishell"
BASH_EXEC="/bin/bash"

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

# --- 테스트 환경 설정 함수 ---
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
    echo -e "${COLOR_CYAN}--- 설정 완료 ---\n${COLOR_RESET}"
}

# --- 테스트 환경 정리 함수 ---
cleanup() {
    echo -e "\n${COLOR_CYAN}--- 테스트 환경 정리 중... ---${COLOR_RESET}"
    rm -f $MINISHELL_OUT $MINISHELL_ERR $BASH_OUT $BASH_ERR
    rm -f infile.txt outfile.txt append_file.txt empty_file.txt
    echo -e "${COLOR_CYAN}--- 정리 완료 ---${COLOR_RESET}"
}

# --- 개별 테스트 실행 함수 ---
run_test() {
    local cmd="$1"
    local description="$2"
    ((test_count++))

    echo -e "${COLOR_YELLOW}테스트 ${test_count}: ${description}${COLOR_RESET}"
    echo -e "  ${COLOR_CYAN}명령어: ${cmd}${COLOR_RESET}"

    # Minishell 실행
    echo "${cmd}" | $MINISHELL_EXEC > $MINISHELL_OUT 2> $MINISHELL_ERR
    minishell_status=$?

    # Bash 실행
    echo "${cmd}" | $BASH_EXEC > $BASH_OUT 2> $BASH_ERR
    bash_status=$?

    # 결과 비교
    diff_out=$(diff $MINISHELL_OUT $BASH_OUT)
    diff_err=$(diff $MINISHELL_ERR $BASH_ERR)

    if [ "$diff_out" == "" ] && [ "$diff_err" == "" ] && [ "$minishell_status" -eq "$bash_status" ]; then
        echo -e "  ${COLOR_GREEN}✅ PASS${COLOR_RESET}\n"
        ((pass_count++))
    else
        echo -e "  ${COLOR_RED}❌ FAIL${COLOR_RESET}"
        if [ "$minishell_status" -ne "$bash_status" ]; then
            echo -e "  ${COLOR_GRAY}--- 종료 코드 다름 ---${COLOR_RESET}"
            echo -e "  Minishell: ${COLOR_RED}${minishell_status}${COLOR_RESET}, Bash (예상): ${COLOR_GREEN}${bash_status}${COLOR_RESET}"
        fi
        if [ "$diff_out" != "" ]; then
            echo -e "  ${COLOR_GRAY}--- STDOUT 다름 ---${COLOR_RESET}"
            echo -e "  ${COLOR_RED}Minishell 출력:${COLOR_RESET}\n$(cat $MINISHELL_OUT)"
            echo -e "  ${COLOR_GREEN}Bash (예상) 출력:${COLOR_RESET}\n$(cat $BASH_OUT)"
        fi
        if [ "$diff_err" != "" ]; then
            echo -e "  ${COLOR_GRAY}--- STDERR 다름 ---${COLOR_RESET}"
            echo -e "  ${COLOR_RED}Minishell 에러:${COLOR_RESET}\n$(cat $MINISHELL_ERR)"
            echo -e "  ${COLOR_GREEN}Bash (예상) 에러:${COLOR_RESET}\n$(cat $BASH_ERR)"
        fi
        echo ""
    fi
}

# --- 메인 스크립트 ---
setup

# --- 테스트 케이스 목록 ---
echo -e "${COLOR_CYAN}========== 기본 명령어 및 내장 명령어 테스트 ==========${COLOR_RESET}"
run_test 'ls -l' "기본 외부 명령어"
run_test 'echo hello world' "echo 기본"
run_test 'echo -n hello' "echo -n 옵션"
run_test 'pwd' "pwd 내장 명령어"
run_test 'env' "env 내장 명령어"
run_test 'export' "export (인자 없음)"
run_test 'export VAR=123' "export (인자 있음)"
run_test 'unset VAR' "unset"
run_test 'exit 42' "exit (종료 코드 지정)"

echo -e "${COLOR_CYAN}========== 인용 부호 및 환경 변수 테스트 ==========${COLOR_RESET}"
run_test 'echo "hello $USER"' "큰따옴표 내 변수 확장"
run_test "echo 'hello \$USER'" "작은따옴표 내 변수 확장 안 됨"
run_test 'echo $NON_EXISTENT_VAR' "존재하지 않는 변수 확장 (빈 문자열)"
run_test 'echo "Status: $?"' "종료 코드 확장"
run_test "echo 'unclosed" "닫히지 않은 작은따옴표 (구문 오류)"
run_test "echo \"unclosed" "닫히지 않은 큰따옴표 (구문 오류)"

echo -e "${COLOR_CYAN}========== 리디렉션 테스트 ==========${COLOR_RESET}"
run_test 'ls > outfile.txt' "출력 리디렉션 (덮어쓰기)"
run_test 'echo hello >> append_file.txt' "출력 리디렉션 (추가)"
run_test 'cat < infile.txt' "입력 리디렉션"
run_test '> empty_file.txt' "명령어 없는 출력 리디렉션 (파일 생성)"
run_test 'cat < infile.txt > outfile.txt' "입력과 출력 동시 리디렉션"
run_test 'cat < no_such_file.txt' "존재하지 않는 파일 입력 리디렉션"

echo -e "${COLOR_CYAN}========== 파이프 테스트 ==========${COLOR_RESET}"
run_test 'ls -l | grep srcs' "기본 파이프"
run_test 'cat infile.txt | sort | uniq' "여러 개 파이프"
run_test 'env | sort' "내장 명령어와 파이프"

echo -e "${COLOR_CYAN}========== 엣지 케이스 테스트 ==========${COLOR_RESET}"
run_test '' "빈 입력"
run_test '   ' "공백만 있는 입력"
run_test 'non_existent_command' "존재하지 않는 명령어"
run_test 'ls | | wc' "연속된 파이프 (구문 오류)"
run_test 'ls >' "파일 없는 리디렉션 (구문 오류)"

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

