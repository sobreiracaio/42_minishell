/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 19:43:25 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/12 15:50:46 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// o tipo fica logo igual ao shell->ps.
// Logo se for > ou < fica logo definido como tal.
// Só append e heredoc precisam de confirmar 
// se o tipo a seguir igual ao anterior,
// isto é, se for > o tipo inicial,
// se tiver outro > a seguir é pq é append.
// O mm raciocínio para o heredoc.
// Se for pipe fica só definido como pipe.

#include "../../include/minishell.h"

// Esta função verifica o tipo de token encontrado e devolve o seu tipo. Além disso dá-nos tb o próprio token. Ver mais info na função parseredir na comentário da type = gettoken(shell, &token) dentro do while loop.
static int	gettoken_type(t_shell *shell)
{
	int	type;

	type = *shell->ps;
	if (*shell->ps == '<')
	{
		if (*(shell->ps + 1) == '<')
			type = HERE_DOC;
	}
	else if (*shell->ps == '>')
	{
		if (*(shell->ps + 1) == '>')
			type = APPEND;
	}
	else if (*shell->ps && !ft_strchr(OPERATORS, *shell->ps))
		type = 'a';
	if (*shell->ps && type != 'a')
		shell->ps += (type == HERE_DOC || type == APPEND) + 1;
	return (type);
}
// Esta função verifica o tipo de token encontrado e devolve o seu tipo. Além disso dá-nos tb o próprio token. Ver mais info na função parseredir na comentário da type = gettoken(shell, &token) dentro do while loop.
int	gettoken(t_shell *sh, char **token)
{
	int	type;

	while (sh->ps < sh->es && !*sh->ps) // Corre até o ps ser igual ao es ou então até encontrar um nulo (nulo esse colocado na insert_null_char no ficheiro process_line.c).
		sh->ps++;
	if (token) // Se o token for diferente de NULL, ou seja, se o token for um argumento, então o token é igual ao ps, ou seja colocamos um ponteiro a apontar para essa parte da linha de comandos
		*token = sh->ps;
	type = gettoken_type(sh); // Aqui chamamos a função gettoken_type para obter o tipo do token que estamos a analisar, se é <, >, <<, >>, | ou a (argumento).
	while (type == 'a' && sh->ps < sh->es && *sh->ps)
		sh->ps++;
	while (sh->ps < sh->es && !*sh->ps)
		sh->ps++;
	return (type);
}
// A primeira parte (o while) serve só para passar o nulo. Aqui verificamos se a linha chegou ao fim (se o ps for igual ao es) ou se alcançamos um nulo (!*shell->ps), nulo esse que adicionamos na função insert_null_char no ficheiro process_line.c.
// A segunda parte é que verifica efetivamente se encontramos
// o operador que estavamos à procura ou não.

int	peek(t_shell *shell, char *op)
{
	while (shell->ps < shell->es && !*shell->ps)
		shell->ps++;
	return (shell->ps < shell->es && ft_strchr(op, *shell->ps));
}

int	parse_cmd(t_shell *shell)
{
	shell->ps = shell->line;
	shell->es = shell->line + shell->line_len;
	shell->cmd = parsepipe(shell);
	peek(shell, "");
	if (shell->ps != shell->es && shell->status != 2)
		return (!print_error_syntax(shell, shell->ps, 2));
	return (shell->status == CONTINUE);
}
