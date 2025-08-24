/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* executor.c                                         :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: juhyeonl <juhyeonl@student.42.fr>          +#+  +:+       +#+        */
/*+#+#+#+#+#+   +#+           */
/* Created: 2025/08/24 11:27:00 by your_login        #+#    #+#             */
/* Updated: 2025/08/24 19:30:00 by juhyeonl         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

static void	update_underscore_var(t_ast_node *node, t_shell *shell)
{
	t_ast_node	*last_cmd;
	char		*last_arg;
	char		*full_path;
	int			i;

	if (!node)
		return ;
	last_cmd = node;
	while (last_cmd->type == NODE_PIPE)
		last_cmd = last_cmd->right;
	if (last_cmd->args && last_cmd->args[0])
	{
		i = 0;
		while (last_cmd->args[i])
			i++;
		last_arg = last_cmd->args[i - 1];
		if (is_builtin(last_cmd->args[0]))
			set_env_value(&(shell->env_list), "_", last_arg);
		else
		{
			full_path = find_command_path(last_cmd->args[0], shell->env_list);
			if (full_path)
			{
				set_env_value(&(shell->env_list), "_", full_path);
				free(full_path);
			}
			else
				set_env_value(&(shell->env_list), "_", last_cmd->args[0]);
		}
	}
}

int	executor(t_ast_node *node, t_shell *shell)
{
	int status;

	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		status = execute_pipeline(node, shell);
	else
		status = execute_simple_command(node, shell, 0);
	update_underscore_var(node, shell);
	return (status);
}

int	_executor(t_ast_node *node, t_shell *shell, int is_child)
{
	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
		return (execute_pipeline(node, shell));
	else
		return (execute_simple_command(node, shell, is_child));
}
