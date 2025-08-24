/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/*   Updated: 2025/08/24 15:29:26 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h> // perror

// 에러 출력 표준화 (테스트 21 해결)
static int print_redir_error(char *context)
{
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    // perror는 자동으로 context: system_error_message를 출력합니다.
    perror(context);
    return (-1);
}

static int	handle_redir_in(t_redir *redir)
{
	int	fd;

    // TODO: Heredoc (<<) 구현 필요
    if (redir->type == TOKEN_HEREDOC)
    {
        ft_putendl_fd("minishell: Heredoc not implemented", STDERR_FILENO);
        return (-1);
    }

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		return (print_redir_error(redir->filename));
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (print_redir_error("dup2"));
	}
	close(fd);
	return (0);
}

static int	handle_redir_out(t_redir *redir)
{
	int	fd;
	int	flags;

	if (redir->type == TOKEN_REDIR_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(redir->filename, flags, 0644);
	if (fd == -1)
	{
		return (print_redir_error(redir->filename));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (print_redir_error("dup2"));
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
		{
			if (handle_redir_in(current) == -1)
				return (-1);
		}
		else if (current->type == TOKEN_REDIR_OUT
			|| current->type == TOKEN_REDIR_APPEND)
		{
			if (handle_redir_out(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}

// 명령어가 없을 때 파일 생성/유효성 검사 (테스트 19)
int	apply_redirections_for_empty(t_redir *redirs)
{
	t_redir	*current;
	int		fd;

	current = redirs;
	while (current)
	{
        if (current->type == TOKEN_HEREDOC)
        {
             ft_putendl_fd("minishell: Heredoc not implemented", STDERR_FILENO);
             return (1);
        }
		else if (current->type == TOKEN_REDIR_IN)
			fd = open(current->filename, O_RDONLY, 0);
		else if (current->type == TOKEN_REDIR_APPEND)
			fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else // TOKEN_REDIR_OUT
			fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

		if (fd == -1)
		{
			print_redir_error(current->filename);
            // Bash는 첫 번째 오류에서 즉시 중단하고 1을 반환합니다.
			return (1);
		}
		else
			close(fd);
		current = current->next;
	}
	return (0);
}
