import builtins
import sys
import os
import gc
#gc.disable()

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

def DispatchTouch(pEnt, other):
	pEnt.touch(other)

def DispatchUse(pEnt, other):
	pEnt.use(other)

def DispathThink(pEnt):
	pEnt.think()

def PlayerPostThink(pPlayer):
	try:
		eng.MakeVector(pPlayer.pev.v_angle)
		pEnt = pPlayer
		pClose = None
		flMaxDot = 0.7

		while True:
			ent = eng.FindEntityInSphere(pEnt.edict, pPlayer.pev.origin, 64.0)

			if not ent:
				break

			pEnt = ENT(ent)
			absmin = Vector(*pEnt.pev.absmin)
			size = Vector(*pEnt.pev.size)
			origin = Vector(*pPlayer.pev.origin)
			view_ofs = Vector(*pPlayer.pev.view_ofs)

			vecLOS = UTIL_ClampVectorToBox((absmin + (size*0.5)) - (origin + view_ofs), size*0.5)
			flDot = DotProduct(vecLOS , Vector(*gpGlobals.v_forward))

			if(flDot > flMaxDot):
				pClose = pEnt
				flMaxDot = flDot
				
		if pClose and pClose.pev.classname.startswith('@'):
			pClose = LINKED_ENTS[pClose.edict[0]] if pClose.edict[0] in LINKED_ENTS else pClose
			if (pPlayer.pev.button & IN_USE):
				pClose.use(pPlayer.edict)


	except Exception as e:
		ALERT(at_console, e)

def RunLibFunc(etype, args):
	ent = args[0]

	if 'dispatch' in etype:
		try:
			func = etype.split('_')[1]
			pEnt = LINKED_ENTS[ent[0]] if ent[0] in LINKED_ENTS else None

			if pEnt:
				if not pEnt.is_valid():
					return LINKED_ENTS.pop(pEnt.edict[0])

				if func == 'touch':
					DispatchTouch(pEnt, *args[1:])	
				elif func == 'use':
					DispatchUse(pEnt, *args[1:])
				elif func == 'collbox':
					pass
				elif func == 'think':
					DispathThink(pEnt)
				else:
					ALERT(at_error, func, "not implemented")
		except Exception as e:
			ALERT(at_error, e)
	elif etype == 'pPostThink':
		PlayerPostThink(ENT(ent))


SET_GLOBAL('SET_GLOBAL', SET_GLOBAL)
SET_GLOBAL('GET_GLOBAL', GET_GLOBAL)
SET_GLOBAL('GetCmdFunc', GetCmdFunc)
SET_GLOBAL('RunMsgHandlers', RunMsgHandlers)
SET_GLOBAL('RunLibFunc', RunLibFunc)

import utils

load_plugins()