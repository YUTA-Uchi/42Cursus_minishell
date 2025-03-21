TARGET		:=	run_tests
SRCS		:=	tokenizer.c \
				tokenizer_state_handler_normal.c \
				tokenizer_state_handler_meta_character.c \
				error_handler.c \

TESTS_SRCS	:=	test_parse.c 

CC			:=	cc
LIBFT_DIR	:=	../libft
CFLAGS		:=	-Wall -Wextra -I../includes -I$(LIBFT_DIR) -fprofile-arcs -ftest-coverage
LD_FLAGS	:=	-L$(LIBFT_DIR)
LDFLAGS		:=	-lft -lcriterion -lgcov --coverage -lreadline

vpath %.c ../srcs/parser:../srcs/error_handler:

all: $(TARGET)

$(TARGET): $(SRCS) $(TESTS_SRCS) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) -o $@ $^ $(LD_FLAGS) $(LDFLAGS)

$(LIBFT_DIR)/libft.a: $(LIBFT_DIR)
	$(MAKE) complete -C $(LIBFT_DIR)

clean:
	rm -f $(TARGET) *.gcda *.gcno

run: $(TARGET)
	./$(TARGET) --xml=test_results.xml

coverage: run
	lcov --capture --directory . --directory ../obj --output-file coverage.info --ignore-errors mismatch --ignore-errors empty
	genhtml coverage.info --output-directory coverage_report
