# !!CAUTION!!

Usefull information will be covered here, so if you are looking for answers, you should read this readme

---

## MAKEFILE

strange thing was found: if you compile programs using gcc with `--std=c11` params you probably will get an error, here is actually two solve, first was given by Podenok

> In c11 developers get rid of all function that doesn have connection to C (including number p (3,14)), so you should connect deleted library using define @Podenok

In current situation they ~~for some reason~~ throw out sigaction function.
My solution was much easier ~~delete param c11~~ replace c11 with `--std=gnu11` and it will work properly

---

## child.c (and combination of {0,0}->{1,1}

The task is not so understandable as at could be and a lot of questions coming from task to changing stat. My teacher said:

> Condition is unclear, because we can thing that we should displat 4 possible combinations ({0,0} {1,0} {0,1} {1,1}), but if we check the lecture, we find out that Podenok means that program should show only two possible combinations {0,0} {1,1}. @Basak

So, in my program i change one variable per second, **it doesnt means that my version of program is correct**, if your teacher asks to do as Podenok says, rewrite child.c

---

## functions.c

Suddenly, i decided to write my "small lib" for C, so it will always appear in future (but i dont know if i should send it :p), from this file you need only two functions `enterline , parseintend`

Nothing more, good luck!
