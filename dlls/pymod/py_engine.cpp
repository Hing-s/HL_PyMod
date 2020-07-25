#include "pymod/py_engine.h"
#include "cbase.h"

int msgs_count = 0;
msg_t MSGS[197];

#define REG(F, H) PyRegister(#F, &F, H)
#define IS(val) (!strcmp(var, #val))

FUNC(AlertMessage)
{
    const char *message;
    int type;

    if(!PyArg_ParseTuple(args, "is", &type, &message))
        return NULL;

    ALERT((ALERT_TYPE)type, message);

    Py_XDECREF(args);
    Py_XINCREF(Py_None);
    return Py_None;
}

FUNC(send_message) {
    PyObject *msg;
    int dest, msg_type;
    PyObject *origin, *owner;


    if(!PyArg_ParseTuple(args, "iiOOO", &dest, &msg_type, &origin, &owner, &msg))
        return NULL;

    if(msg_type == -1) {
        ALERT(at_error, "Passed unregistered msg! Return.\n");
        Py_XDECREF(args);
        Py_XINCREF(Py_None);
        return Py_None;
    }

    if(PyObject_Size(msg) > 0) {
        Vector VecOrigin;
        edict_t *pEntOwner = ParseEnt(owner);

        if(SetVector(VecOrigin, origin))
            MESSAGE_BEGIN(dest, msg_type, VecOrigin, pEntOwner);
        else
            MESSAGE_BEGIN(dest, msg_type);

        for(int i = 0; i < PyObject_Size(msg); i++) {
            PyObject *index  = PyLong_FromLong(i);
            PyObject *data = PyObject_GetItem(msg, index);
            const char *type;
            PyObject *value;

            if(!PyArg_ParseTuple(data, "sO", &type, &value)) {
                MESSAGE_END();
                return NULL;
            }

            if(!strcmp("byte", type))
                WRITE_BYTE(PyLong_AsLong(value));
            else if(!strcmp("char", type))
                WRITE_CHAR(PyLong_AsLong(value));
            else if(!strcmp("short", type))
                WRITE_SHORT(PyLong_AsLong(value));
            else if(!strcmp("long", type))
                WRITE_LONG(PyLong_AsLong(value));
            else if(!strcmp("angle", type))
                WRITE_ANGLE((float)PyFloat_AsDouble(value));
            else if(!strcmp("coord", type))
                WRITE_COORD((float)PyLong_AsLong(value));
            else if(!strcmp("string", type))
                WRITE_STRING(PyUnicode_AsUTF8(value));
            else if(!strcmp("entity", type))
            {
                edict_t *pEnt = ParseEnt(value);

                if(pEnt)
                    WRITE_ENTITY(ENTINDEX(pEnt));
            }

              Py_DECREF(data);
              Py_DECREF(index);
              Py_DECREF(origin);
              Py_DECREF(owner);
        }

        MESSAGE_END();
    }

    Py_XDECREF(msg);
    return Py_None;
}

FUNC(GetPlayerAuthId) {
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "O", &ent))
        return NULL;

    edict_t *pEnt = ParseEnt(ent);

    if(pEnt)
        return PyUnicode_FromString(g_engfuncs.pfnGetPlayerAuthId(pEnt));

    return PyLong_FromLong(-1);
}

FUNC(GetPlayerWONId) {
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "O", &ent))
        return NULL;

    edict_t *pEnt = ParseEnt(ent);

    if(pEnt)
        return PyLong_FromLong(g_engfuncs.pfnGetPlayerWONId(pEnt));

    return PyLong_FromLong(-1);
}

FUNC(get_msg_id) {
    const char *msg;

    if(!PyArg_ParseTuple(args, "s", &msg))
        return NULL;

     return PyLong_FromLong(GET_MSG_ID(msg));
}
FUNC(find_entity_by_classname)
{
    PyObject *StartEnt;
    const char *classname;

    if(!PyArg_ParseTuple(args, "Os", &StartEnt, &classname))
        return NULL;

    edict_t *pStartEnt = NULL;
    edict_t *pentEntity;

    if(PyObject_IsTrue(StartEnt))
         pStartEnt = ParseEnt(StartEnt);

    if(pStartEnt)
        pentEntity = FIND_ENTITY_BY_STRING(pStartEnt, "classname", classname);
    else
        pentEntity = FIND_ENTITY_BY_STRING(NULL, "classname", classname);

    if(!FNullEnt(pentEntity))
        return Py_BuildValue("(ii)", ENTINDEX(pentEntity), pentEntity->serialnumber);

    return Py_None;
}

