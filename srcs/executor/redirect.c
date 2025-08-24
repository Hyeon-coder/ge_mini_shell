/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* redirect.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 17:00:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redir_in(t_redir *redir)
{
	int	fd;

	fd = open(redir->filename, O_RDONLY);
	if (fd == -1)
	{
		print_error(redir->filename, strerror(errno), 1);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		print_error("dup2", strerror(errno), 1);
		return (-1);
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
		print_error(redir->filename, strerror(errno), 1);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		print_error("dup2", strerror(errno), 1);
		return (-1);
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

int	apply_redirections_for_empty(t_redir *redirs)
{
	t_redir	*current;
	int		fd;
	int		status;

	current = redirs;
	status = 0;
	while (current)
	{
		if (current->type == TOKEN_REDIR_IN)
			fd = open(current->filename, O_RDONLY, 0);
		else if (current->type == TOKEN_REDIR_APPEND)
			fd = open(current->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(current->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			print_error(current->filename, strerror(errno), 1);
			status = 1;
			break ;
		}
		else
			close(fd);
		current = current->next;
	}
	return (status);
}
