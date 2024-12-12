/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 18:52:19 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/13 01:43:16 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// A função path_slash é chamada na função cdpath. Se o último caractere do CDPATH for diferente de '/', a função retorna um '/' e o diretório que estamos à procura. Se o último caractere do CDPATH for igual a '/', a função retorna o diretório que estamos à procura.
static char	*path_slash(char *cdpath, char **path)
{
	char	*tmp;

	if (cdpath[ft_strlen(cdpath) - 1] != '/') // Se o último caractere do CDPATH for diferente de '/'.
	{
		if (*path[0] != '/') // Se o diretório que estamos à procura for diferente de '/'.
		{
			tmp = ft_strjoin("/", *path); // Junta o diretório que estamos à procura com '/'.
			return (tmp);
		}
		else
			return (ft_strdup(cdpath));// Retorna o CDPATH.
	}
	else
	{
		if (*path[0] == '/') // Se o diretório que estamos à procura for igual a '/'.
		{
			tmp = ft_strdup(*path + 1); // Retorna o diretório que estamos à procura.
			return (tmp);
		}
		else
			return (ft_strdup(cdpath)); // Retorna o CDPATH.
	}
}

// A função cdpath é chamada na função ms_cd. Se o diretório que estamos à procura está no CDPATH, a função retorna true. Se o diretório que estamos à procura não está no CDPATH, a função retorna false.
static bool	cdpath(t_shell *shell, char *path, int i)
{
	char	**cdpath;
	char	*tmp;
	char	*tmp_path;

	if (!env_get("CDPATH", shell) || path[0] == '/')
		return (false);
	cdpath = ft_split(env_get("CDPATH", shell), ':');
	while (cdpath[i++])
	{
		tmp_path = path_slash(cdpath[i], &path); // Se o diretório que estamos à procura está no CDPATH.
		tmp = ft_strjoin(cdpath[i], tmp_path); // Junta o diretório que estamos à procura com o CDPATH.
		free(tmp_path);
		if (ms_chdir(shell, tmp)) // Se o diretório existir.
		{
			tmp[ft_strlen(tmp) - 1] = '\0'; // O último caractere do diretório é igual a '\0'.
			ft_putendl_fd(tmp, STDOUT_FILENO); // Imprime o diretório.
			ft_free_array(cdpath);
			free(tmp);
			return (true);
		}
		free(tmp);
	}
	ft_free_array(cdpath);
	return (false);
}

// Esta função imprime o diretório atual. Se o diretório atual for um diretório home, a função imprime o diretório home. Se o diretório atual for um diretório diferente de home, a função imprime o diretório atual.
static void	last_pwd(t_shell *shell, char *pwd)
{
	char	*str;

	if (pwd[0] != '~')  // Se o diretório atual for diferente de home.
		ft_putendl_fd(pwd, STDOUT_FILENO); // Imprime o diretório atual.
	else
	{
		str = ft_strjoin(env_get("HOME", shell), &pwd[1]); // Se o diretório atual for home.
		ft_putendl_fd(str, STDOUT_FILENO); // Imprime o diretório home.
		free(str);
	}
}
// Esta função muda o diretório atual para o diretório especificado. Se o diretório não existir, a função retorna false. Se o diretório existir, a função retorna true.
bool	ms_chdir(t_shell *shell, char *path)
{
	char	*tmp_pwd;

	if (!path)
		return (false);
	tmp_pwd = getcwd(NULL, 0); // Retorna o diretório atual.
	if (chdir(path) != 0) // Se chdir que vem de unistd.h não for 0. chdir muda o diretório atual para o diretório especificado.
	{
		free(tmp_pwd);
		return (false);
	}
	env_export(shell, "OLDPWD", tmp_pwd, 1); // Exporta o diretório antigo.
	free(tmp_pwd);
	tmp_pwd = getcwd(NULL, 0);
	env_export(shell, "PWD", tmp_pwd, 1); // Exporta o diretório atual.
	free(tmp_pwd);
	return (true);
}

// Esta função verifica se HOME e OLDPWD estão definidos. Se HOME e OLDPWD não estiverem definidos, a função retorna um erro. Se HOME e OLDPWD estiverem definidos, a função muda o diretório atual para o diretório home e faz isso na função ms_chdir. Esta função verifica tb se o diretório que estamos à procura está no CDPATH ou não existe OLD PWD ou PWD (o que é verificado na função ms_chdir), então imprime um erro de diretório ou arquivo não encontrado.
void	ms_cd(t_shell *shell, t_exec *cmd)
{
	if (!cmd->argv[1] || !*cmd->argv[1]) // Se o argumento for NULL ou vazio.
	{
		if (!ms_chdir(shell, env_get("HOME", shell))) // Se o diretório home não existir.
			print_error(shell, "cd", "HOME not set", 1); // Imprime um erro.
	}
	else
	{
		if (cmd->argv[2]) // Se houver mais de um argumento.
			print_error(shell, "cd", "too many arguments", 1); // Imprime um erro.
		else if (ft_strcmp(cmd->argv[1], "-") == 0) // Se o argumento for "-".
		{
			if (!ms_chdir(shell, env_get("OLDPWD", shell))) // Se o diretório antigo não existir.
			{
				print_error(shell, "cd", "OLDPWD not set", 1); // Imprime um erro.
			}
			last_pwd(shell, env_get("PWD", shell)); // Imprime o diretório atual.
		}
		else if (cmd->argv[1][0] // Se o argumento for um diretório.
			&& !ms_chdir(shell, cmd->argv[1]) // Se o diretório não existir.
			&& !cdpath(shell, cmd->argv[1], 0)) // Se o diretório não existir no CDPATH.
			print_error(shell, "cd: no such file or directory",
				cmd->argv[1], 1);
	}
	if (shell->status == CONTINUE)
		g_exit = 0;
}
