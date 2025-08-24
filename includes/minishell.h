/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* minishell.h                                        :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: your_login <your_login@student.42.fr>      +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 12:15:00 by your_login       ###   ########.fr       */
/* */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

// 전역 변수: 시그널 번호 저장
extern int	g_exit_status;

/* --- 토큰 타입 열거형 --- */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

/* --- 토큰 구조체 --- */
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

/* --- AST 노드 타입 열거형 --- */
typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

/* --- 리디렉션 구조체 --- */
typedef struct s_redir
{
	t_token_type	type;
	char			*filename;
	struct s_redir	*next;
}	t_redir;

/* --- AST 노드 구조체 --- */
typedef struct s_ast_node
{
	t_node_type			type;
	char				**args;
	t_redir				*redirs;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

/* --- 환경 변수 구조체 --- */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

/* --- 쉘 상태 구조체 --- */
typedef struct s_shell
{
	t_env	*env_list;
	int		last_exit_status;
}	t_shell;

/* --- Core (main.c) --- */
void		shell_loop(t_shell *shell);

/* --- Signals (signals.c) --- */
void		setup_signals(void);

/* --- Error (error.c) --- */
void		print_error(char *cmd, char *arg, int exit_code);
int			set_exit_status(int status);

/* --- Environment (env/) --- */
t_env		*init_env(char **envp);
char		*get_env_value(t_env *env_list, char *key);
void		set_env_value(t_env **env_list, char *key, char *value);
void		free_env_list(t_env *env_list);
char		**env_list_to_array(t_env *env_list);

/* --- Lexer (lexer/) --- */
t_token		*lexer(char *line);
void		free_tokens(t_token *tokens);
t_token		*new_token(char *value, t_token_type type);
void		add_token(t_token **head, t_token *new);
int			is_special_char(char c);

/* --- Parser (parser/) --- */
t_ast_node	*parser(t_token **tokens);
void		free_ast(t_ast_node *node);
t_ast_node	*new_ast_node(t_node_type type);
int			count_args_in_cmd(t_token **tokens);
void		fill_args_and_redirs(t_ast_node *node, t_token **tokens);
void		consume_token(t_token **tokens);

/* --- Expander (expander/) --- */
void		expand_ast(t_ast_node *node, t_shell *shell);
char		*expand_string(char *str, t_shell *shell);

/* --- Executor (executor/) --- */
int			executor(t_ast_node *node, t_shell *shell);
int			execute_simple_command(t_ast_node *node, t_shell *shell);
int			execute_pipeline(t_ast_node *node, t_shell *shell);
int			apply_redirections(t_redir *redirs);
int			apply_redirections_for_empty(t_redir *redirs);
char		*find_command_path(char *cmd, t_env *env_list);

/* --- Builtins (builtins/) --- */
int			execute_builtin(char **args, t_shell *shell);
int			is_builtin(char *cmd);
int			builtin_echo(char **args);
int			builtin_cd(char **args, t_shell *shell);
int			builtin_pwd(void);
int			builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);
int			builtin_env(t_shell *shell);
int			builtin_exit(char **args, t_shell *shell);

/* --- Memory (memory/) --- */
// void		free_string_array(char **arr);
// void		cleanup(char *line, t_token *tokens, t_ast_node *ast);
void		free_string_array(char **arr);
void		cleanup(t_token *tokens, t_ast_node *ast);

#endif
