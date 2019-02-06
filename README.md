# strace
g++ -c strace.cpp -I.
g++ -g strace_test.cpp strace.o -ldl -I.
