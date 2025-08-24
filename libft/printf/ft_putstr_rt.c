/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_rt.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <ljh3900@gmail.com>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 13:30:40 by JuHyeon           #+#    #+#             */
/*   Updated: 2024/11/19 13:31:40 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

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
