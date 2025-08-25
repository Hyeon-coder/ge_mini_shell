/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 10:44:51 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/25 10:44:53 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status;

/*
** Trims, tokenizes, parses, expands, and executes a single line of command.
*/
static void	execute_line(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_token		*tokens_head;
	t_ast_node	*ast;
	char		*trimmed_line;

	trimmed_line = ft_strtrim(line, " \t\n");
	if (!trimmed_line || !*trimmed_line)
	{
		free(trimmed_line);
		return ;
	}
	tokens = lexer(trimmed_line);
	tokens_head = tokens;
	if (!tokens)
	{
		shell->last_exit_status = g_exit_status;
		free(trimmed_line);
		return ;
	}
	ast = parser(&tokens);
	if (!ast)
	{
		cleanup(tokens_head, NULL);
		shell->last_exit_status = g_exit_status;
		free(trimmed_line);
		return ;
	}
	expand_ast(ast, shell);
	shell->last_exit_status = executor(ast, shell);
	g_exit_status = shell->last_exit_status;
	cleanup(tokens_head, ast);
	free(trimmed_line);
}

/*
** The main loop for interactive mode. It displays a prompt and reads
** commands using readline.
*/
void	shell_loop(t_shell *shell)
{
	char	*line;

	while (1)
	{
		setup_signals();
		line = readline("minishell$ ");
		if (!line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (g_exit_status == 130)
		{
			shell->last_exit_status = 130;
			g_exit_status = 0;
		}
		if (*line)
		{
			add_history(line);
			execute_line(line, shell);
		}
		free(line);
	}
}

/*
** Handles non-interactive mode, reading commands line by line from
** standard input using get_next_line.
*/
void	non_interactive_mode(t_shell *shell)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		execute_line(line, shell);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
}

/*
** Initializes the shell, determines the mode (interactive or non-interactive),
** and starts the appropriate loop.
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	g_exit_status = 0;
	shell.last_exit_status = 0;
	shell.env_list = init_env(envp);
	if (!shell.env_list)
	{
		ft_putstr_fd("minishell: Environment initialization failed\n", 2);
		return (1);
	}
	if (isatty(STDIN_FILENO))
		shell_loop(&shell);
	else
		non_interactive_mode(&shell);
	free_env_list(shell.env_list);
	if (isatty(STDIN_FILENO))
		rl_clear_history();
	return (shell.last_exit_status);
}
