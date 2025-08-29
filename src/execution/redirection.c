/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 17:37:13 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 17:37:15 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_outfile(char *filename, int is_append)
{
	int	fd;
	int	flags;

	if (is_append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(filename, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
	}
	return (fd);
}

int	handle_output_redirection(t_cmd *cmd)
{
	int	fd;
	int	original_stdout;
	int	i;

	if (!cmd->outfile || !cmd->outfile[0])
		return (STDOUT_FILENO);
	i = 0;
	fd = -1;
	while (cmd->outfile[i])
	{
		if (fd != -1)
			close(fd);
		fd = open_outfile(cmd->outfile[i], cmd->append[i]);
		if (fd == -1)
			return (-1);
		i++;
	}
	original_stdout = dup(STDOUT_FILENO);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (-1);
	close(fd);
	return (original_stdout);
}

void	restore_output(int original_stdout)
{
	if (original_stdout != STDOUT_FILENO)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
}

int	handle_input_redirection(t_cmd *cmd)
{
	int	fd;
	int	original_stdin;

	if (!cmd->infile || !cmd->infile[0])
		return (STDIN_FILENO);
	fd = process_infile_loop(cmd);
	if (fd == -1)
		return (-1);
	original_stdin = dup(STDIN_FILENO);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (original_stdin);
}

void	restore_input(int original_stdin)
{
	if (original_stdin != STDIN_FILENO)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
}
