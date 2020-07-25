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
	handlers = [h for h in MSG_EVENTS if (MSG(h.msg) if isinstance(h.msg, str) else h.msg) == index]

	for h in handlers:
		h.func(data)

def DispatchTouch(ent, pEnt, other):
	if pEnt:
		pEnt[0].touch(other)
	else:
		ENT(ent).touch(other)

def DispatchUse(ent, pEnt, other):
	if pEnt:
		pEnt[0].use(other)
	else:
		ENT(ent).use(other)

def DispathThink(ent, pEnt):
	if pEnt:
		pEnt[0].think()
	else:
		ENT(ent).think()

def RunDispatch(type, args):
	ent = args[0]
	pEnt = [e for e in LINKED_ENTS if e.edict == ent]

	if pEnt and not pEnt[0].is_valid():
		return LINKED_ENTS.remove(pEnt[0])

	if type == 'touch':
		DispatchTouch(ent, pEnt, *args[1:])	
	elif type == 'use':
		DispatchUse(ent, pEnt, *args[1:])
	elif type == 'collbox':
		pass
	elif type == 'think':
		DispathThink(ent, pEnt)
	else:
		ALERT(at_error, type, "not implemented")

SET_GLOBAL('SET_GLOBAL', SET_GLOBAL)
SET_GLOBAL('GET_GLOBAL', GET_GLOBAL)
SET_GLOBAL('GetCmdFunc', GetCmdFunc)
SET_GLOBAL('RunMsgHandlers', RunMsgHandlers)
SET_GLOBAL('RunDispatch', RunDispatch)

import utils

load_plugins()