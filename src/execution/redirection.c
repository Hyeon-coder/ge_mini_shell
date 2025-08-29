/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* redirection.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: <your_login> <your_login@student.42.fr>    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 03:00:13 by <your_login>      #+#    #+#             */
/* Updated: 2025/08/29 04:15:15 by <your_login>     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/*
** Opens a single output file with the correct flags (O_TRUNC or O_APPEND).
** Returns the file descriptor, or -1 on failure.
*/
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

/*
** Handles output redirection by iterating through output files.
** The final output file's fd is duplicated to stdout.
** Returns the original stdout fd for later restoration, or -1 on failure.
*/
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

/*
** Restores the original standard output after a command has executed.
*/
void	restore_output(int original_stdout)
{
	if (original_stdout != STDOUT_FILENO)
	{
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdout);
	}
}

/*
** Handles input redirection ('<') by opening the file and duplicating the fd.
** Returns the original stdin fd for later restoration, or -1 on failure.
*/
int	handle_input_redirection(t_cmd *cmd)
{
	int	fd;
	int	original_stdin;

	if (cmd->infile && cmd->infile[0] && cmd->infile[0]->heredoc == 0)
	{
		fd = open(cmd->infile[0]->name, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->infile[0]->name, 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			return (-1);
		}
		original_stdin = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		close(fd);
		return (original_stdin);
	}
	return (STDIN_FILENO);
}

/*
** Restores the original standard input after a command has executed.
*/
void	restore_input(int original_stdin)
{
	if (original_stdin != STDIN_FILENO)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
	}
}
