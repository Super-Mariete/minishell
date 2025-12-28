#include <stdio.h>
#include <unistd.h>

int	main()
{
	static char	s[40];

	s = "HOLA\n";
	printf("%s\n", s);
}
