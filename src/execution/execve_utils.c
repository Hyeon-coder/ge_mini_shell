/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* execve_utils.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13 07:25:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13 07:25:00 by hyeon-coder     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief PATH 환경변수를 ':' 기준으로 분리하고 각 경로에 cmd를 붙여서
 * 실행 가능한 파일인지 확인합니다.
 * @return 실행 가능한 전체 경로 문자열 (동적 할당), 없으면 NULL
 */
char	*get_path(t_ms *ms, char *cmd)
{
	char	**paths;
	char	*path_part;
	char	*path;
	int		i;

	path = find_var(ms, ms->envp, "PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = -1;
	while (paths[++i])
	{
		path_part = ft_strjoin(paths[i], "/");
		path = ft_strjoin(path_part, cmd);
		free(path_part);
		if (access(path, F_OK | X_OK) == 0)
		{
			ft_free_array(paths);
			return (path); // MEMORY: 성공 시 path 반환, 호출한 쪽에서 해제해야 함
		}
		free(path);
	}
	ft_free_array(paths);
	return (NULL);
}

/**
 * @brief 외부 명령어를 실행합니다. 경로를 찾지 못하거나 권한이 없으면 에러 처리 후 종료합니다.
 */
void	run_execve(t_ms *ms, t_cmd *cmd)
{
	char	*path;

	path = NULL;
	if (cmd->full_cmd && cmd->full_cmd[0])
	{
		if (ft_strchr(cmd->full_cmd[0], '/')) // 상대/절대 경로
			path = ft_strdup(cmd->full_cmd[0]);
		else // PATH에서 검색
			// [수정] cmd 구조체 대신 명령어 문자열(cmd->full_cmd[0])을 전달합니다.
			path = get_path(ms, cmd->full_cmd[0]);
	}
	if (!path || access(path, F_OK) != 0)
	{
		error_join(ms, cmd->full_cmd[0], "command not found");
		bi_exit(ms, 127, 1);
	}
	if (access(path, X_OK) != 0)
	{
		error_join(ms, cmd->full_cmd[0], "Permission denied");
		bi_exit(ms, 126, 1);
	}
	// execve 호출 직전에, 자식에게 불필요한 FD들을 모두 닫아줍니다.
	if (ms->std_copy[0] != -1)
		close(ms->std_copy[0]);
	if (ms->std_copy[1] != -1)
		close(ms->std_copy[1]);
	close_pipes(ms);

	execve(path, cmd->full_cmd, ms->envp);
	
	// execve가 실패한 경우에만 아래 코드가 실행됩니다.
	error_join(ms, cmd->full_cmd[0], strerror(errno));
	free(path); // MEMORY FREE
	bi_exit(ms, 127, 1);
}
