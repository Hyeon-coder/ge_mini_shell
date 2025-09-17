/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:22:09 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/18 00:16:25 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Handles a fatal error by printing a message and exiting.
void	ms_error(t_ms *ms, char *msg, int ex_code, int free_msg)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	else
		ft_putstr_fd("An unspecified error occurred.\n", 2);
	if (free_msg == 1)
		free(msg);
	bi_exit(ms, ex_code, 1);
}

// Handles errors during array duplication, leading to program termination.
void	arr_dup_fail(t_ms *ms, char **arr, int j)
{
	(void)arr;
	(void)j;
	ms_error(ms, "Array duplication failed", 1, 0);
}
