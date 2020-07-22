#include "pymod/py_engine.h"
#include "cbase.h"

#define REG(F, H) PyRegister(#F, &F, H)


FUNC(AlertMessage)
{
    const char *message;
    int type;

    if(!PyArg_ParseTuple(args, "is", &type, &message))
        return NULL;

    ALERT((ALERT_TYPE)type, message);

    return Py_None;
}

FUNC(SetOrigin) {
    PyObject *ent, *origin;

    if(!PyArg_ParseTuple(args, "OO", &ent, &origin))
        return NULL;

    edict_t *entity = ParseEnt(ent);
    Py_DECREF(ent);

    if(entity) {

    }

    return  Py_None;

}

FUNC(CreateEntity) {
    const char *entity;
    PyObject *origin, *angles, *owner;

    if(!PyArg_ParseTuple(args, "sOOO", &entity, &origin, &angles, &owner))
        return Py_None;

    edict_t *pEnt = CREATE_NAMED_ENTITY(MAKE_STRING(entity));

    if(pEnt) {
        SetVector(pEnt->v.origin, origin);
        SetVector(pEnt->v.angles, angles);

        if(owner != Py_None) {
            edict_t *pEntOwner = ParseEnt(owner);
            Py_DECREF(owner);

            if(pEntOwner)
                pEnt->v.owner = pEntOwner;
        }


        PyObject *pyEnt = Py_BuildValue("(ii)", ENTINDEX(pEnt), pEnt->serialnumber);

        DispatchSpawn(pEnt);

        return pyEnt;
    }

    return Py_None;
}

FUNC(RemoveEntity) {
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "O", &ent))
        return NULL;

    edict_t *entity = ParseEnt(ent);
    Py_DECREF(ent);

    if(entity)
        REMOVE_ENTITY(entity);

    return Py_None;
}

FUNC(PrecacheModel)
{
    const char *model;

    if(!PyArg_ParseTuple(args, "s", &model))
        return NULL;

    PRECACHE_MODEL(model);

    return Py_None;
}

FUNC(PrecacheSound)
{
    const char *sound;

    if(!PyArg_ParseTuple(args, "s", &sound))
        return NULL;

    PRECACHE_SOUND(sound);

    return Py_None;
}

FUNC(SetModel)
{
    const char *model;
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "Os", &ent, &model))
        return NULL;

    edict_t *entity = ParseEnt(ent);
    Py_DECREF(ent);

    if(entity)
        SET_MODEL(entity, model);

    return Py_None;
}

#define IS(val) (!strcmp(var, #val))

FUNC(get)
{
    const char *var;
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "Os", &ent, &var))
        return Py_None;

    edict_t *entity = ParseEnt(ent);
    Py_DECREF(ent);

    if(entity)
        if IS(health)
                return PyFloat_FromDouble((double)entity->v.health);
        if IS(origin)
                return GetVector(entity->v.origin);
        if IS(velocity)
                return GetVector(entity->v.velocity);
        if IS(armor)
                return PyFloat_FromDouble((double)entity->v.armorvalue);

     return Py_None;
}

FUNC(set)
{
    const char *var;
    PyObject *value;
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "OsO", &ent, &var, &value))
        return Py_None;

    edict_t *entity = ParseEnt(ent);
    Py_DECREF(ent);

   if(entity)
        if IS(health)
                 entity->v.health = (float)PyFloat_AsDouble(value);
        if IS(origin)
                SetVector(entity->v.origin, value);
        if IS(velocity)
                SetVector(entity->v.velocity, value);
        if IS(armor)
                entity->v.armorvalue = (float)PyFloat_AsDouble(value);

     return Py_None;
}

FUNC(get_player_by_name) {
    const char *name;

     if(!PyArg_ParseTuple(args, "s", &name))
         return NULL;

    for(int i = 0; i < gpGlobals->maxClients; i++)
    {
        CBaseEntity *pPlayer = UTIL_PlayerByIndex(i);


        if(pPlayer && !strcmp(name, STRING(pPlayer->pev->netname)))
        {
            edict_t *pEnt = pPlayer->edict();
            return Py_BuildValue("(ii)",  ENTINDEX(pEnt), pEnt->serialnumber);
        }
    }

    return Py_None;
}

FUNC(ServerCmd) {
    const char *cmd;

    if(!PyArg_ParseTuple(args, "s", &cmd))
        return NULL;

    SERVER_COMMAND(cmd);

    return Py_None;
}

FUNC(ClientCmd) {
    PyObject *ent;
    const char *cmd;

    if(!PyArg_ParseTuple(args, "ks", &ent, &cmd))
        return NULL;

    edict_t *entity = ParseEnt(ent);
    Py_DECREF(ent);

    if(entity)
        CLIENT_COMMAND(entity, cmd);

    return Py_None;
}


void CreateEngineModule() {
    PyObject *builtins = PyImport_ImportModule("builtins");

    PyModuleDef eng_funcs = {PyModuleDef_HEAD_INIT, "eng", NULL, -1, methods, NULL, NULL, NULL, NULL};
    PyObject *eng = PyModule_Create(&eng_funcs);

    PyObject_SetAttrString(builtins, "eng", eng);;
}

void PyInitEngine() {
    REG(AlertMessage, "");
    REG(PrecacheModel, "");
    REG(PrecacheSound, "");
    REG(SetModel, "");
    REG(set, "");
    REG(get, "");
    REG(CreateEntity, "");
    REG(RemoveEntity, "");
    REG(get_player_by_name, "");
    REG(ServerCmd, "");
    REG(ClientCmd, "");
    REG(SetOrigin, "");

    CreateEngineModule();
}



