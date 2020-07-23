import builtins
import sys
import os

def SET_GLOBAL(val, var):
	setattr(builtins, val, var)
	del var

SET_GLOBAL('CL_CMDS', [])
SET_GLOBAL('MSG_EVENTS', [])
SET_GLOBAL('MODULES_PATH', './py/modules')

def GET_GLOBAL(val):
	if hasattr(builtins, val):
		return getattr(builtins, val)

	return None


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

def RunMsgHandlers(index, data):
	handlers = [h for h in MSG_EVENTS if eng.get_msg_id(h.msg) == index]

	for h in handlers:
		h.func(data)

SET_GLOBAL('SET_GLOBAL', SET_GLOBAL)
SET_GLOBAL('GET_GLOBAL', GET_GLOBAL)
SET_GLOBAL('GetCmdFunc', GetCmdFunc)
SET_GLOBAL('RunMsgHandlers', RunMsgHandlers)

import utils

load_plugins()