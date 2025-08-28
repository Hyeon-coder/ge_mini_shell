
#include "minishell.h"

void	run_executor(t_ms *ms, int i)
{
	(void)i; // 현재 사용하지 않는 인자에 대한 컴파일러 경고 방지

	if (!ms || !ms->ast)
		return ;

	// TODO: 여기에 AST를 해석하고 명령어를 실행하는 코드를 추가할 예정입니다.
	printf("--- Executor is running! ---\n");
	printf("Command: %s\n", ms->ast->cmd->full_cmd[0]);
	printf("--------------------------\n");
}
