
#include "minishell.h"

/*
** Executes the 'echo' builtin command.
** Prints the arguments to standard output.
** Handles the '-n' option to suppress the trailing newline.
*/
void	builtin_echo(t_cmd *cmd)
{
	int		i;
	bool	newline;

	newline = true;
	i = 1;
	if (cmd->full_cmd[i] && ft_strcmp(cmd->full_cmd[i], "-n") == 0)
	{
		newline = false;
		i++;
	}
	while (cmd->full_cmd[i])
	{
		ft_putstr_fd(cmd->full_cmd[i], 1);
		if (cmd->full_cmd[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", 1);
}
