// src/execution/heredoc.c
#include "minishell.h"

// start_heredoc의 임시 구현
// 일단은 아무것도 하지 않고 성공(0)을 반환합니다.
int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
    (void)ms;
    (void)lim;
    (void)infile;
    (void)quo;
    ft_putendl_fd("heredoc is not implemented yet", 2);
    return (0);
}
