/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:07:38 by mpierce           #+#    #+#             */
/*   Updated: 2025/09/09 22:02:19 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** Prepare heredoc information for the command.
*/
bool	setup_heredoc(t_ms *ms, t_token *token, t_infile **infile)
{
	int	quoted;

	quoted = token->next->quoted;
	ms->heredoc_no++;
	*infile = new_infile("", 1);
	if (start_heredoc(ms, token->next->value, *infile, quoted))
	{
		if (*infile) 
        {
            if ((*infile)->name) 
                free((*infile)->name);
            free(*infile);
            *infile = NULL;
        }
		return (false);	// TMP
	}
	return (true);
}

/*
** Check if the heredoc limit is reached. Max 16 heredocs allowed.
*/
bool	check_heredoc_limit(t_ms *ms)
{
	t_token	*current;
	int		heredoc_count;

	current = ms->token;
	heredoc_count = 0;
	while (current)
	{
		if (current->type == TOKEN_HEREDOC)
			heredoc_count++;
		if (heredoc_count > 16)
		{
			ft_putstr_fd(HEREDOC_LIM, STDERR_FILENO);
			return (false);
		}
		current = current->next;
	}
	return (true);
}
