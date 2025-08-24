/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* lexer_utils.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:50:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	// 디버그 출력 추가
	printf("[DEBUG] Malloc Token: %p (Value: %s)\n", (void *)token, value);
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*current;

	if (!new)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	printf("[DEBUG] --- Entering free_tokens ---\n");
	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
		{
			printf("[DEBUG] Cleanup Freeing Token->value: %p (%s)\n", (void *)tokens->value, tokens->value);
			free(tokens->value);
		}
		else
		{
			printf("[DEBUG] Cleanup Skipping NULL Token->value in Token: %p\n", (void *)tokens);
		}
		printf("[DEBUG] Cleanup Freeing Token: %p\n", (void *)tokens);
		free(tokens);
		tokens = tmp;
	}
	printf("[DEBUG] --- Exiting free_tokens ---\n");
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
