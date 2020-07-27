SET_GLOBAL('LINKED_ENTS', {})

class EntLinker:
	def __init__(self, pEnt):
		self.pEnt = pEnt

class entvars(object):
	def __init__(self, edict):
		self.edict = edict
		
	def __getattr__(self, y):
		if y == 'edict':
			return super().__getattribute__(y)
		
		return eng.ent_get(self.edict, y)
	
	def __setattr__(self, x, y):
		if x == 'edict':
			return super().__setattr__(x, y)
		
		eng.ent_set(self.edict, x, y)

class Entity:
	def __del__(self):
		self.this = None
		del self

	def __init__(self, edict):
		self.this = self # Hack for GC. This cannot by deleted by him.
		self.edict = edict
		self.pev = entvars(edict)

	def is_player(self):
		return eng.is_player(self.edict)

	def is_valid(self):
		return eng.is_valid(self.edict)

	def get_gun_position(self):
		if not self.is_player():
			return

		return (Vector(*self.pev.origin) + Vector(*self.pev.view_ofs)).tuple()

	def get_nick(self):
		if not self.is_player():
			return

		return self.pev.netname

	def spawn(self):
		return eng.dispatch_spawn(self.edict)

	def touch(self, other):	
		eng.dispatch_touch(self.edict, other.edict if not isinstance(other, tuple) else other)

	def use(self, other):
		eng.dispatch_use(self.edict, other.edict if not isinstance(other, tuple) else other)

	def set_touch(self, touch):
		self._touch = lambda t, e: touch(t, e)

	def give(self, ent):
		pass

def LinkEntToObject(pEnt):
	if not pEnt.is_valid():
		return
	
	LINKED_ENTS[pEnt.edict[0]] = pEnt

SET_GLOBAL('LinkEntToObject', LinkEntToObject)
SET_GLOBAL('ENT', Entity)