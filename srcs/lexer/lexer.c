/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* lexer.c                                            :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 17:00:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static int	is_quote_closed(char *line)
{
	char	quote;
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			quote = line[i];
			i++;
			while (line[i] && line[i] != quote)
				i++;
			if (line[i] == '\0')
			{
				ft_putstr_fd("minishell: unexpected EOF while looking for matching `", 2);
				ft_putchar_fd(quote, 2);
				ft_putstr_fd("'\n", 2);
				g_exit_status = 2;
				return (0);
			}
		}
		i++;
	}
	return (1);
}

static int	handle_special_tokens(char *line, int *i, t_token **head)
{
	if (line[*i] == '|')
		add_token(head, new_token(ft_strdup("|"), TOKEN_PIPE));
	else if (line[*i] == '<' && line[*i + 1] == '<')
	{
		add_token(head, new_token(ft_strdup("<<"), TOKEN_HEREDOC));
		(*i)++;
	}
	else if (line[*i] == '<')
		add_token(head, new_token(ft_strdup("<"), TOKEN_REDIR_IN));
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		add_token(head, new_token(ft_strdup(">>"), TOKEN_REDIR_APPEND));
		(*i)++;
	}
	else if (line[*i] == '>')
		add_token(head, new_token(ft_strdup(">"), TOKEN_REDIR_OUT));
	else
		return (0);
	return (1);
}

static int	get_word_len(char *line, int i)
{
	int		len;
	char	quote;

	len = 0;
	while (line[i + len] && !ft_isspace(line[i + len])
		&& !is_special_char(line[i + len]))
	{
		if (line[i + len] == '\'' || line[i + len] == '\"')
		{
			quote = line[i + len];
			len++;
			while (line[i + len] && line[i + len] != quote)
				len++;
			if (line[i + len])
				len++;
		}
		else
			len++;
	}
	return (len);
}

t_token	*lexer(char *line)
{
	t_token	*head;
	int		i;
	int		len;

	if (!is_quote_closed(line))
		return (NULL);
	i = 0;
	head = NULL;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (line[i] == '\0')
			break ;
		if (handle_special_tokens(line, &i, &head))
			i++;
		else
		{
			len = get_word_len(line, i);
			add_token(&head, new_token(ft_substr(line, i, len), TOKEN_WORD));
			i += len;
		}
	}
	add_token(&head, new_token(NULL, TOKEN_EOF));
	return (head);
}
