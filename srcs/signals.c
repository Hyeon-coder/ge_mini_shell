/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/*   Updated: 2025/08/24 15:30:02 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <termios.h>
#include <unistd.h> // isatty

// 인터랙티브 모드(readline 프롬프트) 핸들러
static void	sigint_handler_interactive(int sig)
{
	(void)sig;
	g_exit_status = 1; // Ctrl+C는 종료 상태 1 설정
	ft_putchar_fd('\n', 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// ECHOCTL 제어 (Ctrl+C 입력 시 ^C 표시 여부)
// "Inappropriate ioctl for device" 에러 해결
static void set_echoctl(int enable)
{
    struct termios term;

    // STDIN이 터미널(tty)인 경우에만 시도
    if (!isatty(STDIN_FILENO))
        return;

    if (tcgetattr(STDIN_FILENO, &term) != 0)
        return;

    if (enable)
        term.c_lflag |= ECHOCTL;
    else
        term.c_lflag &= ~ECHOCTL; // ECHOCTL 비활성화

    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

// 기본 설정 (인터랙티브 readline 루프용)
void	setup_signals(void)
{
	struct sigaction	sa_int;

    set_echoctl(0); // 프롬프트에서 ^C 표시 끔

	sa_int.sa_handler = sigint_handler_interactive;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN); // Ctrl+\ 무시
}

/* --- 아래 함수들은 minishell.h에 선언 필요 --- */

// 부모 프로세스가 자식 대기 시 (시그널 무시)
void    setup_signals_parent(void)
{
    set_echoctl(1); // 명령어 실행 중에는 터미널 설정 복원
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

// 자식 프로세스 실행 시 (기본 동작 복원)
void    setup_signals_child(void)
{
    set_echoctl(1);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}
