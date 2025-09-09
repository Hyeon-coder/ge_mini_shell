/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:12:20 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/09 23:16:32 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Prints "exit" to stderr and terminates the shell after freeing memory.
*/
void	bi_exit(t_ms *ms, int ex_code, int error)
{
	rl_clear_history();
	if (ms)
	{
		if (ms->prompt)
		{
			free(ms->prompt);
			ms->prompt = NULL;
		}
		close_fd(ms);
		free_envp(ms);
		if (ms->pids)
		{
			free(ms->pids);
			ms->pids = NULL;
		}
		if (!error)
			write(2, "exit\n", 5);
		free_structs(ms);
	}
	exit(ex_code);
}

/*
** Checks if a string represents a valid number (long long).
*/
static bool	is_numeric_str(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

/*
** Executes the 'exit' builtin command.
** Handles exit codes and argument validation.
*/
void	builtin_exit(t_ms *ms, t_cmd *cmd)
{
	int	exit_code;

	exit_code = 0;
	if (!cmd->full_cmd[1])
		bi_exit(ms, ms->exit_status, 0);
	if (!is_numeric_str(cmd->full_cmd[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->full_cmd[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		bi_exit(ms, 2, 0);
	}
	if (cmd->full_cmd[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		ms->exit_status = 1;
		return ;
	}
	exit_code = ft_atoi(cmd->full_cmd[1]);
	bi_exit(ms, exit_code, 0);
}
