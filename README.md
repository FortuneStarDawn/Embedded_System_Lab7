# Embedded_System_Lab7

Game of Memory

In this lab, you are going to develop a round-based game to test the memory of the player.

The game requires a button, a red LED, and a green LED.

In each round, the red LED is on for x seconds, where x is set randomly between 1 to 5 seconds, and then off.

The player counts the duration that the red LED is on and then presses the button for the same amount of time. 

If the difference between the button-press time and x is within a margin y seconds,

then the player wins this round and the green LED is blinked twice (0.5 second on and off).

Otherwise, the red LED is blinked 5 times (0.2 second on and off).

At the end of each round, transfer the result to the PC, including the LED on time and button-press time.

The PC displays the accumulated counts of wins and loses, and the LED on time and button-press time of the last round.
