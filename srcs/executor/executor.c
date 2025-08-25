/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 12:15:43 by juhyeonl          #+#    #+#             */
/*   Updated: 2025/08/25 12:15:45 by juhyeonl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Updates the '_' environment variable to the last argument of the
** last command in the pipeline.
*/
static void	update_underscore_var(t_ast_node *node, t_shell *shell)
{
	t_ast_node	*last_cmd_node;
	char		*last_arg;
	int			i;

	if (!node)
		return ;
	last_cmd_node = node;
	while (last_cmd_node->type == NODE_PIPE)
		last_cmd_node = last_cmd_node->right;
	if (!last_cmd_node->args || !last_cmd_node->args[0])
		return ;
	i = 0;
	while (last_cmd_node->args[i])
		i++;
	last_arg = last_cmd_node->args[i - 1];
	set_env_value(&(shell->env_list), "_", last_arg);
}

/*
** Main executor function that traverses the AST and executes commands.
*/
int	executor(t_ast_node *node, t_shell *shell)
{
	int	status;

	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		status = execute_pipeline(node, shell);
	else
		status = execute_simple_command(node, shell, 0);
	update_underscore_var(node, shell);
	return (status);
}

/*
** A recursive helper for the executor, especially for pipelines.
*/
int	_executor(t_ast_node *node, t_shell *shell, int is_child)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		return (execute_pipeline(node, shell));
	else
		return (execute_simple_command(node, shell, is_child));
}
