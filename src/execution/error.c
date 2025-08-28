// src/execution/error.c
#include "minishell.h"

// ms_error의 임시 구현
void	ms_error(t_ms *ms, char *msg, int ex_code, int free_msg)
{
    (void)free_msg; // unused parameter
    ft_putendl_fd(msg, 2);
    bi_exit(ms, ex_code, 1);
}

void	arr_dup_fail(t_ms *ms, char **arr, int j)
{
    // TODO: 나중에 arr을 free하는 로직을 추가해야 합니다.
    (void)arr;
    (void)j;
    ms_error(ms, "Array duplication failed", 1, 0);
}
