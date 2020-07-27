class Weapon(ENT):
	def AttachToPlayer(self, player):
		if not ENT(player).is_player():
			return

		pev.movetype = MOVETYPE_FOLLOW;
		pev.solid = SOLID_NOT;
		pev.aiment = pPlayer.edict();
		pev.effects = EF_NODRAW; 
		pev.modelindex = 0;
		pev.model = 0;
		pev.owner = pPlayer.edict();
		pev.nextthink = 0;