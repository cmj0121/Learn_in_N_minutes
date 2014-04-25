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
	def __init__(self, border=False, cursor=True):
		scr = curses.initscr()
		curses.noecho()

		if border: scr.box()
		curses.curs_set(cursor)
		scr.refresh()

		self.scr = scr
	def loopMain(self, pos=(8, 10)):
		win = curses.newwin(3, 20, pos[0], pos[1])
		win.box()
		win.refresh()
		while True:
			ch = self.scr.getch()
			if 0x71 == (ch | 0x20): break

			msg = "Key: %s" %ch
			if chr(ch) in string.printable: msg += " (%s)" %chr(ch)
			win.clear()
			win.box()
			win.addstr(1, 1, msg)
			win.refresh()
if __name__ == "__main__":
	try:
		cli = CLI(border=True, cursor=False)
		cli.loopMain()
	except Exception as e:
		print e
		del cli
	except KeyboardInterrupt:
		del cli
		pass
	finally:
		curses.endwin()
