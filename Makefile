pickomino: *.cc
	g++ -o pickomino --std=c++11 -Wall -Wextra -Werror *.cc

test: pickomino
	echo 21 | ./pickomino
