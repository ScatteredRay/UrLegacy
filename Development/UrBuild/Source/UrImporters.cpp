#include "Core.h"
#include "UrImporters.h"
#include "Names.h"

#define BEGIN_BUILDVAR_MAP { BuildVar* CurrentVar = BuildVars;	\
	while(CurrentVar != NULL) { 

#define MAP_STRING(name, var) if(CurrentVar->VarName == name) { assert(CurrentVar->ValueType == BuildVar::Var_String); var = CurrentVar->StringVal; }

#define END_BUILDVAR_MAP CurrentVar = CurrentVar->Next; } }

void ImportMesh(BuildVar* BuildVars)
{
	char* Path = "";
	BEGIN_BUILDVAR_MAP
	MAP_STRING(Name_Path, Path)
	END_BUILDVAR_MAP

	printf("Importing Mesh: %s\n", Path);
	delete BuildVars;
}

void ImportTexture(BuildVar* BuildVars)
{
	char* Path = "";
	BEGIN_BUILDVAR_MAP
	MAP_STRING(Name_Path, Path)
	END_BUILDVAR_MAP

	printf("Importing Texture: %s\n", Path);
	delete BuildVars;
}

void RunImporter(Name ImportName, BuildVar* BuildVars)
{
#define MAP_IMPORTER(nam, imp) if(ImportName == nam) { imp(BuildVars); return; }
	MAP_IMPORTER(Name_Model, ImportMesh);
	MAP_IMPORTER(Name_Texture, ImportTexture);
#undef MAP_IMPORTER
}