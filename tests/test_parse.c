#include <criterion/criterion.h>
#include "parser.h"

Test(test_parse, test_parse)
{
	t_list	*token_list;
	t_list	*head;

	token_list = tokenize_line("ls -l");
	head = token_list;
	cr_assert_not_null(head, "tokenize failed");

	cr_assert_str_eq(((t_token *)(head->content))->value, "ls", "token名が正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->len, 2, "token_lenが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->capacity, INITIAL_TOKEN_BUF_SIZE, "token_capacityが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->type, TOKEN_WORD, "token_typeが正しく解析されていない");
	head = head->next;
	cr_assert_str_eq(((t_token *)(head->content))->value, "-l", "token名が正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->len, 2, "token_lenが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->capacity, INITIAL_TOKEN_BUF_SIZE, "token_capacityが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->type, TOKEN_WORD, "token_typeが正しく解析されていない");
	ft_lstclear(&token_list, free_token);

	token_list = tokenize_line("cat <in | echo \"$PATHsss\'uuu\'sss\"");
	head = token_list;
	cr_assert_not_null(head, "tokenize failed");

	cr_assert_str_eq(((t_token *)(head->content))->value, "cat", "token名が正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->len, 3, "token_lenが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->capacity, INITIAL_TOKEN_BUF_SIZE, "token_capacityが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->type, TOKEN_WORD, "token_typeが正しく解析されていない");
	head = head->next;
	cr_assert_str_eq(((t_token *)(head->content))->value, "<", "token名が正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->len, 1, "token_lenが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->capacity, INITIAL_TOKEN_BUF_SIZE, "token_capacityが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->type, TOKEN_REDIR_IN, "token_typeが正しく解析されていない");
	head = head->next;
	cr_assert_str_eq(((t_token *)(head->content))->value, "in", "token名が正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->len, 2, "token_lenが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->capacity, INITIAL_TOKEN_BUF_SIZE, "token_capacityが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->type, TOKEN_WORD, "token_typeが正しく解析されていない");
	head = head->next;
	cr_assert_str_eq(((t_token *)(head->content))->value, "|", "token名が正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->len, 1, "token_lenが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->capacity, INITIAL_TOKEN_BUF_SIZE, "token_capacityが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->type, TOKEN_PIPE, "token_typeが正しく解析されていない");
	head = head->next;
	cr_assert_str_eq(((t_token *)(head->content))->value, "echo", "token名が正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->len, 4, "token_lenが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->capacity, INITIAL_TOKEN_BUF_SIZE, "token_capacityが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->type, TOKEN_WORD, "token_typeが正しく解析されていない");
	head = head->next;
	cr_assert_str_eq(((t_token *)(head->content))->value, "\"$PATHsss\'uuu\'sss\"", "token名が正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->len, 18, "token_lenが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->capacity, INITIAL_TOKEN_BUF_SIZE, "token_capacityが正しく解析されていない");
	cr_assert_int_eq(((t_token *)(head->content))->type, TOKEN_WORD, "token_typeが正しく解析されていない");
	ft_lstclear(&token_list, free_token);
}
