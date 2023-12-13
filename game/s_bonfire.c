#include "g_local.h"
#include "m_player.h"

qboolean NearBonfire(edict_t* ent) {
	edict_t* blip = NULL;
	float range = 100;

	if (!ent || !ent->inuse)
		return false;

	while ((blip = findradius(blip, ent->s.origin, range)) != NULL) {
		if (!blip->inuse)
			continue;

		if (strcmp(blip->classname, "bonfire") == 0)
			return true;
	}

	return false;
}

void Cmd_BonfireUI_f(edict_t *ent) {
	gi.dprintf("===========================\n\nBONFIRE UPGRADE MENU\n\n==========================\n\n");
	gi.dprintf("Here, you can upgrade your stats using souls. It costs 10 souls to uprade a stat.\n");
	gi.dprintf("Souls Balance: %d\n\n", ent->client->souls);
	gi.dprintf("CURRENT STATS\n\n");
	gi.dprintf("STR: %d\n", ent->client->pers.str);
	gi.dprintf("DEX: %d\n", ent->client->pers.dex);
	gi.dprintf("INT: %d\n\n", ent->client->pers.inte);
	gi.dprintf("To upgrade a stat, type \"upgrade [stat] [levels]\".\n");
	for (int i = 0; i < 35; i++) gi.dprintf("\n");
}

void Cmd_UpgradeStat_f(edict_t* ent) {
    char* stat;
    int levels;

	stat = gi.argv(1);
	levels = atoi(gi.argv(2));

	if (ent->client && NearBonfire(ent)) {
		if (ent->client->souls >= 10 * levels) {
			// charge the player
			ent->client->souls -= 10 * levels;

			if (Q_stricmp(stat, "str") == 0) {
				ent->client->pers.str += levels;
			}
			else if (Q_stricmp(stat, "dex") == 0) {
				ent->client->pers.dex += levels;
			}
			else if (Q_stricmp(stat, "int") == 0) {
				ent->client->pers.inte += levels;
			}

			Cmd_BonfireUI_f(ent);
		}
		else {
			gi.dprintf("Insufficient funds.\n");
		}
	}
	else {
		gi.dprintf("Must be near a bonfire.\n");
	}
}

void Cmd_PrintPos_f(edict_t* ent) {
	gi.dprintf("Player's Origin: x: %f, y: %f, z: %f\n",
		ent->s.origin[0],
		ent->s.origin[1],
		ent->s.origin[2]);
}

void SP_bonfire(edict_t* self) {
	self->s.modelindex = gi.modelindex("models/objects/dmspot/tris.md2");
	self->solid = SOLID_BBOX;
	self->movetype = MOVETYPE_NONE;
	VectorSet(self->mins, -16, -16, 0);
	VectorSet(self->maxs, 16, 16, 32);
	self->takedamage = DAMAGE_NO;
	self->touch = NULL;
	gi.linkentity(self);
}

void Cmd_SpawnBonfire_f(edict_t* ent) {
	edict_t* bonfire = G_Spawn();
	bonfire->classname = "bonfire";
	VectorCopy(ent->s.origin, bonfire->s.origin);
	SP_bonfire(bonfire);
}

void PlaceBonfires() {
	vec3_t origin = { -90, 598, -103.875 };

	edict_t* bonfire = G_Spawn();
	bonfire->classname = "bonfire";
	VectorCopy(origin, bonfire->s.origin);
	SP_bonfire(bonfire);
}