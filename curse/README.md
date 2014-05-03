# Learn text-mode program by curse / Python-2.7 under 30 min

We will introduce how to implement text-mode program by curse, and
except you can learn this skill in 30 minute. We will give you some
targets and try to write a POC to meet these scenario.


## Scenario 1
	Build a window which has border, show any key press on window
	and accept Q/q to quit this progrm.


In this scenraio, we need to initial our project and construct a
basic workspace to contain all components in this box. So we build
a class called *CLI* which run `curses.initscr()` to initial curse
and then set border by `box()`.

Then run `loopMain` as a listener to accept user's input, this will
capture user's input one by one (in ASCII code). When receive a
characture, we will parse it and show on the box we created. Using
`addstr` by win instance (we create to contain the message) to show
on the screen. Also, we need to `refresh` to force show data into
screen.

Conclusion:
1. Initial your curses by `initscr`.
2. Setup your curses for `echo`, `curs_set` ... etc.
3. Update your screen by `refresh`.
4. Receive user's input by `getch`.
5. Clean-up all setting for your curses, otherwise your bash will be polluted.


## Scenario 2
	Build a window which have three panels. You can switch to each one via
	arrow key. And more, only one panel can type and show messages you
	input when this panel is focused.


In this scenraio, we need to create three panels and each panel controls their
background color. We can use `curses.init_pair` to setup the color-pair, and
it uses the alias number which you defined to setup the background color.
Before setup the background, we need to create three panels via `curses.win`
which return a panel object (exactly, it is window object) and we can setup
the background color.

After create the necessary widget, we need to control our logic via `getch`
which return each KeyCode we press. If we press the arrow keys, program will
resketch the screen and program will cache which panel is focused and display
the background color. When the footer bar is focused, it will enable user
to type any string via we cache the string and shows the string.

Conclusion:
1. Create three panel via `curses.win`
2. Set the background via `bkgd` which setup color pair by `curses.init_pair`
3. Control logic by `getch` and resketch screen

