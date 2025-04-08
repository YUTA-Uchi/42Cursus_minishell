
#include "libft.h"
typedef struct s_hoge_fuga
{
	int	hoge;
	int	fuga;
}	t_hoge_fuga;

typedef int	(*t_handler)(t_hoge_fuga *, t_list *, char);

int	handler_none(t_hoge_fuga *state, t_list *a, char c)
{
	(void)state;
	(void)a;
	(void)c;
	return (0);
}

int	handler_something(t_hoge_fuga *state, t_list *a, char c)
{
	(void)state;
	(void)a;
	(void)c;
	return (0);
}

int	handler_something2(t_hoge_fuga *state, t_list *a, char c)
{
	(void)state;
	(void)a;
	(void)c;
	return (0);
}

int	handler_something3(t_hoge_fuga *state, t_list *a, char c)
{
	(void)state;
	(void)a;
	(void)c;
	return (0);
}

int	handler_something4(t_hoge_fuga *state, t_list *a, char c)
{
	(void)state;
	(void)a;
	(void)c;
	return (0);
}

int	handler_something5(t_hoge_fuga *state, t_list *a, char c)
{
	(void)state;
	(void)a;
	(void)c;
	return (0);
}

int	handler_something6(t_hoge_fuga *state, t_list *a, char c)
{
	(void)state;
	(void)a;
	(void)c;
	return (0);
}

int handle(t_hoge_fuga *state, t_list *a, char c)
{
	static const t_handler	handlers[4] = {
		handler_none,
		handler_something,
		handler_something2,
		handler_something3
	};
	return (handlers[0](state, a, c));
}

int	main(int argc, char**argv)
{
	(void)argc;
	(void)argv;
	return (handle(0, 0, 0));
}
