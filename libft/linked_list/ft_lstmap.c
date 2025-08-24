/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:50:05 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:53:55 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Creates a new linked list by applying a function to each node of 
	an existing list. (연결 리스트의 각 노드에 함수를 적용하여 새로운 리스트 생성)
*/
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*node;

	if (!lst)
		return (NULL);
	newlst = NULL;
	node = NULL;
	while (lst)
	{
		if (!f)
			node = ft_lstnew(lst->content);
		else
			node = ft_lstnew(f(lst->content));
		if (!node)
		{
			ft_lstclear(&newlst, del);
			return (NULL);
		}
		ft_lstadd_back(&newlst, node);
		lst = lst->next;
	}
	return (newlst);
}
