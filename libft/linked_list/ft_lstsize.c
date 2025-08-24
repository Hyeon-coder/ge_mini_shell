/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:45:00 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:26:46 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Gets the size of a linked list. (연결 리스트의 크기 반환)
*/
int	ft_lstsize(t_list *lst)
{
	int			cnt;
	t_list		*tmp;

	tmp = (t_list *)lst;
	cnt = 0;
	while (tmp)
	{
		cnt++;
		tmp = tmp->next;
	}
	return (cnt);
}
