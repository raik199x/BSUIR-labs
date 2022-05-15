Task for this lab is in lab-9.odg

---

if you don't know how mmap works you [MUST watch this video](https://www.youtube.com/watch?v=m7E9piHcfr4), after it
everything becomes clear except one thing...

Pagesize, yep, problem is offset%sysconf(pagesize) == 0, i dunno they made this like that, but if you set wrong offset (%pagesize != 0) mmap will failed, funny (no).

In my prog i made 10 threads process all blocks of information, for every thread program creates block of data for setting up mmap,
in files i simple change every second letter to capitalized.

p.s. i recommend to comment "thread started" + "thread ended" for big files, because output to console really slows the program.

---

For generating files you still can use example given in "task", but i created python-script GenerateFile.py that randoms line, if you want to create file
~ 200-500 mb create 10000 words, if 20gb something near 90000000000 (this will take a while).
