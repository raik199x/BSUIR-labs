In my opinion this is very hard lab,
 because you need to learn how [shared memory](https://www.youtube.com/watch?v=WgVSq-sgHOc)
  + [semaphors](https://www.youtube.com/watch?v=ukM_zzrIeXs&t) works...

---

Every section of circle create in a shared memory, so struct Circle have such fields as
  * current_shmid 
  * right_shmid
  * left_shmid

All of this replace pointers (that must be in a normal circle), using them our program can move in circle.

We also have  3 semaphors (actually i really used only one, other just for vision), so if we are entering in a danger zone,
we use SemMutex to block access to Circle.

---

Producer finds Circle block without message and randoms size, than randoms letters and in the end calculates checksum for this line.

Taker finds Circle block with message and calculate cheksum, if stored checksum equls that we've already calculated we simply output message, otherwise we ~~leave error and clear block~~ send message that checksum is corrupted.

---

To watch how producers running through shared memory open terminal and write `watch ipcs`, your shared memory will have 0644 perms,
 nattch shows how many objects currently using this piece of memory.

 ---

Thanks to [Maksim](https://github.com/postavtezachet) for passing this road with me.
