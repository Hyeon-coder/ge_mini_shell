/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 12:15:51 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/25 12:15:56 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Checks if quotes in the command line are properly closed.
*/
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
				print_error("unexpected EOF while looking for matching `", \
					&quote, 2);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

/*
** Handles special character tokens like pipes and redirections.
*/
static int	handle_special_tokens(char *line, int *i, t_token **head)
{
	char	*val;
	int		len;

	len = 0;
	if (line[*i] == '|')
		len = 1;
	else if (line[*i] == '<' && line[*i + 1] == '<')
		len = 2;
	else if (line[*i] == '<')
		len = 1;
	else if (line[*i] == '>' && line[*i + 1] == '>')
		len = 2;
	else if (line[*i] == '>')
		len = 1;
	if (len > 0)
	{
		val = ft_substr(line, *i, len);
		if (len == 1 && val[0] == '|')
			add_token(head, new_token(val, TOKEN_PIPE));
		else if (len == 2 && val[0] == '<')
			add_token(head, new_token(val, TOKEN_HEREDOC));
		else if (len == 1 && val[0] == '<')
			add_token(head, new_token(val, TOKEN_REDIR_IN));
		else if (len == 2 && val[0] == '>')
			add_token(head, new_token(val, TOKEN_REDIR_APPEND));
		else if (len == 1 && val[0] == '>')
			add_token(head, new_token(val, TOKEN_REDIR_OUT));
		*i += len;
		return (1);
	}
	return (0);
}

/*
** Calculates the length of a word, handling quotes correctly.
*/
static int	get_word_len(char *line, int i)
{
	int		len;
	char	quote;

	len = 0;
	while (line[i + len] && !ft_isspace(line[i + len]) && \
			!is_special_char(line[i + len]))
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

/*
** The main lexer function that tokenizes the input line.
*/
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
		if (!handle_special_tokens(line, &i, &head))
		{
			len = get_word_len(line, i);
			add_token(&head, new_token(ft_substr(line, i, len), TOKEN_WORD));
			i += len;
		}
	}
	add_token(&head, new_token(NULL, TOKEN_EOF));
	return (head);
}
