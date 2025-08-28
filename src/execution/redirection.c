/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* redirection.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: <your_login> <your_login@student.42.fr>    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 03:00:13 by <your_login>      #+#    #+#             */
/* Updated: 2025/08/29 03:00:15 by <your_login>     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/*
** Handles output redirection by opening the file and duplicating the fd.
** Returns the original stdout fd for later restoration, or -1 on failure.
*/
int	handle_output_redirection(t_cmd *cmd)
{
	int	fd;
	int	original_stdout;

	// For simplicity, we only handle the first output file for now.
	// Bash handles multiple redirections by applying them in order.
	if (cmd->outfile && cmd->outfile[0])
	{
		// O_TRUNC: If the file exists, truncate it. O_CREAT: Create if not exist.
		// 0644: standard file permissions (rw-r--r--).
		fd = open(cmd->outfile[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->outfile[0], 2);
			ft_putstr_fd(": ", 2);
			ft_putendl_fd(strerror(errno), 2);
			return (-1);
		}
		original_stdout = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		return (original_stdout);
	}
	return (STDOUT_FILENO);
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
