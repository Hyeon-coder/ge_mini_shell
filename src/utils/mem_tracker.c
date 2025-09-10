// src/utils/mem_tracker.c

#include "minishell.h"
#include <stdio.h>

// 전역 변수로 현재 사용 중인 메모리 바이트를 추적합니다.
long long g_total_allocated_bytes = 0;

void	*track_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (ptr)
	{
		g_total_allocated_bytes += size;
		printf("ALLOC: %lld bytes total, new block size %zu\n", g_total_allocated_bytes, size);
	}
	return (ptr);
}

void	track_free(void *ptr, size_t size)
{
	if (ptr)
	{
		g_total_allocated_bytes -= size;
		printf("FREE: %lld bytes total, freed block size %zu\n", g_total_allocated_bytes, size);
		free(ptr);
	}
}

void	print_mem_usage(const char *label)
{
	printf("--- Memory Usage (%s) ---\n", label);
	printf("Current allocated bytes: %lld\n", g_total_allocated_bytes);
	printf("------------------------------------\n");
}

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
