/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:16:33 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/09/09 22:57:22 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ms_error(t_ms *ms, char *msg, int ex_code, int free_msg)
{
	if (msg)
		ft_putendl_fd(msg, 2);
	if (free_msg)
		free(msg);
	bi_exit(ms, ex_code, 1);
}

void	arr_dup_fail(t_ms *ms, char **arr, int j)
{
	while (j--)
		free(arr[j]);
	free(arr);
	ms_error(ms, "Array duplication failed", 1, 0);
}
