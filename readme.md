OWoD - Dice Statistics Calculator
This is a set of Dice Rollers for the classic World Of Darkness systems, written in lazy C-style C++. Currently written for the Linux commandline.

'wdice' is a roller using a single core, while 'wdice-t' is highly optimized for threads and 8 cores, or 4 cores and hyperthreading.

Compile with 'g++ -lpthread -std=c++0x -o wdice wdice.cpp' (or wdice-t)

Commandline:
./wdice(-t) diceamount difficulty numberofrolls specialty

'specialty' can be 0 or 1 to disable or enable 10's counting for two successes.

Example:

./wdice-t 6 6 1000000 0

The threaded roller can do a hundred million complete sets of rolls in the matter of a second on the right hardware.

Statistics outputted will be average successes, percentage of succs, failures and botches, individual percentages of each success level, highest success, highest botch.