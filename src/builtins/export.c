/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:15:15 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/01 03:49:07 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Sorts a duplicated array of environment variables alphabetically.
*/
static void	sort_env_array(char **env_copy, int count)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env_copy[j], env_copy[j + 1]) > 0)
			{
				tmp = env_copy[j];
				env_copy[j] = env_copy[j + 1];
				env_copy[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

/*
** Prints all environment variables in the 'declare -x' format, sorted.
*/
// 이 함수 전체를 아래 코드로 교체하세요.

static void	print_exported_vars(t_ms *ms)
{
	char	**env_copy;
	int		i;
	char	*eq_sign;

	// 환경 변수 배열을 복사합니다.
	env_copy = malloc(sizeof(char *) * (ms->elements + 1));
	if (!env_copy)
		return ;
	i = 0;
	while (i < ms->elements)
	{
		env_copy[i] = ms->envp[i]; // 포인터만 복사
		i++;
	}
	env_copy[i] = NULL;
	
	// 복사된 배열을 정렬합니다.
	sort_env_array(env_copy, ms->elements);

	i = 0;
	while (env_copy[i])
	{
		eq_sign = ft_strchr(env_copy[i], '=');
		ft_putstr_fd("declare -x ", 1);
		if (eq_sign)
		{
			// KEY="VALUE" 형식으로 출력
			write(1, env_copy[i], eq_sign - env_copy[i] + 1);
			ft_putchar_fd('"', 1);
			ft_putstr_fd(eq_sign + 1, 1);
			ft_putendl_fd("\"", 1);
		}
		else
			ft_putendl_fd(env_copy[i], 1);
		i++;
	}
	free(env_copy);
}

/*
** Checks if the key for an environment variable is a valid identifier.
** Must start with a letter or '_', followed by letters, digits, or '_'.
*/
static bool	is_valid_identifier(const char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (false);
	i++;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

/*
** Parses a single argument to 'export', validates it, and sets the variable.
*/
static void	process_export_argument(t_ms *ms, const char *arg)
{
	char	*key;
	char	*value;
	int		key_len;

	key_len = 0;
	while (arg[key_len] && arg[key_len] != '=')
		key_len++;
	key = ft_substr(arg, 0, key_len);
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd((char *)arg, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		ms->exit_status = 1;
	}
	else if (arg[key_len] == '=')
	{
		value = (char *)arg + key_len + 1;
		set_env_var(ms, key, value);
	}
	free(key);
}

/*
** Executes the 'export' builtin command.
*/
void	builtin_export(t_ms *ms, t_cmd *cmd)
{
	int	i;

	if (!cmd->full_cmd[1])
	{
		print_exported_vars(ms);
		return ;
	}
	i = 1;
	while (cmd->full_cmd[i])
	{
		process_export_argument(ms, cmd->full_cmd[i]);
		i++;
	}
}
