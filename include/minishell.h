/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: crocha-s <crocha-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:14:45 by crocha-s          #+#    #+#             */
/*   Updated: 2024/05/27 11:49:05 by crocha-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <signal.h>
# include <fcntl.h>
/*# include <linux/limits.h>*/
# include <limits.h>
# include <dirent.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <ncurses.h>
# include <term.h>

# define PROMPT_BEGIN "┎─── "
# define PROMPT_END "\n┖─❯ "

# define STOP 0
# define CONTINUE 1
# define RESTORE 2

# define EXEC 300
# define REDIR 301
# define PIPE 302
//# define AND 303
# define HERE_DOC 304
/*# define OR_OP 305*/
# define APPEND 306
//# define BLOCK 307

# define MAXARGS 50

# define SPACES " \t\n\v\f\r"
# define OPERATORS "|><"
# define NOT_EXP "|></ \t\n\v\f\r"
# define OPANDSP "|>< \t\n\v\f\r"

# define ERROR_TITLE "-minishell: "
# define ERROR_QUOTE "unclosed quotes"
# define ERROR_SINTAX "syntax error"
# define ERROR_HERE_DOC "unexpected EOF while looking for matching `"

# define SIGRESTORE 1
# define SIGHEREDOC 2
# define SIGCHILD 3
# define SIGIGNORE 4

extern int g_exit;

typedef struct s_cmd
{
   int  type;
}   t_cmd;

typedef struct s_env
{
    char            *key;
    char            *value;
    int                     visible;
    int                     index;
    struct s_env    *next;
}                           t_env;

typedef struct s_shell
{
    char    *line;
    char    *prompt;
    char    *ps;
    char    *es;
    int         line_len;
    t_env   *env_list;
	t_env   *env_list_sorted;
    t_cmd   *cmd;
    int     status;
    int     exec_cmd;
    int     envp_size;
    int     pid;
    char    **envp_char;
} t_shell;

typedef struct s_exec
{
	int		type;
	char	*argv[MAXARGS];
}			t_exec;

typedef struct s_redir
{
	int		type;
	t_cmd	*cmd;
	char	*file;
	int		mode;
	int		fd;
}			t_redir;

typedef struct s_here
{
	int		type;
	t_cmd	*cmd;
	char	*eof;
	int		mode;
	int		fdin;
	int		fdout;
}			t_here;

typedef struct s_pipe
{
	int		type;
	t_cmd	*left;
	t_cmd	*right;
}			t_pipe;

void	check(int result, char *msg, int exit);
void run_cmd(t_shell *shell, t_cmd *cmd);
int run_builtin(t_shell* shell, t_exec *cmd );
void ms_echo(t_exec *cmd);
void	ms_cd(t_shell *shell, t_exec *cmd);
bool	ms_chdir(t_shell *shell, char *path);
void	ms_pwd(t_shell *shell, t_exec *cmd);
void	ms_export(t_shell *shell, t_exec *cmd);
void	ms_unset(t_shell *shell, t_exec *cmd);
void	ms_env(t_shell *shell, t_exec *cmd);
void	ms_exit(t_shell *shell, t_exec *cmd);


int     process_line(t_shell *shell);
void	free_exit(t_shell *shell);


void	convert_envp(char **envp, t_shell *shell);
void	ft_envlstclear(t_env *lst, void (*del)(void*));
void 	convert_envp_to_char(t_shell *shell);
t_env 	*add_node_to_envp_list(t_shell *shell, char *key, char *value, int visible);
t_env	*envp_to_sort_list(t_shell *shell);
char	*env_get_value(char *key, t_shell *shell);

void 	ft_envlstdelone(t_env *lst, void (*del)(void*));
int		expand_line(char *key, int i, int j, char **line);
int		print_error_syntax(char *msg, int exit);
int	    print_error(t_shell *shell, char *msg, char *msg2, int exit);
bool	env_mod(t_shell *shell, char *target, char *new_value);
void	env_export(t_shell *shell, char *key, char *value, int visible);
void	envp_print(t_shell *shell);

void	run_cmd(t_shell *shell, t_cmd *cmd);
void	run_exec(t_shell *shell, t_exec *cmd);
void	run_redir(t_shell *shell, t_redir *cmd);
void	run_heredoc(t_shell *shell, t_here *here);
t_cmd	*here_cmd(t_cmd *cmd, char *eof);

void	expand_arg(t_shell *shell, char **arg);
int	expand_line(char *space, int i, int j, char **line);
void	run_pipe(t_shell *shell, t_pipe *cmd);
void	free_cmd(t_cmd *cmd);


int	parse_cmd(t_shell *shell);
t_cmd	*redir_cmd(t_cmd *cmd, char *file, int mode, int fd);
t_cmd	*pipe_cmd(t_cmd *left, t_cmd *right);
t_cmd	*exec_cmd(void);
void trim_arg(char *arg);
void	trim_quotes(char *arg, int *len);

t_cmd	*parsepipe(t_shell *shell);
int	peek(t_shell *shell, char *op);
int	gettoken(t_shell *sh, char **token);

#endif
