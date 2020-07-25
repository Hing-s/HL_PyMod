def cmd(ent, cmd, args):
	try:
		eng.AlertMessage(1, '{}\n'.format(eval(args)))
	except Exception as e:
		eng.AlertMessage(1, '{}\n'.format(e))

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
	eng.send_message(2, 'HudText', None, None, (
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
		LinkEntToObject(self.edict, self)
		self.spawn()

		eng.PrecacheModel('models/zombie.mdl')
		eng.SetModel(self.edict, 'models/zombie.mdl')

		self.pev.classname = '@my_ent'
		self.pev.nextthink = gpGlobals.time + 0.1

	def touch(self, other):
		pass

	def use(self, other):
		ALERT(at_console, "Used by: ", other)

	def think(self):
		self.pev.nextthink = gpGlobals.time + 0.1

		ALERT(at_console, "ITS ALIVE!!!1")

def death(data):
	eng.AlertMessage(at_console, '{}\n'.format(data))

	try:
		CustomEnt(eng.CreateNamedEntity('info_target', ENTINDEX(data[1]).pev.origin, None, None))
	except Exception as e:
		ALERT(at_console, e)

HandleMsg('DeathMsg', death)