class entvars(object):
	def __init__(self, edict):
		self.edict = edict
		
	def __getattr__(self, y):
		if y == 'edict':
			return super().__getattribute__(y)
		
		return eng.get(self.edict, y)
	
	def __setattr__(self, x, y):
		if x == 'edict':
			return super().__setattr__(x, y)
		
		eng.set(self.edict, x, y)
	
class Entity:
	def __init__(self, edict):
		self.edict = edict
		self.pev = entvars(edict)


SET_GLOBAL('ENT', Entity)
SET_GLOBAL('at_notice', 0)
SET_GLOBAL('at_console', 1)
SET_GLOBAL('at_aiconsole', 2)
SET_GLOBAL('at_warning', 3)
SET_GLOBAL('at_error', 4)
SET_GLOBAL('at_logged', 5)