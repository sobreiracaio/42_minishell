/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envp1_convert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 11:26:00 by joaosilva         #+#    #+#             */
/*   Updated: 2024/06/13 02:15:31 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
************** OBJETIVO DAS FUNÇÕES NESTE FICHEIRO ENVP1_CONVERT.C:

Nesta pasta iremos copiar as variáveis de ambiente (env) do sistema (da shell)
// para o nosso programa (minishell).
Iremos criar 3 variáveis distintas e adicioná-las à nossa struct t_shell, consoante a utilidade que lhe queremos dar:
Resumindo, 
1ª variável - shell->env_list_unsorted (a lista ligada por ordem de criação),
2ª variável - shell->env_sorted (a lista ligada por ordem alfabética), e
3ª variável - shell->envp_char (a char **envp).

**************** PARA QUE SERVEM?:
1 - A shell->env_list_unsorted serve para imprimir as variáveis quando
	executamos o comando env (ou seja,
	exibi-las por ordem de criação). É criada na função
	convert_envp_to_linked_lists que chama a função create_update_envp_lists que por sua vez 
	chama a função add_node_to_envp_unsorted_list (semelhante à append_node do push_swap), que 
	essa sim cria a lista.
2- A shell->env_sorted serve para imprimir as variáveis por ordem alfabética
	quando executamos o comando export (ou seja,
	exibi-las por ordem alfabética). É criada tb na função
	convert_envp_to_linked_lists, através da função create_update_envp_lists.que invoca a função add_node_to_envp_sorted_list, 
	que cria e ordena a lista e q é chamada cada vez q é preciso a atualizar.
3 - A shell->envp_char ou char **envp é o que será passado para o execve,
	para que o processo filho tenha acesso às variáveis de ambiente. É criada na
	função convert_envp_to_char e atualizada sempre que houver uma alteração nas
	variáveis de ambiente (env) e por consequência na lista_unsorted. 
	Se houver uma adição, remoção ou modificação de uma variável de ambiente,
	teremos de chamar a função convert_envp_to_char para atualizar a char **envp.
	Assim, ela é chamada na add_node_to_envp_unsorted_list, env_rm e env_mod (e aqui tinhamos um erro, pq a convert_envp_to_char não era chamada na env_mod (nem na env_export, que era uma alternativa a ser chamada na env_mod), o que faz com que não seja atualizada). Entretanto já acrescentei o que faltava na linha 46 da env_mod.
As restantes funções são auxiliares a estas.

********************* ATUALIZAÇÃO DAS LISTAS (unsorted e sorted) e da array de strings (char **envp_char):

Sempre que houver uma alteração nas variáveis de ambiente (env),
a lista_unsorted, a lista_sorted e a char **envp_char têm de ser atualizadas.

As listas só podem ser alteradas através do comando export ou unset.
O unset remove a variável (e para tal criamos a função env_rm). 
O export pode:
1 - criá-las (se fizermos o comando export somente com a key a variável fica invisível na env, apenas aparece quando fazemos export. 
	Se fizermos export com a key e = ou se fizermos com a key e = e valo, então fica visível quando fazemos o comando env e qd fazemos o export.
	Usa as funções add_node_to_envp_unsorted_list ou add_node_to_envp_sorted_list para tal.
2 - modificá-las
	alterá-las (quando colocamos o nome da variável (a sua key)
	e o novo valor à frente)... e pode ser chamada na env_export.
	Usa a função env_mod_sorted_list e env_mod_unsorted_list para tal.
3 - imprimi-las:
	... por ordem alfabética quando fazemos somente o comando export (isto é, não acompanhado
	por nenhum argumento). Apresenta as vísiveis e
	não visíveis. As não vísiveis apenas não aparecem quando executamos o env, como já referido.
	Para imprimir usa a função env_print.

*************** FUNÇÕES E SUAS LIGAÇÕES ***************

1 - A create_update_envp_lists(shell, key, value, visible) q chama a 
add_node_to_envp_unsorted_list(shell, key, value, visible);
ou a add_node_to_envp_sorted_list(shell, key, value, visible);
é chamada na env_export e na convert_envp_to_linked_lists.
2 - A env_mod é chamada na env_export após verificação por parte da env_get.
3 - A env_get é chamada na
env_mod e na print_to_envp_tilde.
4 - A env_rm é chamada no ms_unset (nos built-ins).
5 - A env_export é chamada no ms_export (nos built-ins).
6 - A convert_envp_to_linked_lists é chamada no env_export e no init_shell_variables.
A convert_envp_to_char é chamada unicamente no init_shell_variables, pq na export é chamada a reate_update_envp_lists.

*/

/* Primeiramente copiava a lista_sorted da lista_unsorted e dps ordenava essa última lista. 
// Daí a função copy_list. Contudo desisti dessa abordagem e optei por criar a lista_sorted diretamente e ordená-la enquanto a crio.
t_env *copy_list(t_env *env_list_unsorted)
{
	t_env	*new_list;

	if (!env_list_unsorted)
		return (NULL);
	new_list = (t_env*)malloc(sizeof(t_env));
	new_list->key = env_list_unsorted->key;
	new_list->value = env_list_unsorted->value;
	new_list->visible = env_list_unsorted->visible;
	new_list->next = copy_list(env_list_unsorted->next);
	return (new_list);
}
 */
