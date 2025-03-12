CC = gcc
LIBFT_DIR	=	../libft
CFLAGS = -Wall -Wextra -I../includes -I$(LIBFT_DIR) -fprofile-arcs -ftest-coverage
LD_FLAGS	=	-L$(LIBFT_DIR)
LDFLAGS = -lft -lcriterion -lgcov --coverage -lreadline

SOURCES = test_parse.c ../src/parser/tokenizer.c \
			../src/parser/tokenizer_state_handler_normal.c \
			../src/parser/tokenizer_state_handler_meta_character.c

TARGET = run_tests

all: $(TARGET)

$(TARGET): $(SOURCES) $(LIBFT_DIR)/libft.a
	$(CC) $(CFLAGS) -o $@ $^ $(LD_FLAGS) $(LDFLAGS)

$(LIBFT_DIR)/libft.a: $(LIBFT_DIR)
	$(MAKE) complete -C $(LIBFT_DIR)

clean:
	rm -f $(TARGET) *.gcda *.gcno

run: $(TARGET)
	./$(TARGET)

coverage: run
	lcov --capture --directory . --directory ../src --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report
