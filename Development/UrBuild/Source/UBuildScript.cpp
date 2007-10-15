#include "UBuildScript.h"
#include "Core.h"
#include "UPlatform.h"
#include "UMemory.h"
#include <stdio.h>

const char* ContentDirectory = "..\\Content\\*";
const char* BuildScriptExt = ".ubs";

char* GetSubDirPath(const char* Dir, const char* SubDir)
{
			psize CDirLen = appStrlen(Dir);
			psize FileNameLen = appStrlen(SubDir);
			psize DirBufLen = CDirLen + FileNameLen + 2; // - ('*') + ('\\', '*', '\0')
			char* NewDir = (char*)appMalloc(DirBufLen * sizeof(char));
			appStrcpy(NewDir, Dir);
			appStrcpy(NewDir+CDirLen-1, SubDir);
			appStrcpy(NewDir+CDirLen+FileNameLen-1, "\\*");
			return NewDir;
}

bool HasExtension(const char* Dir, const char* Ext)
{
	size_t DirLen = appStrlen(Dir);
	size_t ExtLen = appStrlen(Ext);
	return (appStricmp(Dir+DirLen-ExtLen, Ext) == 0);
}

void RunBuildScriptsFor(const char* CurrDir)
{
	DirIterator ContentDir(CurrDir);
	do
	{
		if(ContentDir.isCurOrPrevDir())
			continue;
		if(ContentDir.isDirectory())
		{
			char* SubDir = GetSubDirPath(CurrDir, ContentDir.FileName());
			RunBuildScriptsFor(SubDir);
			delete SubDir;
		}
		else if(HasExtension(ContentDir.FileName(), BuildScriptExt))
		{
			RunBuildScript(ContentDir.FileName());
		}
	}
	while(ContentDir.Next());
}

void RunBuildScripts()
{
}

void RunBuildScript(const char* Filename)
{

}