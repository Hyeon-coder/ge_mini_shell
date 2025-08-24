/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_rt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:30:40 by JuHyeon           #+#    #+#             */
/*   Updated: 2025/08/24 18:17:48 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_rt(char *str)
{
	int	i;

	if (!str)
		return (ft_putstr_rt("(null)"));
	i = 0;
	while (str[i])
		ft_putchar_rt(str[i++]);
	return (i);
}
