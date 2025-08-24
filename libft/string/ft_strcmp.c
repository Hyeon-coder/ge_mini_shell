/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljh3900 <ljh3900@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 13:11:51 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/06/08  ##:##:## by ljh3900          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

int ft_strcmp(const char *s1, const char *s2)
{
	unsigned char uc1;
	unsigned char uc2;

	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	uc1 = (unsigned char)*s1;
	uc2 = (unsigned char)*s2;
	return (uc1 - uc2);
}
