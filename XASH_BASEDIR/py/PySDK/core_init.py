import builtins
import sys
import os

def SET_GLOBAL(val, var):
	setattr(builtins, val, var)
	del var

SET_GLOBAL('CL_CMDS', [])
SET_GLOBAL('MODULES_PATH', './py/modules')

def GET_GLOBAL(val):
	if hasattr(builtins, val):
		return getattr(builtins, val)

	return None


class ClientCmdHandler:
	def __init__(self, cmd, func):
		self.cmd = cmd
		self.func = func

def HandleClientCmd(cmd, func):
	CL_CMDS.append(ClientCmdHandler(cmd, func))

def load_plugins():
	sys.path.append(MODULES_PATH)

	eng.AlertMessage(at_console, '[Module Loader] Reading modules dir...\n')
	for module in os.listdir('py/modules'):
		if module[-3:] == '.py':
			try:
				__import__(module[:-3])
			except Exception as e:
				eng.AlertMessage(at_error, '[Module Loader]{}: {}\n'.format(module[:-3], e))


def GetCmdFunc(cmd):
	funcs = [h for h in CL_CMDS if h.cmd == cmd]

	if funcs:
		return funcs[0]
	else:
		return None


SET_GLOBAL('SET_GLOBAL', SET_GLOBAL)
SET_GLOBAL('GET_GLOBAL', GET_GLOBAL)
SET_GLOBAL('HandleClientCmd', HandleClientCmd)
SET_GLOBAL('GetCmdFunc', GetCmdFunc)

import utils

load_plugins()