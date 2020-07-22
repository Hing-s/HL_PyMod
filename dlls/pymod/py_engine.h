#include "extdll.h"
#include "h_export.h"
#include "../Python/Include/Python.h"

#define MAX_METHODS 64

static PyMethodDef methods[MAX_METHODS];
static int size = 0;

#define FUNC(X) PyObject *X(PyObject *self, PyObject *args)

FUNC(PrecacheModel);
FUNC(PrecacheSound);
FUNC(SetModel);
FUNC(ModelIndex);
FUNC(ModelFrames);
FUNC(SetSize);
FUNC(Changelevel);
FUNC(GetSpawnParams);
FUNC(SaveSpawnParams);
FUNC(VecToYaw);
FUNC(VecToAngles);
FUNC(MoveToOrigin);
FUNC(ChangeYaw);
FUNC(ChangePitch);
FUNC(FindEnitityByString);
FUNC(GetEntityIllum);
FUNC(FindEnitityInSphere);
FUNC(FindClientInPVS);
FUNC(EntitiesInPVS);
FUNC(MakeVectors);
FUNC(AngleVectors);
FUNC(CreateEntity);
FUNC(RemoveEntity);
FUNC(CreateNamedEntity);
FUNC(MakeStatic);
FUNC(EntIsOnFloor);
FUNC(DropToFloor);
FUNC(WalkMove);
FUNC(SetOrigin);
FUNC(EmitSound);
FUNC(EmitAmbientSound);
FUNC(TraceLine);
FUNC(TraceToss);
FUNC(TraceMonsterHull);
FUNC(TraceHull);
FUNC(TraceModel);
FUNC(TraceTexture);
FUNC(TraceSphere);
FUNC(GetAimVector);
FUNC(ServerCommand);
FUNC(ServerExecute);
FUNC(ClientCommand);
FUNC(ParticleEffect);
FUNC(LightStyle);
FUNC(DecalIndex);
FUNC(PointContents);
FUNC(MessageBegin);
FUNC(MessageEnd);
FUNC(WriteByte);
FUNC(WriteChar);
FUNC(WriteShort);
FUNC(WriteLong);
FUNC(WriteAngle);
FUNC(WriteCoord);
FUNC(WriteString);
FUNC(WriteEntity);
FUNC(CvarRegister);
FUNC(CvarGetFloat);
FUNC(CvarGetString);
FUNC(CvarSetFloat);
FUNC(CvarSetString);
FUNC(AlertMessage);
FUNC(EngineFprintf);
FUNC(PvAllocEntPrivateData);
FUNC(PvEntPrivateData);
FUNC(FreeEntPrivateData);
FUNC(SzFromIndex);
FUNC(AllocEngineString);
FUNC(GetVarsOfEnt);
FUNC(PEntityOfEntOffset);
FUNC(EntOffsetOfPEntity);
FUNC(IndexOfEdict);
FUNC(PEntityOfEntIndex);
FUNC(FindEntityByVars);
FUNC(GetModelPtr);
FUNC(RegUserMsg);
FUNC(AnimationAutomove);
FUNC(GetBonePosition);
FUNC(FunctionFromName);
FUNC(NameForFunction);
FUNC(ClientPrintF);
FUNC(ServerPrint);
FUNC(Cmd_Args);
FUNC(Cmd_Argv);
FUNC(Cmd_Argc);
FUNC(GetAttachment);
FUNC(CRC32_Init);
FUNC(CRC32_ProcessBuffer);
FUNC(CRC32_ProcessByte);
FUNC(CRC32_Final);
FUNC(RandomLong);
FUNC(RandomFloat);
FUNC(SetView);
FUNC(Time);
FUNC(CrosshairAngle);
FUNC(LoafFileForMe);
FUNC(FreeFile);
FUNC(EndSection);
FUNC(CompareFileTime);
FUNC(GetGameDir);
FUNC(Cvar_RegisterVariable);
FUNC(FadeClientVolume);
FUNC(SetClientMaxspeed);
FUNC(CreateFakeClient);
FUNC(RunPlayerMove);
FUNC(NumberOfEntities);
FUNC(GetInfoKeyBuffer);
FUNC(InfoKeyValue);
FUNC(SetInfoKeyValue);
FUNC(SetClientKeyValue);
FUNC(IsMapValid);
FUNC(StaticDecal);
FUNC(PrecacheGeneric);
FUNC(GetPlayerUserid);
FUNC(BuildSoundMsg);
FUNC(IsDedicatedServer);
FUNC(CvarGetPointer);
FUNC(GetPlayerWONId);

static void PyRegister(const char *name, PyObject *(*function) (PyObject*, PyObject *), const char *help)
{
    if(size >= MAX_METHODS-1)
        return;

    PyMethodDef methd = {name, function, METH_VARARGS, help};
    methods[size] = methd;
    size++;
}

static void SetVector(Vector &vec, PyObject *coords) {
    float x, y, z;

    if(!PyArg_ParseTuple(coords, "fff", &x, &y, &z))
        return;

    vec =  Vector(x, y, z);
}

static PyObject *GetVector(Vector vec) {
    PyObject *pyVec = PyTuple_New(3);

    PyTuple_SetItem(pyVec, 0, PyFloat_FromDouble((float)vec.x));
    PyTuple_SetItem(pyVec, 1, PyFloat_FromDouble((float)vec.y));
    PyTuple_SetItem(pyVec, 2, PyFloat_FromDouble((float)vec.z));

    return pyVec;
}

void PyInitEngine();