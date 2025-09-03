/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 00:45:06 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/03 14:50:05 by JuHyeon          ###   ########.fr       */
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

/*
** Sets or updates an environment variable.
** If the key exists, its value is updated.
** If the key does not exist, a new variable is added to the environment.
*/
// void	set_env_var(t_ms *ms, char *key, char *value)
// {
// 	int		i;
// 	size_t	len;
// 	char	*new_var;
// 	char	*tmp;

// 	i = -1;
// 	len = ft_strlen(key);
// 	tmp = ft_strjoin(key, "=");
// 	new_var = ft_strjoin(tmp, value);
// 	free(tmp);
// 	while (ms->envp[++i])
// 	{
// 		if (ft_strncmp(ms->envp[i], key, len) == 0 && ms->envp[i][len] == '=')
// 		{
// 			free(ms->envp[i]);
// 			ms->envp[i] = new_var;
// 			return ;
// 		}
// 	}
// 	ft_add_to_array(&ms->envp, new_var);
// 	ms->elements++;
// 	free(new_var);
// }

void set_env_var(t_ms *ms, char *key, char *value)
{
    int i;
    size_t len;
    char *new_var;
    char *tmp;

    i = -1;
    len = ft_strlen(key);
    if (value)
    {
        tmp = ft_strjoin(key, "=");
        new_var = ft_strjoin(tmp, value);
        free(tmp);
    }
    else
        new_var = ft_strdup(key);

    while (ms->envp[++i])
    {
        if (ft_strncmp(ms->envp[i], key, len) == 0 && (ms->envp[i][len] == '=' || ms->envp[i][len] == '\0'))
        {
            free(ms->envp[i]);
            ms->envp[i] = new_var;
            return;
        }
    }
    ft_add_to_array(&ms->envp, new_var);
    ms->elements++;
    free(new_var);
}
