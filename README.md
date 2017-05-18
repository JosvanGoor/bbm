# Bomberman

### NOTES
Gamerooms should just pass the controls to the eventsync so that it can keep the controllers up to date.
Controller copy protection.
JE LEAKT ERGENS 1 POINTER.

### valgrind report
Valgrind currently reports on memory leaks, these are originating in the X11 and SDL2 frameworks and cannot be handeled.
Should it look different then below the leaks are introduced by me:

==5712== HEAP SUMMARY:  
==5712==     in use at exit: 45,685 bytes in 507 blocks  
==5712==   total heap usage: 25,846 allocs, 25,339 frees, 269,420,290 bytes allocated  
==5712==   
==5712== LEAK SUMMARY:  
==5712==    definitely lost: 225 bytes in 4 blocks  
==5712==    indirectly lost: 1,728 bytes in 38 blocks  
==5712==      possibly lost: 0 bytes in 0 blocks  
==5712==    still reachable: 43,732 bytes in 465 blocks  
==5712==         suppressed: 0 bytes in 0 blocks  
==5712== Rerun with --leak-check=full to see details of leaked memory  
==5712==   
==5712== For counts of detected and suppressed errors, rerun with: -v  
==5712== ERROR SUMMARY: 4 errors from 2 contexts (suppressed: 0 from 0)  
