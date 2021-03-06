SET_GLOBAL('VEC_HUMAN_HULL_MIN', ( -16, -16, 0 ))
SET_GLOBAL('VEC_HUMAN_HULL_MAX', ( 16, 16, 72 ))

SET_GLOBAL('FL_FLY', (1<<0))
SET_GLOBAL('FL_SWIM', (1<<1))
SET_GLOBAL('FL_CONVEYOR', (1<<2))
SET_GLOBAL('FL_CLIENT', (1<<3))
SET_GLOBAL('FL_INWATER', (1<<4))
SET_GLOBAL('FL_MONSTER', (1<<5))
SET_GLOBAL('FL_GODMODE', (1<<6))
SET_GLOBAL('FL_NOTARGET', (1<<7))
SET_GLOBAL('FL_SKIPLOCALHOST', (1<<8))
SET_GLOBAL('FL_ONGROUND', (1<<9))
SET_GLOBAL('FL_PARTIALGROUND', (1<<10))
SET_GLOBAL('FL_WATERJUMP', (1<<11))
SET_GLOBAL('FL_FROZEN', (1<<12))
SET_GLOBAL('FL_FAKECLIENT', (1<<13))
SET_GLOBAL('FL_DUCKING', (1<<14))
SET_GLOBAL('FL_FLOAT', (1<<15))
SET_GLOBAL('FL_GRAPHED', (1<<16))
SET_GLOBAL('FL_IMMUNE_WATER', (1<<17))
SET_GLOBAL('FL_IMMUNE_SLIME', (1<<18))
SET_GLOBAL('FL_IMMUNE_LAVA', (1<<19))
SET_GLOBAL('FL_PROXY', (1<<20))
SET_GLOBAL('FL_ALWAYSTHINK', (1<<21))
SET_GLOBAL('FL_BASEVELOCITY', (1<<22))
SET_GLOBAL('FL_MONSTERCLIP', (1<<23))
SET_GLOBAL('FL_ONTRAIN', (1<<24))
SET_GLOBAL('FL_WORLDBRUSH', (1<<25))
SET_GLOBAL('FL_SPECTATOR', (1<<26))
SET_GLOBAL('FL_CUSTOMENTITY', (1<<29))
SET_GLOBAL('FL_KILLME', (1<<30))
SET_GLOBAL('FL_DORMANT', (1<<31))
SET_GLOBAL('FTRACE_SIMPLEBOX', (1<<0))
SET_GLOBAL('FTRACE_IGNORE_GLASS', (1<<1))
SET_GLOBAL('WALKMOVE_NORMAL', 0)
SET_GLOBAL('WALKMOVE_WORLDONLY', 1)
SET_GLOBAL('WALKMOVE_CHECKONLY', 2)
SET_GLOBAL('MOVETYPE_NONE', 0)
SET_GLOBAL('MOVETYPE_WALK', 3)
SET_GLOBAL('MOVETYPE_STEP', 4)
SET_GLOBAL('MOVETYPE_FLY', 5)
SET_GLOBAL('MOVETYPE_TOSS', 6)
SET_GLOBAL('MOVETYPE_PUSH', 7)
SET_GLOBAL('MOVETYPE_NOCLIP', 8)
SET_GLOBAL('MOVETYPE_FLYMISSILE', 9)
SET_GLOBAL('MOVETYPE_BOUNCE', 10)
SET_GLOBAL('MOVETYPE_BOUNCEMISSILE', 11)
SET_GLOBAL('MOVETYPE_FOLLOW', 12)
SET_GLOBAL('MOVETYPE_PUSHSTEP', 13)
SET_GLOBAL('MOVETYPE_COMPOUND', 14)
SET_GLOBAL('SOLID_NOT', 0)
SET_GLOBAL('SOLID_TRIGGER', 1)
SET_GLOBAL('SOLID_BBOX', 2)
SET_GLOBAL('SOLID_SLIDEBOX', 3)
SET_GLOBAL('SOLID_BSP', 4)
SET_GLOBAL('SOLID_CUSTOM', 5)
SET_GLOBAL('DEAD_NO', 0)
SET_GLOBAL('DEAD_DYING', 1)
SET_GLOBAL('DEAD_DEAD', 2)
SET_GLOBAL('DEAD_RESPAWNABLE', 3)
SET_GLOBAL('DEAD_DISCARDBODY', 4)
SET_GLOBAL('DAMAGE_NO', 0)
SET_GLOBAL('DAMAGE_YES', 1)
SET_GLOBAL('DAMAGE_AIM', 2)
SET_GLOBAL('EF_BRIGHTFIELD', 1)
SET_GLOBAL('EF_MUZZLEFLASH', 2)
SET_GLOBAL('EF_BRIGHTLIGHT', 4)
SET_GLOBAL('EF_DIMLIGHT', 8)
SET_GLOBAL('EF_INVLIGHT', 16)
SET_GLOBAL('EF_NOINTERP', 32)
SET_GLOBAL('EF_LIGHT', 64)
SET_GLOBAL('EF_NODRAW', 128)
SET_GLOBAL('EF_NIGHTVISION', 256)
SET_GLOBAL('EF_SNIPERLASER', 512)
SET_GLOBAL('EF_FIBERCAMERA', 1024)
SET_GLOBAL('EF_NOREFLECT', (1<<24))
SET_GLOBAL('EF_REFLECTONLY', (1<<25))
SET_GLOBAL('EF_NOWATERCSG', (1<<26))
SET_GLOBAL('EF_FULLBRIGHT', (1<<27))
SET_GLOBAL('EF_NOSHADOW', (1<<28))
SET_GLOBAL('EF_MERGE_VISIBILITY', (1<<29))
SET_GLOBAL('EF_REQUEST_PHS', (1<<30))
SET_GLOBAL('EFLAG_SLERP', 1)
SET_GLOBAL('TE_BEAMPOINTS', 0)
SET_GLOBAL('TE_BEAMENTPOINT', 1)
SET_GLOBAL('TE_GUNSHOT', 2)
SET_GLOBAL('TE_EXPLOSION', 3)
SET_GLOBAL('TE_EXPLFLAG_NONE', 0)
SET_GLOBAL('TE_EXPLFLAG_NOADDITIVE', 1)
SET_GLOBAL('TE_EXPLFLAG_NODLIGHTS', 2)
SET_GLOBAL('TE_EXPLFLAG_NOSOUND', 4)
SET_GLOBAL('TE_EXPLFLAG_NOPARTICLES', 8)
SET_GLOBAL('TE_EXPLFLAG_DRAWALPHA', 16)
SET_GLOBAL('TE_EXPLFLAG_ROTATE', 32)
SET_GLOBAL('TE_TAREXPLOSION', 4)
SET_GLOBAL('TE_SMOKE', 5)
SET_GLOBAL('TE_TRACER', 6)
SET_GLOBAL('TE_LIGHTNING', 7)
SET_GLOBAL('TE_BEAMENTS', 8)
SET_GLOBAL('TE_SPARKS', 9)
SET_GLOBAL('TE_LAVASPLASH', 10)
SET_GLOBAL('TE_TELEPORT', 11)
SET_GLOBAL('TE_EXPLOSION2', 12)
SET_GLOBAL('TE_BSPDECAL', 13)
SET_GLOBAL('TE_IMPLOSION', 14)
SET_GLOBAL('TE_SPRITETRAIL', 15)
SET_GLOBAL('TE_BEAM', 16)
SET_GLOBAL('TE_SPRITE', 17)
SET_GLOBAL('TE_BEAMSPRITE', 18)
SET_GLOBAL('TE_BEAMTORUS', 19)
SET_GLOBAL('TE_BEAMDISK', 20)
SET_GLOBAL('TE_BEAMCYLINDER', 21)
SET_GLOBAL('TE_BEAMFOLLOW', 22)
SET_GLOBAL('TE_GLOWSPRITE', 23)
SET_GLOBAL('TE_BEAMRING', 24)
SET_GLOBAL('TE_STREAK_SPLASH', 25)
SET_GLOBAL('TE_BEAMHOSE', 26)
SET_GLOBAL('TE_DLIGHT', 27)
SET_GLOBAL('TE_ELIGHT', 28)
SET_GLOBAL('TE_TEXTMESSAGE', 29)
SET_GLOBAL('TE_LINE', 30)
SET_GLOBAL('TE_BOX', 31)
SET_GLOBAL('TE_KILLBEAM', 99)
SET_GLOBAL('TE_LARGEFUNNEL', 100)
SET_GLOBAL('TE_BLOODSTREAM', 101)
SET_GLOBAL('TE_SHOWLINE', 102)
SET_GLOBAL('TE_BLOOD', 103)
SET_GLOBAL('TE_DECAL', 104)
SET_GLOBAL('TE_FIZZ', 105)
SET_GLOBAL('TE_MODEL', 106)
SET_GLOBAL('TE_EXPLODEMODEL', 107)
SET_GLOBAL('TE_BREAKMODEL', 108)
SET_GLOBAL('TE_GUNSHOTDECAL', 109)
SET_GLOBAL('TE_SPRITE_SPRAY', 110)
SET_GLOBAL('TE_ARMOR_RICOCHET', 111)
SET_GLOBAL('TE_PLAYERDECAL', 112)
SET_GLOBAL('TE_BUBBLES', 113)
SET_GLOBAL('TE_BUBBLETRAIL', 114)
SET_GLOBAL('TE_BLOODSPRITE', 115)
SET_GLOBAL('TE_WORLDDECAL', 116)
SET_GLOBAL('TE_WORLDDECALHIGH', 117)
SET_GLOBAL('TE_DECALHIGH', 118)
SET_GLOBAL('TE_PROJECTILE', 119)
SET_GLOBAL('TE_SPRAY', 120)
SET_GLOBAL('TE_PLAYERSPRITES', 121)
SET_GLOBAL('TE_PARTICLEBURST', 122)
SET_GLOBAL('TE_FIREFIELD', 123)
SET_GLOBAL('TEFIRE_FLAG_ALLFLOAT', 1)
SET_GLOBAL('TEFIRE_FLAG_SOMEFLOAT', 2)
SET_GLOBAL('TEFIRE_FLAG_LOOP', 4)
SET_GLOBAL('TEFIRE_FLAG_ALPHA', 8)
SET_GLOBAL('TEFIRE_FLAG_PLANAR', 16)
SET_GLOBAL('TEFIRE_FLAG_ADDITIVE', 32)
SET_GLOBAL('TE_PLAYERATTACHMENT', 124)
SET_GLOBAL('TE_KILLPLAYERATTACHMENTS', 125)
SET_GLOBAL('TE_MULTIGUNSHOT', 126)
SET_GLOBAL('TE_USERTRACER', 127)
SET_GLOBAL('MSG_BROADCAST', 0)
SET_GLOBAL('MSG_ONE', 1)
SET_GLOBAL('MSG_ALL', 2)
SET_GLOBAL('MSG_INIT', 3)
SET_GLOBAL('MSG_PVS', 4)
SET_GLOBAL('MSG_PAS', 5)
SET_GLOBAL('MSG_PVS_R', 6)
SET_GLOBAL('MSG_PAS_R', 7)
SET_GLOBAL('MSG_ONE_UNRELIABLE', 8)
SET_GLOBAL('MSG_SPEC', 9)
SET_GLOBAL('CONTENTS_EMPTY', -1)
SET_GLOBAL('CONTENTS_SOLID', -2)
SET_GLOBAL('CONTENTS_WATER', -3)
SET_GLOBAL('CONTENTS_SLIME', -4)
SET_GLOBAL('CONTENTS_LAVA', -5)
SET_GLOBAL('CONTENTS_SKY', -6)
SET_GLOBAL('CONTENTS_ORIGIN', -7)
SET_GLOBAL('CONTENTS_CLIP', -8)
SET_GLOBAL('CONTENTS_CURRENT_0', -9)
SET_GLOBAL('CONTENTS_CURRENT_90', -10)
SET_GLOBAL('CONTENTS_CURRENT_180', -11)
SET_GLOBAL('CONTENTS_CURRENT_270', -12)
SET_GLOBAL('CONTENTS_CURRENT_UP', -13)
SET_GLOBAL('CONTENTS_CURRENT_DOWN', -14)
SET_GLOBAL('CONTENTS_TRANSLUCENT', -15)
SET_GLOBAL('CONTENTS_LADDER', -16)
SET_GLOBAL('CONTENT_FLYFIELD', -17)
SET_GLOBAL('CONTENT_GRAVITY_FLYFIELD', -18)
SET_GLOBAL('CONTENT_FOG', -19)
SET_GLOBAL('CONTENT_EMPTY', -1)
SET_GLOBAL('CONTENT_SOLID', -2)
SET_GLOBAL('CONTENT_WATER', -3)
SET_GLOBAL('CONTENT_SLIME', -4)
SET_GLOBAL('CONTENT_LAVA', -5)
SET_GLOBAL('CONTENT_SKY', -6)
SET_GLOBAL('CHAN_AUTO', 0)
SET_GLOBAL('CHAN_WEAPON', 1)
SET_GLOBAL('CHAN_VOICE', 2)
SET_GLOBAL('CHAN_ITEM', 3)
SET_GLOBAL('CHAN_BODY', 4)
SET_GLOBAL('CHAN_STREAM', 5)
SET_GLOBAL('CHAN_STATIC', 6)
SET_GLOBAL('CHAN_NETWORKVOICE_BASE', 7)
SET_GLOBAL('CHAN_NETWORKVOICE_END', 500)
SET_GLOBAL('CHAN_BOT', 501)
SET_GLOBAL('ATTN_NONE', 0)
SET_GLOBAL('ATTN_NORM', 0.8)
SET_GLOBAL('ATTN_IDLE', float(2))
SET_GLOBAL('ATTN_STATIC', float(1.25))
SET_GLOBAL('PITCH_NORM', 100)
SET_GLOBAL('PITCH_LOW', 95)
SET_GLOBAL('PITCH_HIGH', 120)
SET_GLOBAL('VOL_NORM', 1.0)
SET_GLOBAL('PLAT_LOW_TRIGGER', 1)
SET_GLOBAL('SF_TRAIN_WAIT_RETRIGGER', 1)
SET_GLOBAL('SF_TRAIN_START_ON', 4)
SET_GLOBAL('SF_TRAIN_PASSABLE', 8)
SET_GLOBAL('IN_ATTACK', (1<<0))
SET_GLOBAL('IN_JUMP', (1<<1))
SET_GLOBAL('IN_DUCK', (1<<2))
SET_GLOBAL('IN_FORWARD', (1<<3))
SET_GLOBAL('IN_BACK', (1<<4))
SET_GLOBAL('IN_USE', (1<<5))
SET_GLOBAL('IN_CANCEL', (1<<6))
SET_GLOBAL('IN_LEFT', (1<<7))
SET_GLOBAL('IN_RIGHT', (1<<8))
SET_GLOBAL('IN_MOVELEFT', (1<<9))
SET_GLOBAL('IN_MOVERIGHT', (1<<10))
SET_GLOBAL('IN_ATTACK2', (1<<11))
SET_GLOBAL('IN_RUN', (1<<12))
SET_GLOBAL('IN_RELOAD', (1<<13))
SET_GLOBAL('IN_ALT1', (1<<14))
SET_GLOBAL('IN_SCORE', (1<<15))
SET_GLOBAL('BREAK_TYPEMASK', 0x4F)
SET_GLOBAL('BREAK_GLASS', 0x01)
SET_GLOBAL('BREAK_METAL', 0x02)
SET_GLOBAL('BREAK_FLESH', 0x04)
SET_GLOBAL('BREAK_WOOD', 0x08)
SET_GLOBAL('BREAK_SMOKE', 0x10)
SET_GLOBAL('BREAK_TRANS', 0x20)
SET_GLOBAL('BREAK_CONCRETE', 0x40)
SET_GLOBAL('BREAK_2', 0x80)
SET_GLOBAL('BOUNCE_GLASS', BREAK_GLASS)
SET_GLOBAL('BOUNCE_METAL', BREAK_METAL)
SET_GLOBAL('BOUNCE_FLESH', BREAK_FLESH)
SET_GLOBAL('BOUNCE_WOOD', BREAK_WOOD)
SET_GLOBAL('BOUNCE_SHRAP', 0x10)
SET_GLOBAL('BOUNCE_SHELL', 0x20)
SET_GLOBAL('BOUNCE_CONCRETE', BREAK_CONCRETE)
SET_GLOBAL('BOUNCE_SHOTSHELL', 0x80)
SET_GLOBAL('TE_BOUNCE_NULL', 0)
SET_GLOBAL('TE_BOUNCE_SHELL', 1)
SET_GLOBAL('TE_BOUNCE_SHOTSHELL', 2)

