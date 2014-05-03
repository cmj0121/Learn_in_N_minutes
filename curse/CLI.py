#! /usr/bin/env python
#! coding: utf-8

__author__ = "cmj"
__copyright__ = "Copyright (C) 2014-2014. All Rights Reserved."

import curses
import string

class CLI(object):
	""" Text-mode pseudo class designed by curse

	>>> cli = CLI(border=True)

	>>> cli.workspace()
	"""
	def __init__(self, border=False, cursor=True, keypad=1):
		scr = curses.initscr()
		curses.noecho()

		if border: scr.box()
		curses.curs_set(cursor)
		scr.keypad(keypad)
		scr.refresh()
		curses.mousemask(curses.ALL_MOUSE_EVENTS)
		self.msg = ""

		self.scr = scr
	def __del__(self):
		self.scr.keypad(0)
	def buildPanel(self, frame=0, border=False):
		""" Show the three windows as
			+----+--------------------+
			|    |                    |
			|    |                    |
			|    |         2          |
			| 1  |                    |
			|    |                    |
			|    |                    |
			|    +--------------------+
			|    |         3          |
			+----+--------------------+
		"""
		y, x = self.scr.getmaxyx()

		## Left Slidebar, width = ScreenWidth*0.2
		leftSlideBar = curses.newwin(y, int(x*0.2), 0, 0)
		## WorkSpace, height = ScreenHeight*0.8
		workSpace = curses.newwin(int(y*0.8), x-int(x*0.2), 0, int(x*0.2))
		## FootBar
		footBar = curses.newwin(y-int(y*0.8), x-int(x*0.2), int(y*0.8), int(x*0.2))

		ret = (leftSlideBar, workSpace, footBar)
		curses.start_color()
		curses.init_pair(1, curses.COLOR_BLACK, curses.COLOR_BLUE)
		curses.init_pair(2, curses.COLOR_BLACK, curses.COLOR_YELLOW)
		curses.init_pair(3, curses.COLOR_BLACK, curses.COLOR_GREEN)
		ret[frame].bkgd(curses.color_pair(frame+1))

		[_.box() for _ in ret]
		[ret[_].addch(1, 1, str(_)) for _ in range(len(ret))]
		footBar.addstr(" %s "%self.msg)
		[_.refresh() for _ in ret]
		return ret
	def loopMain(self, pos=(8, 10)):
		cur = 3
		_, main, _ = self.buildPanel(cur%3)
		while True:
			ch = self.scr.getch()
			if 0x71 == (ch | 0x20): break
			elif curses.KEY_UP == ch:
				if cur == 2: cur = 1
				elif 0 == cur%3: cur = 3
			elif curses.KEY_DOWN == ch:
				if cur == 1: cur = 2
				elif 0 == cur%3: cur = 6
			elif curses.KEY_RIGHT == ch:
				if 0 == cur%3: cur = cur/3
			elif curses.KEY_LEFT == ch:
				if 0 != cur%3: cur = cur*3
			elif 2 == cur%3:
				if 0x7F == ch: self.msg = self.msg[:-1]
				else: self.msg =  self.msg + chr(ch)
			_, main, _ = self.buildPanel(cur%3)
			main.refresh()

if __name__ == "__main__":
	try:
		cli = CLI(border=False, cursor=False)
		cli.loopMain()
	except Exception as e:
		print e
		del cli
	except KeyboardInterrupt:
		del cli
		pass
	finally:
		curses.endwin()
