#include "UBuildScript.h"
#include "Core.h"
#include "UPlatform.h"
#include "UMemory.h"
#include <stdio.h>

const char* ContentDirectory = "..\\Content\\*";

char* GetSubDirPath(const char* Dir, const char* SubDir)
{
			psize CDirLen = appStrlen(Dir);
			psize FileNameLen = appStrlen(SubDir);
			psize DirBufLen = CDirLen + FileNameLen + 2; // - ('*') + ('\\', '*', '\0')
			char* NewDir = (char*)appStackAlloc(DirBufLen * sizeof(char));
			appStrcpy(NewDir, Dir);
			appStrcpy(NewDir+CDirLen-1, SubDir);
			appStrcpy(NewDir+CDirLen+FileNameLen-1, "\\*");
			return NewDir;
}

void RunBuildScripts()
{
	DirIterator ContentDir(ContentDirectory);
	do
	{
		if(ContentDir.isCurOrPrevDir())
			continue;
		printf("IsDirectory: %s Name: '%s'\n", (ContentDir.isDirectory() ? "true" : "false"), ContentDir.FileName());
		if(ContentDir.isDirectory())
		{
			char* Dir = GetSubDirPath(ContentDirectory, ContentDir.FileName());
			DirIterator SubContentDir(Dir);
			do
			{
				if(SubContentDir.isCurOrPrevDir())
					continue;
				printf("	IsDirectory: %s Name: '%s'\n", (SubContentDir.isDirectory() ? "true" : "false"), SubContentDir.FileName());
			}
			while(SubContentDir.Next());
			delete Dir;
		}
	}
	while(ContentDir.Next());
}