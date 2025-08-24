/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <juhyeonl@student.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:16:18 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/02/28 15:27:01 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
	Adds an element to the front of a linked list. (연결 리스트의 앞에 요소 추가)
*/
void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}
/*
#include <stdio.h>

void	print_list(t_list *lst)
{
	while (lst)
	{
		printf("%p -> ", (void *)lst);
		printf("%d\n", *(int *)(lst->content));
		lst = lst->next;
	}
	printf("NULL\n");
}

int	main(void)
{
	int value1 = 42;
	int value2 = 99;

	t_list *lst = NULL;

	t_list *node1 = ft_lstnew(&value1);
	t_list *node2 = ft_lstnew(&value2);

	ft_lstadd_front(&lst, node2);
	ft_lstadd_front(&lst, node1);

	printf("Linked List after adding elements to the front:\n");
	print_list(lst);

	free(node1);
	free(node2);

	return (0);
}
*/