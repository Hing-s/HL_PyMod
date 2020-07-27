import vector
import entity
import consts
import weapon

class ClientCmdHandler:
	def __init__(self, cmd, func):
		self.cmd = cmd
		self.func = func

class MsgHandler:
	def __init__(self, msg, func):
		self.msg = msg
		self.func = func

def HandleMsg(msg, func):
	MSG_EVENTS.append(MsgHandler(msg, func))

def HandleCmd(cmd, func):
	CL_CMDS.append(ClientCmdHandler(cmd, func))

class GLOBALS:
	def __getattr__(self, y):
		return eng.globals_get(y)


SET_GLOBAL('HandleCmd', HandleCmd)
SET_GLOBAL('HandleMsg', HandleMsg)
SET_GLOBAL('at_notice', 0)
SET_GLOBAL('at_console', 1)
SET_GLOBAL('at_aiconsole', 2)
SET_GLOBAL('at_warning', 3)
SET_GLOBAL('at_error', 4)
SET_GLOBAL('at_logged', 5)
SET_GLOBAL('gpGlobals', GLOBALS())

SET_GLOBAL('WRITE_BYTE', lambda x: ('byte', x))
SET_GLOBAL('WRITE_CHAR', lambda x: ('char', x))
SET_GLOBAL('WRITE_SHORT', lambda x: ('short', x))
SET_GLOBAL('WRITE_LONG', lambda x: ('long', x))
SET_GLOBAL('WRITE_STRING', lambda x: ('string', x))
SET_GLOBAL('WRITE_ANGLE', lambda x: ('angle', x))
SET_GLOBAL('WRITE_COORD', lambda x: ('coord', x))
SET_GLOBAL('WRITE_ENTITY', lambda x: ('entity', x))
SET_GLOBAL('ALERT', lambda t, *m: eng.AlertMessage(t, '{}\n'.format(' '.join([str(s) for s in m]))))
SET_GLOBAL('ENTINDEX', lambda i: ENT(eng.get_entity_by_index(i)))
SET_GLOBAL('INDEXENT', lambda e: e[0] if isinstance(e, tuple) else e.edict[0])
SET_GLOBAL('MSG', lambda e: eng.get_msg_id(e))
SET_GLOBAL('CREATE_NAMED_ENTITY', lambda c, o=(0,0,0), a=(0,0,0), ow=None: eng.CreateNamedEntity(c, o, a, ow))
SET_GLOBAL('ENT_REMOVE', lambda x: (eng.RemoveEntity(x), LINKED_ENTS.pop(x[0]) if x[0] in LINKED_ENTS else -1))

SET_GLOBAL('SetBits', lambda flBitVector, bits: (flBitVector | bits))
SET_GLOBAL('ClearBits', lambda flBitVector, bits: (flBitVector & (~bits)))
SET_GLOBAL('FBitSet', lambda flBitVector, bit: (flBitVector & bits))