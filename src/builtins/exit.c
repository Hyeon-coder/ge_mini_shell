
#include "minishell.h"

/**
 * @brief minishell을 종료하는 함수.
 * * @param ms minishell 구조체 (나중에 메모리 해제를 위해 사용)
 * @param ex_code 종료 코드
 * @param error 에러 발생 여부 (현재는 사용 안함)
 */
void	bi_exit(t_ms *ms, int ex_code, int error)
{
	(void)ms;
	(void)error;

	// TODO: 여기에 free_ms(ms)와 같이 모든 할당된 메모리를 해제하는 로직을 추가해야 합니다.
	exit(ex_code);
}
