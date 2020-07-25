import entity

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

class Vector:
    def __init__(self, x, y, z):
        self.x = float(x)
        self.y = float(y)
        self.z = float(z)
        
    def __add__(self, vec):
        return Vector(self.x+vec.x, self.y+vec.y, self.z+vec.z)
        
    def __iadd__(self, vec):
        self.__add__(vec)
    
    def __sub__(self, vec):
        return Vector(self.x-vec.x, self.y-vec.y, self.z-vec.z)
    
    def __isub__(self, vec):
        self.__sub__(vec)
    
    def __neg__(self):
        return Vector(-self.x, -self.y, -self.z)
    
    def __eq__(self, vec):
        return self.x == vec.x and self.y == vec.y and self.z == vec.z
        
    def __mul__(self, fl):
        return Vector(self.x * fl, self.y * fl, self.z * fl)
    
    def __imul__(self, fl):
        return self.__mul__(fl)
        
    def lenght(self):
        import math
        return math.sqrt(self.x ** 2 + self.y ** 2 + self.z ** 2)

    def tuple(self):
   		return (self.x, self.y, self.z)
        
    def __repr__(self):
        return '({}, {}, {})'.format(self.x, self.y, self.z)

    def __str__(self):
    	return self.__repr__()


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
SET_GLOBAL('Vector', Vector)
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