def cmd(ent, cmd, args):
	try:
		eng.AlertMessage(1, '{}\n'.format(eval(args)))
	except Exception as e:
		eng.AlertMessage(1, '{}\n'.format(e))


def hp(ent, cmd, args):
	pev = ENT(ent).pev
	pev.health = float(args)

def spawn(ent, cmd, args):
	pev = ENT(ent).pev
	offset = eng.CreateEntity(args, pev.origin, pev.angles, 0)

	eng.RemoveEntity(offset)
	eng.RemoveEntity(offset)


	eng.AlertMessage(1, "OK\n")

HandleClientCmd('py', cmd)
HandleClientCmd('hp', hp)
HandleClientCmd('sp', spawn)