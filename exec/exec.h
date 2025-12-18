#ifndef EXEC_H
#define EXEC_H

int	has_pipe(t_cli *cli);
int	apply_redirs(t_cli *cli);
int	exec_builtin(t_cli * cli);

#endif
