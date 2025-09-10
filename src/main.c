/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: JuHyeon <JuHyeon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 13:18:00 by mpierce           #+#    #+#             */
/*   Updated: 2025/09/10 18:11:16 by JuHyeon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Global variable for signal handler
volatile sig_atomic_t	g_signal = 0;

/*
** Counts the number of environment variables in envp.
*/
int	countenvp(char **envp)
{
	int	i;
	int	j;

	i = -1;
	j = 0;
	while (envp[++i])
		j++;
	return (i);
}

/*
** Duplicates the environment variables into a new array.
*/
char	**assignenvp(t_ms *ms, char **envp)
{
	char	**temp;
	int		i;

	i = -1;
	ms->elements = countenvp(envp);
	temp = malloc((ms->elements + 1) * sizeof(char *));
	if (!temp)
		exit(-1);
	while (envp[++i])
	{
		temp[i] = ft_strdup(envp[i]);
		if (!temp[i])
			arr_dup_fail(ms, temp, i);
	}
	temp[i] = NULL;
	return (temp);
}

/*
** Initialise all variables for ms struct
*/
void	init_ms(t_ms *ms, char **envp)
{
	ms->envp = assignenvp(ms, envp);
	ms->exit_status = 0;
	ms->token = NULL;
	ms->ast = NULL;
	ms->word = NULL;
	ms->cmd = NULL;
	ms->fd_in = -1;
	ms->fd_out = -1;
	ms->pids = NULL;
	ms->pid_index = 0;
	ms->cmd_path = NULL;
	ms->std_copy[0] = -1;
	ms->std_copy[1] = -1;
	ms->ms_fd[0] = -1;
	ms->ms_fd[1] = -1;
	ms->prev_fd = -1;
	ms->cmd_index = 0;
	ms->child = true;
	ms->prompt = NULL;
	ms->heredoc_name = NULL;
	ms->heredoc_no = 0;
	ms->prompt = NULL;
	ms->syntax_error = false;
	ms->heredoc_no = 0;
	ms->sigfirst = false;
}

#include <stdio.h> // printf 함수 사용을 위해 추가

// t_segment 리스트를 출력하는 헬퍼 함수
static void	print_segments(t_segment *segments)
{
	t_segment	*current;
	int			i;

	if (!segments)
		return;
	i = 0;
	current = segments;
	while (current)
	{
		printf("      Segment [%d]: seg='%s', quoted=%d\n", i, current->seg, current->quoted);
		current = current->next;
		i++;
	}
}

// t_token 리스트를 출력하는 헬퍼 함수
static void	print_tokens(t_token *tokens)
{
	t_token	*current;
	int		i;

	if (!tokens)
	{
		printf("  ms->token: NULL\n");
		return;
	}
	i = 0;
	current = tokens;
	while (current)
	{
		printf("  Token [%d]: type=%d, value='%s', quoted=%d, expanded=%s\n",
			i, current->type, current->value, current->quoted, current->expanded ? "true" : "false");
		if (current->segments)
		{
			printf("    Segments:\n");
			print_segments(current->segments);
		}
		current = current->next;
		i++;
	}
}

// t_cmd 구조체를 출력하는 헬퍼 함수
static void	print_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
	{
		printf("  t_cmd: NULL\n");
		return;
	}
	printf("  t_cmd: from_expand=%s\n", cmd->from_expand ? "true" : "false");
	printf("  full_cmd:\n");
	if (cmd->full_cmd)
	{
		i = 0;
		while (cmd->full_cmd[i])
		{
			printf("    [%d]: '%s'\n", i, cmd->full_cmd[i]);
			i++;
		}
	}
	else
		printf("    NULL\n");

	printf("  infile:\n");
	if (cmd->infile)
	{
		i = 0;
		while (cmd->infile[i])
		{
			printf("    [%d]: name='%s', heredoc=%d\n", i, cmd->infile[i]->name, cmd->infile[i]->heredoc);
			i++;
		}
	}
	else
		printf("    NULL\n");

	printf("  outfile:\n");
	if (cmd->outfile)
	{
		i = 0;
		while (cmd->outfile[i])
		{
			printf("    [%d]: '%s'\n", i, cmd->outfile[i]);
			i++;
		}
	}
	else
		printf("    NULL\n");

	printf("  append:\n");
	if (cmd->append)
	{
		i = 0;
		while (cmd->outfile && cmd->outfile[i])
		{
			printf("    [%d]: %d\n", i, cmd->append[i]);
			i++;
		}
	}
	else
		printf("    NULL\n");
}

// t_ast 트리를 재귀적으로 출력하는 헬퍼 함수
static void	print_ast(t_ast *ast, int level)
{
	int	i;

	if (!ast)
		return;

	for (i = 0; i < level; i++)
		printf("  ");

	printf("AST Node (level %d): type=%d\n", level, ast->type);
	if (ast->cmd)
		print_cmd(ast->cmd);

	if (ast->left)
	{
		for (i = 0; i < level; i++)
			printf("  ");
		printf("  Left Child:\n");
		print_ast(ast->left, level + 1);
	}
	if (ast->right)
	{
		for (i = 0; i < level; i++)
			printf("  ");
		printf("  Right Child:\n");
		print_ast(ast->right, level + 1);
	}
}

// 메인 ms 구조체를 출력하는 함수
void	print_ms_struct(t_ms *ms)
{
	int	i;

	if (!ms)
	{
		printf("Error: ms struct is NULL\n");
		return;
	}
	printf("\n==================== AFTER FREE: MS STRUCT CONTENT ====================\n");
	printf("Top-level fields:\n");
	printf("  exit_status: %d\n", ms->exit_status);
	printf("  elements: %d\n", ms->elements);
	printf("  cmd_no: %d\n", ms->cmd_no);
	printf("  heredoc_no: %d\n", ms->heredoc_no);
	printf("  syntax_error: %s\n", ms->syntax_error ? "true" : "false");
	printf("  heredoc_stop: %s\n", ms->heredoc_stop ? "true" : "false");
	printf("  prompt: %s\n", ms->prompt ? ms->prompt : "NULL");

	printf("\nEnvironment variables (envp):\n");
	if (ms->envp)
	{
		for (i = 0; i < ms->elements; i++)
			printf("  [%d]: %s\n", i, ms->envp[i]);
	}
	else
		printf("  NULL\n");

	printf("\nToken List:\n");
	print_tokens(ms->token);

	printf("\nWord List:\n");
	if (ms->word)
		printf("  ms->word: NOT NULL\n");
	else
		printf("  ms->word: NULL\n");

	printf("\nAbstract Syntax Tree (AST):\n");
	print_ast(ms->ast, 0);
	printf("=========================================================================\n");
}

/*
** Initialize the minishell struct and set up minishell.
** Set shell level and receive the prompt.
** Enter the main loop to process input.
*/
int	main(int argc, char **argv, char **envp)
{
	t_ms				ms;

	(void)argc;
	(void)argv;
	init_ms(&ms, envp);
	get_minishell(&ms);
	shlvl(&ms);
	set_interactive_signals();
	get_prompt(&ms);
	while (true)
	{
		ms.heredoc_stop = false;
		process_input(&ms);
		get_prompt(&ms);
		free_structs(&ms);
		// free(ms.token->value);
		// printf("[DEBUG]");
		// print_ms_struct(&ms);	// tmp
	}
	return (0);
}
