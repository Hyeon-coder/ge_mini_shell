/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* cd.c                                               :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 02:05:44 by JuHyeon           #+#    #+#             */
/* Updated: 2025/09/03 17:00:00 by JuHyeon          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

// 함수 프로토타입 선언 (cd_utils.c 에 있는 함수들)
char	*get_target_path(t_ms *ms, t_cmd *cmd);
void	update_pwd_variables(t_ms *ms);

/*
** chdir() 호출 실패 시 에러 메시지를 출력하는 함수
*/
static void	handle_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	ft_putendl_fd(strerror(errno), 2);
}

/*
** Executes the 'cd' builtin command.
*/
void	builtin_cd(t_ms *ms, t_cmd *cmd)
{
	char	*path;
	bool	is_oldpwd;

	is_oldpwd = (cmd->full_cmd[1] && ft_strcmp(cmd->full_cmd[1], "-") == 0);
	path = get_target_path(ms, cmd);
	if (!path)
	{
		ms->exit_status = 1;
		return ;
	}
	if (chdir(path) != 0)
	{
		handle_cd_error(path);
		ms->exit_status = 1;
	}
	else
	{
		update_pwd_variables(ms);
		if (is_oldpwd)
			builtin_pwd(ms);
		ms->exit_status = 0;
	}
	free(path);
}
