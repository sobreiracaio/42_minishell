/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jode-jes <jode-jes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:30:07 by joaosilva         #+#    #+#             */
/*   Updated: 2024/05/27 13:57:29 by jode-jes         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
// Código do Balsa

#include "../../include/minishell.h"

void	trim_arg(char *arg)
{
	int	squote;
	int	dquote;

	dquote = 0;
	squote = 0;
	while (*arg)
	{
		if (*arg == '"' && !squote)
			dquote = !dquote;
		if (*arg == '\'' && !dquote)
			squote = !squote;
		if (ft_strchr(SPACES, *arg) && !squote && !dquote)
			*arg = '\0';
		arg++;
	}
}

void	trim_quotes(char *arg, int *len)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (i < *len)
	{
		if ((arg[i] == '"' || arg[i] == '\'') && !quote)
		{
			quote = arg[i];
			memmove(arg + i, arg + i + 1, *len - i);
			(*len)--;
		}
		else if (quote && arg[i] == quote)
		{
			quote = 0;
			memmove(arg + i, arg + i + 1, *len - i);
			(*len)--;
		}
		else
			i++;
	}
}

*/
#include "../../include/minishell.h"

void	trim_arg(t_shell *shell, char *arg)
{
	while (*arg)
	{
		if (ft_strchr(SPACES, *arg) && !inside_quotes(shell->line, arg))
			*arg = '\0';
		arg++;
	}
}

void	trim_quotes(char *arg, int *len)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (i < *len)
	{
		if ((arg[i] == '"' || arg[i] == '\'') && !quote)
		{
			quote = arg[i];
			memmove(arg + i, arg + i + 1, *len - i);
			(*len)--;
		}
		else if (quote && arg[i] == quote)
		{
			quote = 0;
			memmove(arg + i, arg + i + 1, *len - i);
			(*len)--;
		}
		else
			i++;
	}
}
