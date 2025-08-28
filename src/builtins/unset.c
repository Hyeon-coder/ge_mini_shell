/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 02:08:34 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 02:08:35 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Finds the index of a variable in the environment list.
*/
static int	find_var_index(char **envp, const char *key)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0
			&& (envp[i][len] == '=' || envp[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

/*
** Removes a single environment variable at a given index by reallocating
** the envp array.
*/
static void	remove_env_var(t_ms *ms, int idx_to_remove)
{
	char	**new_envp;
	int		i;
	int		j;

	new_envp = malloc(sizeof(char *) * ms->elements);
	if (!new_envp)
		return ; // TODO: Proper error handling
	i = 0;
	j = 0;
	while (i < ms->elements)
	{
		if (i != idx_to_remove)
		{
			new_envp[j] = ms->envp[i];
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	free(ms->envp[idx_to_remove]);
	free(ms->envp);
	ms->envp = new_envp;
	ms->elements--;
}

/*
** Executes the 'unset' builtin command.
** Loops through arguments and removes each specified environment variable.
*/
void	builtin_unset(t_ms *ms, t_cmd *cmd)
{
	int	i;
	int	target_idx;

	i = 1;
	while (cmd->full_cmd[i])
	{
		// TODO: Add validation for the key format.
		target_idx = find_var_index(ms->envp, cmd->full_cmd[i]);
		if (target_idx != -1)
			remove_env_var(ms, target_idx);
		i++;
	}
}
