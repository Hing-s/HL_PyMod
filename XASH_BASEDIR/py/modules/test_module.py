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
HandleCmd('spawn', spawn)
HandleCmd("msg", msg)
HandleCmd('say', say)


def death(data):
	eng.AlertMessage(at_console, '{}\n'.format(data))
	ALERT(at_console, eng.CreateNamedEntity('weapon_rpg', ENTINDEX(data[1]).pev.origin, None, None))

HandleMsg('DeathMsg', death)