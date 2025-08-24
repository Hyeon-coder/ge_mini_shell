/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:46:36 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/02/28 15:54:09 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Creates a new linked list node. (새로운 연결 리스트 노드 생성)
*/
t_list	*ft_lstnew(void *content)
{
	t_list	*n_node;

	n_node = (t_list *)malloc(sizeof(*n_node));
	if (!n_node)
		return (NULL);
	n_node->content = content;
	n_node->next = NULL;
	return (n_node);
}
