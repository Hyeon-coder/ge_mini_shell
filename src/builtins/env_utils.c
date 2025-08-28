// src/builtins/env_utils.c
#include "minishell.h"

// find_var의 임시 구현. 일단 NULL을 반환하여 변수가 없는 것처럼 동작.
char	*find_var(t_ms *ms, char **envp, char *name)
{
    (void)ms;
    (void)envp;
    (void)name;
    return (NULL);
}
