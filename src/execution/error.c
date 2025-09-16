/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:22:09 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/09/16 13:22:25 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	arr_dup_fail(t_ms *ms, char **arr, int j)
{
	(void)arr;
	(void)j;
	ms_error(ms, "Array duplication failed", 1, 0);
}
