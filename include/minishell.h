/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaosilva <joaosilva@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 15:14:45 by crocha-s          #+#    #+#             */
/*   Updated: 2024/06/06 22:46:17 by joaosilva        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
/*# include <linux/limits.h>*/
# include <dirent.h>
# include <errno.h>
# include <limits.h>
# include <ncurses.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <term.h>

# define PROMPT_BEGIN "┎─── "
# define PROMPT_END "\n┖─❯ "

# define STOP 0
# define CONTINUE 1
# define RESTORE 2

# define EXEC 300
# define REDIR 301
# define PIPE 302
# define HERE_DOC 303
# define APPEND 304

# define MAXARGS 50

# define SPACES " \t\n\v\f\r"
# define OPERATORS "|><"
# define NOT_EXP "|></ \t\n\v\f\r"
# define OPANDSP "|>< \t\n\v\f\r"

# define ERROR_TITLE "minishell: "
# define ERROR_QUOTE "unclosed quotes"
# define ERROR_SINTAX "syntax error"
# define ERROR_HERE_DOC "unexpected EOF while looking for matching `"

# define SIGRESTORE 1
# define SIGHEREDOC 2
# define SIGCHILD 3
# define SIGIGNORE 4

extern int			g_exit;

typedef struct s_cmd
{
	int				type;
}					t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				visible;
	int				index;
	struct s_env	*next;
}					t_env;

typedef struct s_shell
{
	char			*line;
	char			*prompt;
	char			*ps;
	char			*es;
	int				line_len;
	t_env			*env_list_unsorted;
	t_env			*env_list_sorted;
	t_cmd			*cmd;
	int				status;
	int				exec_cmd;
	int				envp_size;
	int				pid;
	char			**envp_char;
}					t_shell;

typedef struct s_exec
{
	int				type;
	char			*argv[MAXARGS];
}					t_exec;

typedef struct s_redir
{
	int				type;
	t_cmd			*cmd;
	char			*file;
	int				mode;
	int				fd;
}					t_redir;

typedef struct s_here
{
	int				type;
	t_cmd			*cmd;
	char			*eof;
	int				mode;
	int				fdin;
	int				fdout;
}					t_here;

typedef struct s_pipe
{
	int				type;
	t_cmd			*left;
	t_cmd			*right;
}					t_pipe;

// process_line file
int					process_line(t_shell *shell);
int					inside_quotes(t_shell *shell);

// envp1 file - convert
void				convert_envp_to_linked_lists(char **envp, t_shell *shell);
void				convert_envp_to_char(t_shell *shell);

// envp2 file - add
void				create_update_envp_lists(t_shell *shell, char *key,
						char *value, int visible);

// envp3 file - export/get/print
void				env_export(t_shell *shell, char *key, char *value,
						int visible);
char				*env_get(char *key, t_shell *shell);
void				envp_print(t_shell *shell);

// envp4 file - modify
bool				env_mod(t_shell *shell, char *target, char *new_value);

// envp5 file - rm
void				env_rm(char *key, t_shell *shell);
void				ft_envlstdelone(t_env *lst, void (*del)(void *));

// envp6 file - clear
void				ft_envlstclear(t_env *lst, void (*del)(void *));

// Parser
t_cmd				*exec_cmd(void);
t_cmd				*pipe_cmd(t_cmd *left, t_cmd *right);
t_cmd				*redir_cmd(t_cmd *cmd, char *file, int mode, int fd);
int					parse_cmd(t_shell *shell);
int					peek(t_shell *shell, char *op);
int					gettoken(t_shell *sh, char **token);
t_cmd				*parsepipe(t_shell *shell);

// Expand
void				expand_arg(t_shell *shell, char **arg);
int					expand_free(char *key, int i, int j, char **line);
int					expand(char *key, int i, int j, char **line);
void				trim_quotes(char *arg, int *len);
void				trim_arg(char *arg);

// error_frees
int					print_error_syntax(t_shell *shell, char *msg, int exit);
int					print_error(t_shell *shell, char *msg, char *msg2,
						int exit);
void				free_exit(t_shell *shell);
int					error_inside(t_shell *shell, char *cmd, char *arg,
						int error_code);
void				free_cmd(t_cmd *cmd);

// run_cmd
void				run_cmd(t_shell *shell, t_cmd *cmd);
void				run_exec(t_shell *shell, t_exec *cmd);
void				run_redir(t_shell *shell, t_redir *cmd);
void				run_pipe(t_shell *shell, t_pipe *cmd);
void				check(int result, char *msg, int exit);
int					check_fork(void);
t_cmd				*here_cmd(t_cmd *cmd, char *eof);
void				run_heredoc(t_shell *shell, t_here *here);

// built-in
int					run_builtin(t_shell *shell, t_exec *cmd);
void				ms_echo(t_exec *cmd);
void				ms_cd(t_shell *shell, t_exec *cmd);
bool				ms_chdir(t_shell *shell, char *path);
void				ms_pwd(t_shell *shell, t_exec *cmd);
void				ms_export(t_shell *shell, t_exec *cmd);
void				ms_unset(t_shell *shell, t_exec *cmd);
void				ms_env(t_shell *shell, t_exec *cmd);
void				ms_exit(t_shell *shell, t_exec *cmd);

void				wait_children(t_shell *shell);
void				signal_handler(int sig);
// void	close_fds_and_sig_handler(int fd[2], int sig);
#endif