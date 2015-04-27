pickomino: *.cc
	g++ -o pickomino --std=c++11 -O3 -Wall -Wextra -Werror *.cc

test: pickomino
	echo 1 www22 234 | ./pickomino
