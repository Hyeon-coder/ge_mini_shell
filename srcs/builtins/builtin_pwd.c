/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* builtin_pwd.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 11:27:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		print_error("pwd", strerror(errno), 1);
		return (1);
	}
	ft_putendl_fd(cwd, 1);
	free(cwd);
	return (0);
}
