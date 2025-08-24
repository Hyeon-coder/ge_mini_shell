/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:23:12 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:53:05 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Iterates over a linked list and applies a function to each node.
	(연결 리스트를 순회하며 각 노드에 함수를 적용)
*/
void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*tmp;

	tmp = (t_list *)lst;
	if (tmp && f)
	{
		while (tmp)
		{
			f(tmp->content);
			tmp = tmp->next;
		}
	}
}
