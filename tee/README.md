Runs an (arguably way better) version of tee without
the hangs of the original included in most linux 
distributions. Run -h for help or see the man page 
for tee - this program behaves exactly the same
but is implemented in a much more user friendly
way.

Program is compiled with apue.h error handling. 
As the full apue file structure is over the 100 
file limit size to include, I've compiled for you.
However, if you find apue and want to compile, do 
so with

gcc -g -Wall -I <path_to_apue.3e>/include -L <path_to_apue.3e>/lib tee.c -lapue -o tee
