# Lab 4

[Interesting info](https://developer.nvidia.com/gpugems/gpugems3/part-vi-gpu-computing/chapter-37-efficient-random-number-generation-and-application)

@ Dmitriy Pertsev

## Important note

2023 - this is experemental task and we dunno if the conditions stays the same

---

## Condition

check pdf

## Notes

Out program is not ideal (yes, it calculates default values right), because if you change default values our program may have unexpected behaviour, also it's have lack of comments so probably you wount understand what is going on.

Also, in cuda kernel we used atomic add insteat of parralel reduce (even though in the condition with must use second), because our algorithm can't follow all rules of the written condition, we just decided to do it the way we like.
