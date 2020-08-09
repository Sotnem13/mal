
all: step0_repl step1_read_print

CPPFLAGS= -std=c++17

step0_repl: step0_repl.cpp
	g++ $(CPPFLAGS)  step0_repl.cpp -o step0_repl

step1_read_print: step1_read_print.cpp
		g++ $(CPPFLAGS) step1_read_print.cpp -o step1_read_print


clean:
	rm step0_repl step1_read_print -f
