/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* heredoc.c                                          :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/08/29 17:53:24 by JuHyeon           #+#    #+#             */
/* Updated: 2025/08/30/ 04:00:00 by JuHyeon          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"


/*
** Writes a single line to the heredoc's temporary file.
** It expands variables within the line if the delimiter was not quoted.
*/
static void	write_to_heredoc(t_ms *ms, char *line, int fd, int expand)
{
	char	*expanded_line;
	bool	was_expanded;

	if (expand)
	{
		was_expanded = false;
		expanded_line = expand_input(ms, line, &was_expanded);
		ft_putendl_fd(expanded_line, fd);
		free(expanded_line);
	}
	else
		ft_putendl_fd(line, fd);
	free(line);
}

/*
** This is the signal handler for SIGINT (Ctrl+C) specifically
** during heredoc input. It sets a global flag and uses ioctl to push a
** newline character into the input buffer. This forces the blocked
** readline() call to return immediately, allowing the loop to terminate.
*/
void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_signal = SIGINT;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
}

static void	heredoc_child_loop(t_ms *ms, char *lim, int fd, int quo)
{
	char	*line;

	// 자식 프로세스에서는 Ctrl+C를 받으면 그냥 기본 동작(종료)을 따르도록 합니다.
	signal(SIGINT, SIG_DFL);

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, lim) == 0)
		{
			free(line);
			break ;
		}
		// write_to_heredoc은 기존 함수를 그대로 사용
		write_to_heredoc(ms, line, fd, (quo == UNQUOTED));
	}
	// 자식 프로세스는 여기서 역할을 다하고 종료합니다.
	// ms 구조체의 메모리 등을 해제하고 exit해야 할 수도 있습니다.
	// free_ms(ms); // 프로젝트의 메모리 관리 정책에 따라 필요할 수 있습니다.
	exit(0);
}



/*
** The main loop for reading heredoc input from the user. It sets up
** temporary signal and terminal settings to handle Ctrl+C interruptions
** correctly, mimicking the behavior of bash.
*/
// static int	heredoc_loop(t_ms *ms, char *lim, int fd, int quo)
// {
// 	char				*line;
// 	struct sigaction	sa_old;
// 	struct termios		orig_termios;

// 	setup_heredoc_handlers(&orig_termios, &sa_old);
// 	while (1)
// 	{
// 		line = readline("> ");
		
// 		// Ctrl+C 감지 시 즉시 처리
// 		if (g_signal == SIGINT)
// 		{
// 			get_minishell(NULL)->exit_status = 1;
// 			get_minishell(NULL)->heredoc_stop = true;
// 			rl_replace_line("", 0);
// 			rl_on_new_line();
// 			rl_redisplay();  // 즉시 프롬프트 표시
// 			if (line)
// 				free(line);
// 			break ;
// 		}
		
// 		if (!line || ft_strcmp(line, lim) == 0)
// 		{
// 			if (line)
// 				free(line);
// 			break ;
// 		}
		
// 		write_to_heredoc(ms, line, fd, (quo == UNQUOTED));
// 	}
	
// 	restore_handlers(&orig_termios, &sa_old);
	
// 	// 시그널 받았으면 상태 설정
// 	if (g_signal == SIGINT)
// 	{
// 		ms->exit_status = 1;
// 		ms->heredoc_stop = true;
// 		g_signal = 0;  // 시그널 플래그 리셋
// 	}
	
// 	return (0);
// }

/*
** Initializes the heredoc process. It creates a temporary file,
** runs the input loop, and finalizes the redirection by updating
** the infile information for the command.
*/
int	start_heredoc(t_ms *ms, char *lim, t_infile *infile, int quo)
{
	int		fd;
	char	*filename;
	pid_t	pid;
	int		status;

	fd = open_heredoc_file(&filename, ms); // heredoc_utils.c의 기존 함수
	
	// 메인 셸(부모)은 자식이 일하는 동안 잠시 Ctrl+C를 무시합니다.
	signal(SIGINT, SIG_IGN);

	pid = fork();
	if (pid == -1)
		ms_error(ms, "fork failed", 1, 0);
	
	if (pid == 0) // 자식 프로세스: 히어독 입력을 전담
	{
		heredoc_child_loop(ms, lim, fd, quo);
	}
	
	// 부모 프로세스: 자식이 끝날 때까지 기다립니다.
	waitpid(pid, &status, 0);
	
	// 자식이 끝나면 메인 셸은 다시 원래의 시그널 핸들러로 돌아옵니다.
	set_interactive_signals(); // signals.c의 함수

	close(fd);

	// 자식이 정상적으로 종료되었는지, 아니면 시그널로 종료되었는지 확인합니다.
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		// Ctrl+C로 자식이 종료된 경우
		ms->heredoc_stop = true;
		ms->exit_status = 1;
		write(1, "\n", 1);
		unlink(filename); // 임시 파일 삭제
		free(filename);
		return (0); // 실패(중단)했음을 알림
	}

	// 정상적으로 종료된 경우
	free(infile->name);
	infile->name = filename;
	return (0);
}
