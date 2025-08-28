/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 00:45:06 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/29 00:45:08 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Finds a variable in the environment list (envp).
** It compares 'name' with the part of the env string before the '='.
** Returns a pointer to the value (the part after '=') if found,
** otherwise returns NULL.
*/
char	*find_var(t_ms *ms, char **envp, char *name)
{
	int		i;
	size_t	len;

	(void)ms;
	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0)
		{
			if (envp[i][len] == '=')
				return (envp[i] + len + 1);
		}
		i++;
	}
	return (NULL);
}
