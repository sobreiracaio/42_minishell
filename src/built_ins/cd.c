/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:52:19 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/09 18:07:30 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*path_slash(char *cdpath, char **path)
{
	char	*tmp;

	if (cdpath[ft_strlen(cdpath) - 1] != '/')
	{
		if (*path[0] != '/')
		{
			tmp = ft_strjoin("/", *path);
			return (tmp);
		}
		else
			return (ft_strdup(cdpath));
	}
	else
	{
		if (*path[0] == '/')
		{
			tmp = ft_strdup(*path + 1);
			return (tmp);
		}
		else
			return (ft_strdup(cdpath));
	}
}

static bool	cdpath(t_shell *shell, char *path)
{
	char	**cdpath;
	char	*tmp;
	char	*tmp_path;
	int		i;

	if (!env_get("CDPATH", shell) || path[0] == '/')
		return (false);
	cdpath = ft_split(env_get("CDPATH", shell), ':');
	i = 0;
	while (cdpath[i])
	{
		tmp_path = path_slash(cdpath[i], &path);
		tmp = ft_strjoin(cdpath[i], tmp_path);
		free(tmp_path);
		if (ms_chdir(shell, tmp))
		{
			tmp[ft_strlen(tmp) - 1] = '\0';
			ft_putendl_fd(tmp, STDOUT_FILENO);
			ft_free_array(cdpath);
			free(tmp);
			return (true);
		}
		free(tmp);
		i++;
	}
	ft_free_array(cdpath);
	return (false);
}


static void	hyphen_cd_print(t_shell *shell, char *pwd)
{
	char	*str;

	if (pwd[0] != '~')
		ft_putendl_fd(pwd, STDOUT_FILENO);
	else
	{
		str = ft_strjoin(env_get("HOME", shell), &pwd[1]);
		ft_putendl_fd(str, STDOUT_FILENO);
		free(str);
	}
}

bool	ms_chdir(t_shell *shell, char *path)
{
	char	*tmp_pwd;

	if (!path)
		return (false);
	tmp_pwd = getcwd(NULL, 0);
	if (chdir(path) != 0)
	{
		free(tmp_pwd);
		return (false);
	}
	env_export(shell, "OLDPWD", tmp_pwd, 1);
	free(tmp_pwd);
	tmp_pwd = getcwd(NULL, 0);
	env_export(shell, "PWD", tmp_pwd, 1);
	free(tmp_pwd);
	return (true);
}

void	ms_cd(t_shell *shell, t_exec *cmd)
{
	if (!cmd->argv[1] || !*cmd->argv[1])
	{
		if (!ms_chdir(shell, env_get("HOME", shell)))
			print_error(shell, "cd", "HOME not set", 1);
	}
	else
	{
		if (cmd->argv[2])
			print_error(shell, "cd", "too many arguments", 1);
		else if (ft_strcmp(cmd->argv[1], "-") == 0)
		{
			if (!ms_chdir(shell, env_get("OLDPWD", shell)))
			{
				print_error(shell, "cd", "OLDPWD not set", 1);
			}
			hyphen_cd_print(shell, env_get("PWD", shell));
		}
		else if (cmd->argv[1][0]
			&& !ms_chdir(shell, cmd->argv[1]) && !cdpath(shell, cmd->argv[1]))
			print_error(shell, "cd: no such file or directory",
				cmd->argv[1], 1);
	}
	if (shell->status == CONTINUE)
		g_exit = 0;
}
