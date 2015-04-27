pickomino: *.cc *.h
	g++ -o pickomino --std=c++11 -Wall -Wextra -Werror *.cc

test: pickomino
	echo 1 www22 234 | ./pickomino
