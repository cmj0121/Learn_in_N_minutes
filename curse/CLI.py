#! /usr/bin/env python
#! coding: utf-8

__author__ = "cmj"
__copyright__ = "Copyright (C) 2014-2014. All Rights Reserved."

from jj.utils import Logs
from jj.utils import TimerFD, iTimerSpec, TimeSpec, TimerFile
import curses
import string
import signal
import sys
import select

LogFile = "./Exception.log"
RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE = range(1, 8)
class CLI(object):
	""" Text-mode pseudo class designed by curse

	>>> cli = CLI(border=True)
	"""
	@Logs(LogFile)
	def __init__(self, cursor=True, keypad=1):
		scr = curses.initscr()
		curses.noecho()

		# Set curses environment
		curses.curs_set(cursor)
		scr.keypad(keypad)
		curses.mousemask(curses.ALL_MOUSE_EVENTS)
		self.color()

		## Resize if we capture signal
		signal.signal(signal.SIGWINCH, self.resize)
		self.scr = scr
		self.closed, self.status = False, ""
		self.mainPanel
	def __del__(self):
		self.scr.keypad(0)
		self.closed = True

	def resize(self, nr=0, frame=None):
		self.mainPanel
	def color(self):
		curses.start_color()
		curses.use_default_colors()
		curses.init_pair(RED, curses.COLOR_RED, 0)
		curses.init_pair(GREEN, curses.COLOR_GREEN, 0)
		curses.init_pair(YELLOW, curses.COLOR_YELLOW, 0)
		curses.init_pair(BLUE, curses.COLOR_BLUE, 0)
		curses.init_pair(MAGENTA, curses.COLOR_MAGENTA, 0)
		curses.init_pair(CYAN, curses.COLOR_CYAN, 0)
		curses.init_pair(WHITE, curses.COLOR_WHITE, 0)

	@property
	def size(self):
		""" Get current windows size """
		def IOCTL(fd):
			try:
				from struct import unpack
				from termios import TIOCGWINSZ
				from fcntl import ioctl

				return unpack("hh", ioctl(fd, TIOCGWINSZ, "1234"))
			except Exception as e:
				return
		y, x = IOCTL(0) or IOCTL(1) or IOCTL(2)
		return y, x
	@property
	def mainPanel(self, nr=0, frame=0):
		""" 
			+-----+-------------------------------+
			|     |                               |
			|     |                               |
			|     |                               |
			|Menu |     Show Message / Work Space |
			|     |                               |
			|  Bar|                               |
			|     |                               |
			|     |                               |
			|     +-------------------------------+
			|     |       Command Line            |
			+-----+-------------------------------+
			| [ID/Time]    Status Bar             |
			+-------------------------------------+
		"""
		y, x = self.size

		if hasattr(self, "win"):
			win = self.win
			win.clear()
		else:
			win = curses.newwin(y, x)
			self.win = win

		## Enable Color
		curses.start_color()
		curses.use_default_colors()

		## Draw the dividing line
		win.hline(y-3, 0, '-', x, curses.color_pair(CYAN))
		win.hline(y-5, 8, '-', x-8, curses.color_pair(CYAN))
		win.vline(0, 8, '|', y-3, curses.color_pair(CYAN))
		win.refresh()

		self.statusBar = ""
		self.cmdLineBar
		return win

	@property
	def statusBar(self, size=(), pos=()):
		if not size: size = (2, self.size[1])
		if not pos:  pos  = (self.size[0]-2, 0)

		parm = size + pos
		bar = curses.newwin(*parm)
		bar.refresh()

		self.cmdLineBar = ""
		return bar
	@statusBar.setter
	def statusBar(self, msg, pos=(0, 2)):
		""" Update the status when set message """
		import time, getpass, os

		self.status, bar = msg if isinstance(msg, str) else self.status, self.statusBar
		bar.move(*pos)

		timeFMT ="%H:%M:%S"
		parm = {"time":	time.strftime(timeFMT),
				"id":	getpass.getuser(),
				"uid":	os.getuid(),
				"msg":	self.status}


		bar.addstr("[{id} (".format(**parm), curses.color_pair(CYAN))
		bar.addstr("{uid}".format(**parm))
		bar.addstr(")", curses.color_pair(CYAN))
		bar.addstr(" {time}".format(**parm), curses.color_pair(YELLOW))
		bar.addstr("]".format(**parm), curses.color_pair(CYAN))
		bar.addstr(" %s" %self.status)
		bar.refresh()

		self.cmdLineBar = ""
	@property
	def cmdLineBar(self, size=(), pos=()):
		if not size: size = (1, self.size[1])
		if not pos:  pos  = (self.size[0]-4, 9)

		parm = size + pos
		bar = curses.newwin(*parm)

		if not hasattr(self, "_cmd_"): self._cmd_ = ""
		bar.bkgd(curses.color_pair(2))
		bar.move(0, 1)
		bar.refresh()
		return bar
	@cmdLineBar.setter
	def cmdLineBar(self, msg):
		bar = self.cmdLineBar

		if msg == "\r":	# Submit
			self._cmd_ = ""
		elif msg == "\x7f":	# Delete
			self._cmd_ = self._cmd_[:-1]
		else:
			self._cmd_ += "".join([hex(ord(_)) for _ in msg])

		bar.clear()
		bar.move(0, 1)
		bar.addstr(self._cmd_)
		bar.refresh()

	@Logs(LogFile)
	def mainLoop(self, timeout=0):
		""" Handle all operation by select which support non-block IO """

		RDIOs = [sys.stdin, TimerFD().getTimer()]
		while not self.closed:
			try:
				rds, _, _ = select.select(RDIOs, [], [], timeout)
			except select.error as e:
				## We skip the interrupt error when run select
				import errno
				if e[0] != errno.EINTR: raise
			## Handle Read Parts
			for fd in rds:
				if fd is sys.stdin:
					ch = fd.read(1)
					if ch == "\x1b":	# Excape
						ch += fd.read(2)
						while ch[-1].isdigit():
							ch += fd.read(2)
					if ch == "\x1b[21~": self.closed = True
					self.cmdLineBar = ch
				elif isinstance(fd, TimerFile):
					fd.read()
					self.statusBar = None
				else:
					raise TypeError("%s" %(type(fd)))

if __name__ == "__main__":
	try:
		cli = CLI()
		cli.mainLoop()
	except Exception as e:
		del cli
	except KeyboardInterrupt:
		del cli
	finally:
		curses.endwin()
