#include "../minishell.h"

int	init_var(size_t *i, size_t *j, size_t *i_a, size_t *j_after)
{
	*i = 0;
	*j = 0;
	*i_a = 0;
	*j_after = 0;
	return (1);
}

int	ft_equal(size_t *j, size_t *i)
{
	*j = *j + 1;
	*i = *i + 1;
	return (1);
}

int	ft_j_s(const size_t *j_s, size_t *i_a, size_t *i, size_t *j)
{
	*i_a = *i_a + 1;
	*i = *i_a;
	*j = *j_s;
	return (1);
}