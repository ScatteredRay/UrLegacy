#include "Core.h"
#include "UrImporters.h"
#include "Names.h"

void ImportMesh(BuildVar* BuildVars)
{
	printf("Importing mesh\n");
}

void ImportTexture(BuildVar* BuildVars)
{
	printf("Importing Texture\n");
}

void RunImporter(Name ImportName, BuildVar* BuildVars)
{
#define MAP_IMPORTER(nam, imp) if(ImportName == nam) { imp(BuildVars); return; }
	MAP_IMPORTER(Name_Model, ImportMesh);
	MAP_IMPORTER(Name_Texture, ImportTexture);
#undef MAP_IMPORTER
}