FUNC(cvar_set_float) {
    const char *cvar;
    float value;

    if(!PyArg_ParseTuple(args, "sf", &cvar, &value))
        return NULL;

    CVAR_SET_FLOAT(cvar, value);

    return Py_None;
}

FUNC(cvar_set_string) {
    const char *cvar, *value;

    if(!PyArg_ParseTuple(args, "ss", &cvar, &value))
        return NULL;

    CVAR_SET_STRING(cvar, value);

    return Py_None;
}


FUNC(SetOrigin) {
    PyObject *ent, *origin;

    if(!PyArg_ParseTuple(args, "OO", &ent, &origin))
        return NULL;

    edict_t *entity = ParseEnt(ent);

    if(entity) {
        Vector VecOrigin = g_vecZero;
        if(SetVector(VecOrigin, origin))
            SET_ORIGIN(entity, VecOrigin);
    }

    return  Py_None;

}

FUNC(CreateNamedEntity) {
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

            if(pEntOwner != NULL)
                pEnt->v.owner = pEntOwner;
        }

        //DispatchSpawn(pEnt);
        return Py_BuildValue("(ii)", ENTINDEX(pEnt), pEnt->serialnumber);
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

    if(entity)
        SET_MODEL(entity, model);

    return Py_None;
}

FUNC(ent_get)
{
    const char *var;
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "Os", &ent, &var))
        return Py_None;

    edict_t *entity = ParseEnt(ent);

    if(entity)
    {
        if IS(health)
                return PyFloat_FromDouble((double)entity->v.health);
        else if IS(origin)
                return GetVector(entity->v.origin);
        else if IS(velocity)
                return GetVector(entity->v.velocity);
        else if IS(armor)
                return PyFloat_FromDouble((double)entity->v.armorvalue);
        else if IS(view_ofs)
                return GetVector(entity->v.view_ofs);
        else if IS(classname)
                return PyUnicode_FromString(STRING(entity->v.classname));
        else if IS(punchangle)
                return GetVector(entity->v.punchangle);
        else if IS(netname)
                return PyUnicode_FromString(STRING(entity->v.netname));
        else if IS(nextthink)
                return PyFloat_FromDouble((double)entity->v.nextthink);
        else if IS(solid)
                return PyLong_FromLong(entity->v.solid);
        else if IS(movetype)
                return PyLong_FromLong(entity->v.movetype);
    }

     return Py_None;
}

FUNC(ent_set)
{
    const char *var;
    PyObject *value;
    PyObject *ent;

    Py_XINCREF(Py_None);

    if(!PyArg_ParseTuple(args, "OsO", &ent, &var, &value))
        return Py_None;

    edict_t *entity = ParseEnt(ent);

   if(entity)
   {
        if IS(health)
                entity->v.health = (float)PyFloat_AsDouble(value);
        else if IS(origin)
                SetVector(entity->v.origin, value);
        else if IS(velocity)
                SetVector(entity->v.velocity, value);
        else if IS(armor)
                entity->v.armorvalue = (float)PyFloat_AsDouble(value);
        else if IS(view_ofs)
                SetVector(entity->v.view_ofs, value);
        else if IS(classname)
                entity->v.classname = MAKE_STRING(PyUnicode_AsUTF8(value));
        else if IS(punchangle)
                SetVector(entity->v.punchangle, value);
        else if IS(nextthink)
                entity->v.nextthink = (float)PyFloat_AsDouble(value);
        else if IS(solid)
                entity->v.solid = PyLong_AsLong(value);
        else if IS(movetype)
                entity->v.movetype = PyLong_AsLong(value);
   }
     return Py_None;
}

FUNC(MakeVector) {
    PyObject *coords;

    if(!PyArg_ParseTuple(args, "O", &coords))
        return Py_None;

    Vector Vec;
    if(SetVector(Vec, coords))
    {
        MAKE_VECTORS(Vec);
        return Py_True;
    }

    return Py_False;
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

    if(!PyArg_ParseTuple(args, "Os", &ent, &cmd))
        return NULL;

    edict_t *entity = ParseEnt(ent);

    if(entity)
        CLIENT_COMMAND(entity, cmd);

    return Py_None;
}

FUNC(CreateEntity) {
    Py_DECREF(args);

    edict_t *pEnt = CREATE_ENTITY();

    if(pEnt)
        return Py_BuildValue("(ii)", ENTINDEX(pEnt), pEnt->serialnumber);

    Py_XINCREF(Py_None);
    return Py_None;
}

