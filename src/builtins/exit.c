/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:20:45 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/24 19:58:26 by JuHyeon          ###   ########.fr       */
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
		if (!error && !ms->child)
			write(2, "exit\n", 5);
		free_structs(ms);
	}
	exit(ex_code);
}

static bool	check_long_long_overflow(const char *str, int sign)
{
	unsigned long long	res;
	unsigned long long	limit;

	res = 0;
	if (sign == 1)
		limit = LLONG_MAX;
	else
		limit = (unsigned long long)LLONG_MAX + 1;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		if (res > limit / 10 || (res == limit / 10
				&& (unsigned long long)(*str - '0') > limit % 10))
			return (false);
		res = res * 10 + (*str - '0');
		str++;
	}
	return (true);
}

static bool	is_valid_numeric_argument(const char *str)
{
	int		i;
	int		sign;
	bool	has_digit;

	i = 0;
	sign = 1;
	has_digit = false;
	while (str[i] == ' ' || (str[i] >= '\t' && str[i] <= '\r'))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		has_digit = true;
		i++;
	}
	if (!has_digit)
		return (false);
	return (check_long_long_overflow(str, sign));
}

void	builtin_exit(t_ms *ms, t_cmd *cmd)
{
	long long	exit_code;

	if (!cmd->full_cmd[1])
		bi_exit(ms, ms->exit_status, 0);
	if (!is_valid_numeric_argument(cmd->full_cmd[1]))
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
	exit_code = ft_atoll(cmd->full_cmd[1]);
	bi_exit(ms, (unsigned char)exit_code, 0);
}
