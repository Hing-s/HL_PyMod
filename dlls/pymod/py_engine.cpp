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

    return Py_None;
}

FUNC(send_message) {
    PyObject *msg;

    if(!PyArg_ParseTuple(args, "O", &msg))
        return NULL;

    if(PyObject_Size(msg) > 1) {
        int dest;
        const char *msg_type;
        PyObject *origin, *owner;
        PyObject *begin = PyObject_GetItem(msg, PyLong_FromLong(0));

        if(!PyArg_ParseTuple(begin, "isOO", &dest, &msg_type, &origin, &owner))
            return NULL;

        Vector VecOrigin;
        bool VecSetted = SetVector(VecOrigin, origin);
        edict_t *pEntOwner = ParseEnt(owner);

        if(VecSetted)
            MESSAGE_BEGIN(dest, GET_MSG_ID(msg_type), VecOrigin, pEntOwner);
        else
            MESSAGE_BEGIN(dest, GET_MSG_ID(msg_type));

        for(int i = 1; i < PyObject_Size(msg); i++) {
            PyObject *index  = PyLong_FromLong(i);
            PyObject *data = PyObject_GetItem(msg, index);
            const char *type;
            PyObject *value;

            if(!PyArg_ParseTuple(data, "sO", &type, &value)) {
                MESSAGE_END();
                return NULL;
            }

            if(!strcmp("byte", type))
                WRITE_BYTE((byte)PyLong_AsLong(value));
            else if(!strcmp("char", type))
                WRITE_CHAR((char)PyLong_AsLong(value));
            else if(!strcmp("short", type))
                WRITE_SHORT((short)PyLong_AsLong(value));
            else if(!strcmp("long", type))
                WRITE_LONG(PyLong_AsLong(value));
            else if(!strcmp("angle", type))
                WRITE_ANGLE((PyLong_AsLong(value)));
            else if(!strcmp("coord", type))
                WRITE_COORD((PyLong_AsLong(value)));
            else if(!strcmp("string", type))
                WRITE_STRING((PyUnicode_AsUTF8(value)));
            else if(!strcmp("entity", type))
            {
                edict_t *pEnt = ParseEnt(value);

                if(pEnt)
                    WRITE_ENTITY(ENTINDEX(pEnt));
            }

              Py_DECREF(data);
              Py_DECREF(index);
              Py_DECREF(begin);
              Py_DECREF(origin);
              Py_DECREF(owner);
        }

        MESSAGE_END();
    }

    Py_DECREF(msg);

    return Py_None;
}

FUNC(get_user_msg_id) {
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
    Py_DECREF(ent);

    if(entity) {
        Vector VecOrigin = g_vecZero;
        bool VecSetted = SetVector(VecOrigin, origin);

        if(VecSetted)
            SET_ORIGIN(entity, VecOrigin);
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

            if(pEntOwner != NULL)
                pEnt->v.owner = pEntOwner;
        }

        DispatchSpawn(pEnt);
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
    Py_DECREF(ent);

    if(entity)
        SET_MODEL(entity, model);

    return Py_None;
}

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
    REG(cvar_set_float, "");
    REG(cvar_set_string, "");
    REG(find_entity_by_classname, "");
    REG(get_user_msg_id, "");
    REG(send_message, "");

    CreateEngineModule();
}



