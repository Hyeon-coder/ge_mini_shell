/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* expander_utils.c                                   :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 12:30:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_variable_expansion(char *str, int *i,
	char **new_str, t_shell *shell)
{
	int		start;
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (str[*i] == '?')
	{
		var_value = ft_itoa(shell->last_exit_status);
		*new_str = ft_strjoin_free(*new_str, var_value);
		free(var_value);
		(*i)++;
		return ;
	}
	start = *i;
	while (ft_isalnum(str[*i]) || str[*i] == '_')
		(*i)++;
	if (*i == start)
	{
		*new_str = ft_strjoin_free(*new_str, "$");
		return ;
	}
	var_name = ft_substr(str, start, *i - start);
	var_value = get_env_value(shell->env_list, var_name);
	if (var_value)
		*new_str = ft_strjoin_free(*new_str, var_value);
	free(var_name);
}

static void	process_quotes(char *str, int *i, char **new_str, t_shell *shell)
{
	char	quote_char;
	int		start;

	quote_char = str[(*i)++];
	start = *i;
	while (str[*i] && str[*i] != quote_char)
		(*i)++;
	if (quote_char == '\'')
		append_substr(new_str, str, start, *i);
	else
	{
		while (start < *i)
		{
			if (str[start] == '$')
				handle_variable_expansion(str, &start, new_str, shell);
			else
			{
				append_substr(new_str, str, start, start + 1);
				start++;
			}
		}
	}
	if (str[*i])
		(*i)++;
}

char	*expand_string(char *str, t_shell *shell)
{
	char	*new_str;
	int		i;
	int		start;

	new_str = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			process_quotes(str, &i, &new_str, shell);
		else if (str[i] == '$')
			handle_variable_expansion(str, &i, &new_str, shell);
		else
		{
			start = i;
			while (str[i] && !ft_strchr("'\"$", str[i]))
				i++;
			append_substr(&new_str, str, start, i);
		}
	}
	return (new_str);
}
