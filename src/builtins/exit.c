/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* exit.c                                             :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 14:12:20 by JuHyeon           #+#    #+#             */
/* Updated: 2025/09/13 08:05:00 by JuHyeon          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h> // LLONG_MAX와 LLONG_MIN을 위해 추가

/*
** Prints "exit" to stderr and terminates the shell after freeing memory.
*/
void	bi_exit(t_ms *ms, int ex_code, int error)
{
	// (이 함수는 이전 단계에서 수정했으므로 그대로 둡니다)
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
		if (!error && isatty(fileno(stdout)))
			write(2, "exit\n", 5);
		free_structs(ms);
	}
	exit(ex_code);
}

/**
 * @brief 문자열을 long long으로 변환하며 오버플로우를 확인하는 함수
 *
 * @param str 확인할 숫자 문자열
 * @param sign 부호 (1 또는 -1)
 * @return 오버플로우가 발생하면 false, 아니면 true
 */
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
		// 오버플로우 확인: res > (limit - (*str - '0')) / 10
		if (res > limit / 10 || (res == limit / 10 && (unsigned long long)(*str - '0') > limit % 10))
			return (false); // 오버플로우 발생
		res = res * 10 + (*str - '0');
		str++;
	}
	return (true);
}

/**
 * @brief 주어진 문자열이 유효한 숫자 형식이며 long long 범위를 넘지 않는지 확인합니다.
 * @param str 검사할 문자열
 * @return 유효하면 true, 아니면 false
 */
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
			return (false); // 숫자가 아닌 문자가 있으면 실패
		has_digit = true;
		i++;
	}
	if (!has_digit) // 숫자 부분이 전혀 없으면 실패
		return (false);
	// 오버플로우 체크는 부호와 숫자 부분만 넘겨서 수행
	return (check_long_long_overflow(str, sign));
}

/**
 * @brief 'exit' 빌트인 명령어를 실행합니다.
 * long long 오버플로우를 포함한 인자 유효성 검사를 수행합니다.
 */
void	builtin_exit(t_ms *ms, t_cmd *cmd)
{
	long long	exit_code;

	if (!cmd->full_cmd[1])
	{
		bi_exit(ms, ms->exit_status, 0);
	}
	if (!is_valid_numeric_argument(cmd->full_cmd[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->full_cmd[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		bi_exit(ms, 2, 0); // bash는 이 경우 종료 코드 2를 사용
	}
	if (cmd->full_cmd[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		ms->exit_status = 1;
		return ;
	}
	// 유효성 검사를 통과했으므로 ft_atoll을 사용하여 변환
	exit_code = ft_atoll(cmd->full_cmd[1]);
	// 셸 종료 코드는 0-255 범위이므로 unsigned char로 변환
	bi_exit(ms, (unsigned char)exit_code, 0);
}
