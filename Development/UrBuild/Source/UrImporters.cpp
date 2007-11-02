#include "Core.h"
#include "UrImporters.h"
#include "UParse.h"
#include "Names.h"

#define BEGIN_BUILDVAR_MAP { BuildVar* CurrentVar = BuildVars;	\
	while(CurrentVar != NULL) { 

#define MAP_STRING(name, var) if(CurrentVar->VarName == name) { assert(CurrentVar->ValueType == BuildVar::Var_String); var = CurrentVar->StringVal; }

#define END_BUILDVAR_MAP CurrentVar = CurrentVar->Next; } }

void ParseObjLine(char* Line)
{
	EatWhiteSpace(&Line);
	switch(Line[0])
	{
	case '#':
		// Comment!
		break;
	case 'v':
		if(Line[1] == 't')
		{
			// Texture Coord
		}
		else if(Line[1] == 'n')
		{
			// Vertex Normal
		}
		else if((Line[1] == ' ') || (Line[1] == '\t'))
		{
			// Vertex Coords
		}
		else
		{
			goto Unsupported;
		}
		break;

	case 'p':
		// Point
		break;
	case 'l':
		// Line
		break;
	case 'f':
		// Face
		break;
	case 'g':
		// Group
		break;
	case 's':
		// Smoothing Group
		break;
	case 'o':
		// Object Name
	Unsupported:
	default:
	BuildError("Unsupported Decleration in ObjFile");
		break;
	}
}

void ImportMesh(BuildVar* BuildVars, BuildState* State)
{
	char* Path = "";
	BEGIN_BUILDVAR_MAP
	MAP_STRING(Name_Path, Path)
	END_BUILDVAR_MAP

	char* SubDir = GetSubDirPath(State->CurrentPath, Path);
	File* ObjFile = File::OpenFile(SubDir);
	assert(ObjFile);
	
	char* Line = NULL;
	do
	{
		delete Line;
		Line = ObjFile->ReadLine();
		ParseObjLine(Line);
	}
	while(Line != NULL);


	delete SubDir;
	delete BuildVars;
}

void ImportTexture(BuildVar* BuildVars, BuildState* State)
{
	char* Path = "";
	BEGIN_BUILDVAR_MAP
	MAP_STRING(Name_Path, Path)
	END_BUILDVAR_MAP

	printf("Importing Texture: %s\n", Path);
	delete BuildVars;
}

void RunImporter(Name ImportName, BuildVar* BuildVars, BuildState* BState)
{
#define MAP_IMPORTER(nam, imp) if(ImportName == nam) { imp(BuildVars, BState); return; }
	MAP_IMPORTER(Name_Model, ImportMesh);
	MAP_IMPORTER(Name_Texture, ImportTexture);
#undef MAP_IMPORTER
}