FUNC(SetSize) {
    PyObject *ent, *min, *max;

    if(!PyArg_ParseTuple(args, "OOO", &ent, &min, &max))
        return NULL;


    edict_t *pEnt = ParseEnt(ent);
    Vector VecMin, VecMax;

    if(pEnt && SetVector(VecMin, min), SetVector(VecMax, max))
        UTIL_SetSize(VARS(pEnt), VecMin, VecMax);

    Py_XDECREF(args);
    Py_XINCREF(Py_None);
    return Py_None;
}

FUNC(is_player) {
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "O", &ent))
        return NULL;

    edict_t *entity = ParseEnt(ent);

    if(entity)
        return PyBool_FromLong(CBaseEntity::Instance(entity)->IsPlayer());

    Py_XINCREF(Py_False);
    return Py_False;
}

FUNC(is_valid)
{
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "O", &ent))
        return NULL;

    edict_t *entity = ParseEnt(ent);
    Py_XDECREF(args);

    if(entity)
    {
        Py_XINCREF(Py_True);
        return Py_True;
    }

    Py_XINCREF(Py_False);
    return Py_False;
}

FUNC(globals_get) {
    const char *var;
    Py_XINCREF(Py_None);

    if(!PyArg_ParseTuple(args, "s", &var))
        return Py_None;

    Py_XDECREF(args);

    if IS(time)
            return PyFloat_FromDouble((double)gpGlobals->time);
    else if IS(v_forward)
            return GetVector(gpGlobals->v_forward);
    else if IS(v_up)
            return GetVector(gpGlobals->v_up);
    else if IS(v_right)
            return GetVector(gpGlobals->v_right);

    return Py_None;
}


FUNC(dispatch_spawn) {
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "O", &ent))
        return NULL;

    edict_t *entity = ParseEnt(ent);

    if(entity)
        return PyBool_FromLong((*other_gFunctionTable.pfnSpawn)(entity));

    Py_XINCREF(Py_False);
    return Py_False;
}

FUNC(dispatch_touch) {
    PyObject *ent, *other;

    if(!PyArg_ParseTuple(args, "OO", &ent, &other))
        return NULL;

    edict_t *pEnt = ParseEnt(ent);
    edict_t *pEntOther = ParseEnt(other);

    Py_XDECREF(args);
    Py_XDECREF(ent);
    Py_XDECREF(other);

    if(pEnt && pEntOther)
        (*other_gFunctionTable.pfnTouch)(pEnt, pEntOther);

    Py_XINCREF(Py_None);
    return Py_None;
}

FUNC(dispatch_use) {
    PyObject *ent, *other;

    if(!PyArg_ParseTuple(args, "OO", &ent, &other))
        return NULL;

    edict_t *pEnt = ParseEnt(ent);
    edict_t *pEntOther = ParseEnt(other);

    Py_XDECREF(args);
    Py_XDECREF(ent);
    Py_XDECREF(other);

    if(pEnt && pEntOther)
        (*other_gFunctionTable.pfnUse)(pEnt, pEntOther);

    Py_XINCREF(Py_None);
    return Py_None;
}

FUNC(dispatch_think) {
    PyObject *ent;

    if(!PyArg_ParseTuple(args, "O", &ent))
        return NULL;

    edict_t *pEnt = ParseEnt(ent);

    Py_XDECREF(args);
    Py_XDECREF(ent);

    if(pEnt)
        (*other_gFunctionTable.pfnThink)(pEnt);

    Py_XINCREF(Py_None);
    return Py_None;
}


FUNC(get_entity_by_index) {
    int index;

    if(!PyArg_ParseTuple(args, "i", &index))
        return NULL;

    edict_t *pEnt = INDEXENT(index);

    if(pEnt)
        return Py_BuildValue("(ii)", ENTINDEX(pEnt), pEnt->serialnumber);

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
    REG(ent_set, "");
    REG(ent_get, "");
    REG(CreateNamedEntity, "");
    REG(RemoveEntity, "");
    REG(get_player_by_name, "");
    REG(ServerCmd, "");
    REG(ClientCmd, "");
    REG(SetOrigin, "");
    REG(cvar_set_float, "");
    REG(cvar_set_string, "");
    REG(find_entity_by_classname, "");
    REG(get_msg_id, "");
    REG(send_message, "");
    REG(is_player, "");
    REG(is_valid, "");
    REG(globals_get, "");
    REG(MakeVector, "");
    REG(GetPlayerAuthId, "");
    REG(GetPlayerWONId, "");
    REG(CreateEntity, "");
    REG(dispatch_spawn, "");
    REG(get_entity_by_index, "");
    REG(dispatch_touch, "");
    REG(dispatch_use, "");
    REG(SetSize, "");

    CreateEngineModule();
}



