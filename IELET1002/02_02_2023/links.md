# Links
NB! Almost all files below include a 100ms delay at the end of void loop. This is only to prevent the Tinkercad website from crashing, and has nothing to do with the examples. When used on a real Arduino, that delay should be removed.

- [Tinkercad circuit (all code below is compatible)](https://www.tinkercad.com/things/b48GpZnqZP3)
- Exercise 1: Timer
    - [a: Delay](./Exercises/1a/1a.ino)
    - [b: Delay and button](./Exercises/1b/1b.ino)
    - [c: Timer and button](./Exercises/1c/1c.ino)
    - [d: Timer with reset and button](./Exercises/1d/1d.ino)
    - [e: Inverted timer activated by button](./Exercises/1e/1e.ino)
    - [f: Nested timers](./Exercises/1f/1f.ino)
    - [g: 5 Leds activaiting one by one](./Exercises/1g/1g.ino)
- Exercise 2: Button
    - [a: Print button state](./Exercises/2a/2a.ino)
    - [b: Print once on press and release](./Exercises/2b/2b.ino)
    - [c: Print constantly while button is held down](./Exercises/2c/2c.ino)
    - [d: 3 Buttons held down](./Exercises/2d/2d.ino)
    - [e: 3 buttons on press](./Exercises/2e/2e.ino)
- Exercise 3: Control timer with button
    - [a: Print 3 seconds after button is pressed](./Exercises/3a/3a.ino)
    - [b: Print for 3 seconds after button is pressed](./Exercises/3b/3b.ino)
- Exercise 4: State machines
    - [a: Change step with button](./Exercises/4a/4a.ino)
    - [b: Looping state machine](./Exercises/4b/4b.ino)
    - [c: Run code only once per step](./Exercises/4c/4c.ino)
    - [d: Change step with timer](./Exercises/4d/4d.ino)
    - [e: Change step with timer and button](./Exercises/4e/4e.ino)
    - [f: Change steps from inside state machine](./Exercises/4f/4f.ino)
- Exercise 5: Creating some simple structs
    - [a: Led](./Exercises/5a/5a.ino)
    - [b: Potmeter](./Exercises/5b/5b.ino)
    - [c: Converting potmeter value](./Exercises/5c/5c.ino)
    - [d: Sequence (State Machine)](./Exercises/5d/5d.ino)
    - [e: Sequence with builtin timer](./Exercises/5e/5e.ino)
    - [f: Sequence.nextOnCondition()](./Exercises/5f/5f.ino)
- Exercise 6: Menu
    - [a: Creating a simple menu](./Exercises/6a/6a.ino)
    - [b: Using enum for the menu items](./Exercises/6b/6b.ino)

# Documentation
Below is a list of the most useful functions and variables in the libraries.

NB! Remember to set pinMode for all pins in ```void setup()```.

- [Timer](./Libraries/Timer/Timer.ino)
    - Functions
        - ```timer.reset()```
            - Resets the timer so it starts counting from 0 again
        - ```timer.getElapsedTime()```
            - Returns the time elapsed since the timer was last reset
        - ```timer.isFinished(duration)```
            - Returns true after the duration has passed since the timer was last reset
- [Button](./Libraries/Button/Button.ino)
    - Functions
        - ```button.update()```
            - Must be run at the top of ```void loop()``` to update the variables below
    - Variables
        - ```button.pin```
            - The physical pin the button is connected to. Set the value for this variable in ```void setup()```
        - ```button.pulldown```
            - Set this to true in ```void setup()``` if the button is connected with a pulldown resistor
        - ```button.pressed```
            - Is true for one loop when a button press is detected
        - ```button.released```
            - Is true for one loop when a button release is detected
        - ```button.state```
            - Is true when the button is held down (same as ```digitalRead(pin)```)
- [Led](./Libraries/Led/Led.ino)
    - Functions
        - ```led.toggle()```
            - Toggles the led on and off each time the function is run
    - Variables
        - ```led.pin```
            - The physical pin the led is connected to
        - ```led.state```
            - Is true when the led is on
        - ```led.setState(nextState)```
            - Will set the ```state``` to the value specified by ```nextState```. This value will be written to the pin that the led is connected to.
- [Potmeter](./Libraries/Potmeter/Potmeter.ino)
    - Functions
        - ```potmeter.update()```
            - Must be run at the top of ```void loop()``` to update the variables below
    - Variables
        - ```potmeter.pin```
            - The physical pin the potmeter is connected to. Set the value for this variable in ```void setup()```
        - ```potmeter.value```
            - The value of the potmeter. 
        This value will be between 0 and 1023
        - ```potmeter.convertedValue```
            - The value of the potmeter mapped between 0 and 10
- [Sequence](./Libraries/Sequence/Sequence.ino)
    
    NB! Remember to paste the Timer library above the Sequence library.
    - Functions
        - ```sequence.next()```
            - Will set the sequence to the next step
        - ```sequence.previous()```
            - Will set the sequence to the previous step
        - ```sequence.goTo(newStep)```
            - Will set the sequence to the specified step
        - ```sequence.nextAfterDelay(duration)```
            - Will set the sequence to the next step after the duration has passed
        - ```sequence.nextOnCondition(condition)```
            - Will set the sequence to the next step if the condition is true
        - ```sequence.reset()```
            - Will set the sequence to step 0
    - Variables
        - ```sequence.step```
            - The current step the sequence is on. Use this in a switch statement to run code for each step.