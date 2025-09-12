/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* file_handler.c                                     :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: hyeon-coder <hyeon-coder@student.42.fr>      +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/09/13 07:20:00 by hyeon-coder      #+#    #+#             */
/* Updated: 2025/09/13 07:20:00 by hyeon-coder     ###   ########.fr       */
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

	// MEMORY ALLOC: 숫자를 문자열로 변환
	no = ft_itoa(i);
	if (!no)
		ms_error(ms, "ft_itoa failure in heredoc", 1, 0);
	// MEMORY ALLOC: ".heredoc"과 숫자 문자열 결합
	name = ft_strjoin(".heredoc", no);
	// MEMORY FREE: 숫자 문자열은 사용 후 즉시 해제
	free(no);
	if (!name)
		ms_error(ms, "heredoc file generation failure", 1, 0);
	return (name);
}

/**
 * @brief 히어독 입력을 처리하고 임시 파일에 저장합니다.
 * @param ms 메인 구조체
 * @param limiter 히어독 종료를 위한 구분자 문자열
 * @param name 임시 파일명
 * @param quoted 구분자가 따옴표로 감싸져 있었는지 여부 (변수 확장 결정)
 * @return 성공 시 0, 인터럽트(Ctrl+C) 시 -1
 */
int	handle_heredoc(t_ms *ms, const char *limiter, char *name, int quoted)
{
	char	*line;
	int		fd;

	fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		ms_error(ms, "Failed to create temporary file for HEREDOC", 1, 0);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0) // EOF 또는 구분자 입력
		{
			if (line)
				free(line);
			break ;
		}
		// (minishell_team의 heredoc_help와 유사한 로직)
		// 변수 확장이 필요한 경우(구분자가 따옴표로 안 감싸인 경우)
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
	return (0);
}

/**
 * @brief 히어독 처리를 시작합니다. 메모리 소유권 이전이 핵심입니다.
 * @return 성공 시 0, 실패(Ctrl+C) 시 1
 */
int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	char	*temp_filename;

	// MEMORY ALLOC: 히어독 임시 파일명 할당
	temp_filename = heredoc_name(ms, ms->heredoc_no++);

	// (minishell_team의 시그널 핸들러 설정 로직 참조)
	// signal(SIGINT, handle_heredoc_signal);

	if (handle_heredoc(ms, lim, temp_filename, quo) == -1) // 사용자 중단
	{
		// MEMORY FREE: 할당했으나 사용하지 않게 된 파일명 메모리 해제
		unlink(temp_filename);
		free(temp_filename);
		ms->heredoc_no--;
		return (1);
	}

	// MEMORY OWNERSHIP TRANSFER:
	// 생성된 파일명의 소유권을 infile->name 포인터로 넘깁니다.
	// 이제 이 메모리의 해제 책임은 infile 구조체를 해제하는 쪽에 있습니다.
	if (infile->name)
		free(infile->name); // 혹시 모를 이전 값 해제
	infile->name = temp_filename;
	return (0);
}

/**
 * @brief 모든 입력 리다이렉션(<, <<)을 처리합니다.
 * !! 메모리 누수 해결을 위한 핵심 수정 사항이 포함되어 있습니다. !!
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
			// (에러 처리 로직)
			perror(infile[i]->name);
			return (-1);
		}
		
		// [WORKAROUND] utils 디렉터리 수정 불가로 인한 메모리 해제 로직
		// 히어독인 경우, 파일을 열어 FD를 확보한 후 즉시 파일을 삭제하고,
		// t_infile->name에 할당된 메모리를 여기서 직접 해제합니다.
		if (infile[i]->heredoc == 1)
		{
			unlink(infile[i]->name);
			// MEMORY FREE: start_heredoc에서 할당된 메모리를 여기서 해제
			free(infile[i]->name);
			infile[i]->name = NULL; // Dangling 포인터 방지
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
		if (append[i] == 1) // >> (append)
			open_flags = O_WRONLY | O_CREAT | O_APPEND;
		else // > (truncate)
			open_flags = O_WRONLY | O_CREAT | O_TRUNC;
		
		ms->fd_out = open(outfile[i], open_flags, 0644);
		if (ms->fd_out < 0)
		{
			perror(outfile[i]);
			return (-1);
		}
		if (dup2(ms->fd_out, STDOUT_FILENO) < 0)
			ms_error(ms, "stdout redirection failed", 1, 0);
		close(ms->fd_out);
		ms->fd_out = -1;
	}
	return (0);
}
