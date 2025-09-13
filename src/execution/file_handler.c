/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* file_handler.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13 07:20:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13/ 09:15:00 by hyeon-coder     ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief 히어독 임시 파일명을 동적으로 생성합니다. (e.g., ".heredoc0")
 * @return 동적으로 할당된 파일명 문자열
 */
char	*heredoc_name(t_ms *ms, int i)
{
	char	*name;
	char	*no;

	no = ft_itoa(i);
	if (!no)
		ms_error(ms, "ft_itoa failure in heredoc", 1, 0);
	name = ft_strjoin(".heredoc", no);
	free(no);
	if (!name)
		ms_error(ms, "heredoc file generation failure", 1, 0);
	return (name);
}

/**
 * @brief 히어독 입력을 처리하고 임시 파일에 저장합니다.
 * readline의 이벤트 훅을 사용하여 Ctrl+C 입력 시 즉시 중단되도록 처리합니다.
 * @return 성공 시 0, 인터럽트(Ctrl+C) 시 -1
 */
int	handle_heredoc(t_ms *ms, const char *limiter, char *name, int quoted)
{
	char	*line;
	int		fd;

	signal(SIGINT, heredoc_sigint_handler);
	rl_event_hook = heredoc_rl_event_hook;
	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ms_error(ms, "Failed to create temporary file for HEREDOC", 1, 0);
	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT || !line || ft_strcmp(line, limiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		if (quoted == UNQUOTED)
		{
			char	*expanded_line;
			bool	was_expanded;

			was_expanded = false;
			expanded_line = expand_input(ms, line, &was_expanded);
			ft_putendl_fd(expanded_line, fd);
			free(expanded_line);
		}
		else
			ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	rl_event_hook = NULL;
	set_interactive_signals();
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		return (-1);
	}
	return (0);
}

/**
 * @brief 히어독 처리를 시작합니다. 메모리 소유권 이전이 핵심입니다.
 * @return 성공 시 0, 실패(Ctrl+C) 시 1
 */
int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	char	*temp_filename;

	// [수정] heredoc_no는 파싱 단계에서 이미 증가되었으므로, 여기서는 사용만 합니다.
	// 파일 인덱스는 0부터 시작해야 하므로 (ms->heredoc_no - 1)을 사용합니다.
	temp_filename = heredoc_name(ms, ms->heredoc_no - 1);
	if (handle_heredoc(ms, lim, temp_filename, quo) == -1)
	{
		unlink(temp_filename);
		free(temp_filename);
		// [수정] 실패 시 파서가 증가시킨 카운터를 되돌립니다.
		ms->heredoc_no--;
		return (1);
	}
	if (infile->name)
		free(infile->name);
	infile->name = temp_filename; // 소유권 이전
	return (0);
}

/**
 * @brief 모든 입력 리다이렉션(<, <<)을 처리합니다.
 */
int	handle_infiles(t_ms *ms, t_infile **infile)
{
	int	i;

	i = -1;
	ms->std_copy[0] = dup(STDIN_FILENO);
	if (ms->std_copy[0] < 0)
		ms_error(ms, "stdin duplication failed", 1, 0);
	ms->reset[0] = 1;
	while (infile[++i])
	{
		ms->fd_in = open(infile[i]->name, O_RDONLY);
		if (ms->fd_in < 0)
		{
			perror(infile[i]->name);
			reset_std(ms);
			return (-1);
		}
		if (infile[i]->heredoc == 1)
		{
			unlink(infile[i]->name);
			// [WORKAROUND] utils를 수정할 수 없으므로, 여기서 직접 메모리 해제
			free(infile[i]->name);
			infile[i]->name = NULL;
		}
		if (dup2(ms->fd_in, STDIN_FILENO) < 0)
			ms_error(ms, "stdin redirection failed", 1, 0);
		close(ms->fd_in);
		ms->fd_in = -1;
	}
	return (0);
}

/**
 * @brief 모든 출력 리다이렉션(>, >>)을 처리합니다.
 */
int	handle_outfiles(t_ms *ms, char **outfile, int *append)
{
	int	i;
	int	open_flags;

	i = -1;
	ms->std_copy[1] = dup(STDOUT_FILENO);
	if (ms->std_copy[1] < 0)
		ms_error(ms, "stdout duplication failed", 1, 0);
	ms->reset[1] = 1;
	while (outfile[++i])
	{
		if (append[i] == 1)
			open_flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			open_flags = O_WRONLY | O_CREAT | O_TRUNC;
		ms->fd_out = open(outfile[i], open_flags, 0644);
		if (ms->fd_out < 0)
		{
			perror(outfile[i]);
			reset_std(ms);
			return (-1);
		}
		if (dup2(ms->fd_out, STDOUT_FILENO) < 0)
			ms_error(ms, "stdout redirection failed", 1, 0);
		close(ms->fd_out);
		ms->fd_out = -1;
	}
	return (0);
}
