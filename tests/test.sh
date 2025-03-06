cleanup() {
	rm -f cmp out a.out
}

assert() {
  # テストしようとしている内容をprint
	printf '%-30s:' "\"$1\""

	# bashの出力をcmpに保存
	echo -n -e "$1" | bash >cmp 2>&-
	# bashのexit statusをexpectedに代入
	expected=$?
	# minishellの出力をoutに保存
	echo -n -e "$1" | ./minishell >out 2>&-
	# minishellのexit statusをactualに代入
	actual=$?

	# bashとminishellの出力を比較
	diff cmp out >/dev/null && echo -n '  diff OK' || echo -n '  diff NG'

	# bashとminishellのexit statusを比較
	if [ "$actual" = "$expected" ]; then
		echo -n '  status OK'
	else
		echo -n "  status NG, expected $expected but got $actual"
	fi
	echo
}

# Generate Executable
cat <<EOF | gcc -xc -o a.out -
#include <stdio.h>
int main() { printf("hello from a.out\n"); }
EOF

# Empty line (EOF)
assert ''
assert '/bin/echo'
assert '/bin/pwd'

assert 'pwd'
assert 'echo'
assert 'ls'
assert './a.out'

assert 'a.out'
assert 'nosuchfile'

assert 'echo hello'
assert 'echo hello world'
assert 'echo "$HOME"'
assert 'echo $HOME'
assert 'echo "sss$PATH $?"'
assert 'echo "sss$PATH $?" | cat -e'

cleanup