// Função auxiliar da convert_envo_to char, que serve para alocar memória para o char **envp_char, ao qual daremos free quando não precisarmos mais dele, ou seja, no main/final do programa.
void	alocate_memory_to_envp_char(t_shell *shell)
{
	if (shell->envp_char)
		ft_free_array(shell->envp_char);
	if (!shell->env_list_unsorted)
	{
		shell->envp_char = NULL;
		return ;
	}
	shell->envp_char = calloc(shell->envp_size + 1, sizeof(char *));
}

// Função para criar a char **envp a partir da lista
// ligada de variáveis de ambiente.
// +2 for the '=' and '\0'
// printenv é o comando usado para verificar se o envp_char está atualizado. 
// Se quisermos ver todas as variáveis fazemos apenas o comando printenv.
// Se quisermos ver apenas uma variável, fazemos printenv e o nome da variável.
// Por exemplo, printenv HOME ou printenv PATH, ou printenv e o nome da nova variável que criamos.
// Variável que criamos usando o comando export para tal. Se fizermos export nome=valor, o printenv nome
// tem de retornar o valor. Se dps modificamos o valor com export nome=valor2, 
// o printenv nome tem de nos dar o valor2 (o valor atualizado)). 
// Se não der é pq a array de strings não está a atualizar.
void	convert_envp_to_char(t_shell *shell)
{
	t_env	*tmp;
	char	*env;
	int		i;

	alocate_memory_to_envp_char(shell); 
	tmp = shell->env_list_unsorted; // A ordem das envp na array de strings tem de ser igual à ordem na lista_unsorted. Logo, copiamos a lista unsorted para a envp_char.
	i = 0;
	while (tmp)
	{
		if (tmp->visible)
		{
			env = ft_strjoin(tmp->key, "=");
			shell->envp_char[i++] = ft_strjoin(env, tmp->value);
			free(env);
		}
		tmp = tmp->next;
	}
}

// Função para ir buscar os valores da key e do value do envp e adicionar
//à lista ligada de variáveis de ambiente. A segunda vez que é chamada para ir buscar o value, o primeiro argumento é NULL e o segundo o valor do =, que entra como delimitador na função strchr. Pq chamamos com Nulo? Pq não queremos que ela entre no primeiro if. Porquê? Pq queremos que o valor do buffer (o que está lá dentro) seja o mesmo, para que possamos agora ir buscar a info dps do igual (uma vez que a primeira vez que rodamos esta função fomos buscar o que estava antes...o valor da key).
static char	*ft_strtok(char *str, const char *delim)
{
	static char	*buffer;
	char		*token;
	char		*ps;

	if (str)
		buffer = str;
	if (!buffer)
		return (NULL);
	token = buffer; // token é igual ao buffer, ou seja, token é igual ao que está no buffer. Por exemplo Key = value.
	ps = buffer; // ps é igual ao buffer. Porquê? Para que possamos correr o ciclo while e ir buscar o que está dps do =.
	while (*ps && !ft_strchr(delim, *ps)) // Enquanto ps não for nulo e o que está em ps não for igual a nenhum dos delimitadores, continuamos a correr o ciclo.
		ps++;
	if (*ps) // Se o que está em ps for igual a um dos delimitadores (neste caso o =), então anda pa frente, para que da segunda vez que corre o loop estar após o = (de forma a ir buscar o valor de value)...
	{
		*ps = 0; // ...o que está em ps passa a ser nulo...Porquê? Para que o token seja apenas o que está antes do delimitador. Ou seja, o que está antes do =. O que está dps do = é o value, o qual vamos buscar na segunda vez que corremos a função.
		buffer = ps + 1; // ...e o buffer passa a ser o que está em ps + 1, ou seja, o que está dps do delimitador. Qd queremos ir buscar o valor de value, o valor do buffer andou para a frente e agora está dps do =.
	}
	else
		buffer = NULL;
	return (token); // Retornamos o token, que é igual ao buffer. Da primeira vez que passamos é o que está antes do =, da segunda vez é o que está dps do =.
}

// Função para converter o envp em uma lista ligada de variáveis de ambiente.
// shell->env_list_unsorted Cria a linked list por ordem de criação
// através da função add_node_to_env_list.
void	convert_envp_to_linked_lists(char **envp, t_shell *shell)
{
	int		i;
	char	*key;
	char	*value;

	i = 0;
	while (envp[i])
	{
		key = ft_strtok(envp[i], "=");
		value = ft_strtok(NULL, "=");
		create_update_envp_lists(shell, key, value, 1); // Função que cria as duas listas ligadas de variáveis de ambiente: a unsorted_list e a sorted_list.
		i++;
	}
}
