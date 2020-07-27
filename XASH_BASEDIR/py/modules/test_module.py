SET_GLOBAL('a', [])

def cmd(ent, cmd, args):
	try:
		ALERT(at_console, str(eval(args)))
	except Exception as e:
		ALERT(at_console, e)

	return True


def hp(ent, cmd, args):
	ENT(ent).pev.health = float(args)
	return True

def spawn(ent, cmd, args):
	try:
		CustomEnt(eng.CreateNamedEntity('info_target', ENT(ent).pev.origin, None, None))
	except Exception as e:
		ALERT(at_console, e)

	return True

def msg(ent, cmd, args):
	eng.send_message(2, MSG('HudText'), None, None, (
		WRITE_STRING('TEST'),
	))

	return True

def say(ent, cmd, args):
	args = args.strip('"')
	
	if(args == 'test'):
		hp(ent, cmd, 100)
		return True

	return False

HandleCmd('py', cmd)
HandleCmd('hp', hp)
HandleCmd('ent', spawn)
HandleCmd("msg", msg)
HandleCmd('say', say)

class CustomEnt(ENT):
	def __init__(self, ent):
		super().__init__(ent)
		LinkEntToObject(self)
		self.spawn()

	def spawn(self):
		eng.PrecacheModel('models/zombie.mdl')
		eng.SetModel(self.edict, 'models/zombie.mdl')

		self.pev.movetype = 6
		self.pev.solid = SOLID_NOT
		self.pev.classname = '@my_ent'
		self.pev.nextthink = gpGlobals.time + 0.1
		self.last_use = 0

		self.can_take = 25

	def touch(self, other):
		super().touch(other)

	def use(self, other):
		user = ENT(other)
		if not user.is_player() or user.pev.health <= 1:
			return

		if(gpGlobals.time - self.last_use < 0.5):
			return

		self.last_use = gpGlobals.time
		self.can_take -= 1
		user.pev.health-=1

		if(self.can_take < 0):
			user.pev.armor += 200
			ENT_REMOVE(self.edict)

	def think(self):
		self.pev.nextthink = gpGlobals.time + 0.5

		if(gpGlobals.time - self.last_use > 3):
			self.can_take = 50

def death(data):
	eng.AlertMessage(at_console, '{}\n'.format(data))

	try:
		CustomEnt(CREATE_NAMED_ENTITY('info_target', ENTINDEX(data[0]).pev.origin))
	except Exception as e:
		ALERT(at_console, e)

HandleMsg('DeathMsg', death)