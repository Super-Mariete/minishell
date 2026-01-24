#include <unistd.h>
#include <stdio.h>

size_t  ft_strlen(const char *str)
{
	int     i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

int	quoted_len(const char *line, const char quote)
{
	int	i;

	if (!line)
		return (0);
	i = 1;
	while (line[i])
	{
		if (line[i] == quote)
			return (i + 1);
		i++;
	}
	write(2, "minishell: expected another ", 28);
	write(2, &quote, 1);
	write(2, "\n", 1);
	return (-1);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (write(2, "Invalid number of args\n", 23), 1);
	write(1, "arg = ", 6);
	write(1, argv[1], ft_strlen(argv[1]));
	write(1, "\n", 1);
	int	r = quoted_len(argv[1], argv[1][0]);
	printf("r = %d\n", r);
	return (0);
}
