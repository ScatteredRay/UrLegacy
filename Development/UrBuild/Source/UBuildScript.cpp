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
	RunBuildScriptsFor(ContentDirectory);
}

struct Token* GetBuildToken(const char* CurrentChar);

enum BuildToken
{
	TOK_Invalid,
	TOK_EOF,
	TOK_LParen,
	TOK_RParen,
	TOK_Equal,
	TOK_ID,
	TOK_String,
	TOK_Int,
	TOK_Float
};

struct Token
{
	BuildToken Tok;
	Token() : Tok(TOK_Invalid) {}
};

void RunBuildScript(const char* Filename)
{
	File* ScriptFile = File::OpenFile(Filename);
	char* ScriptBuffer = ScriptFile->ReadAll();
	char* ScriptChar = ScriptBuffer;
	delete ScriptFile;

	Token* CurrentToken = NULL;
	do
	{
		delete CurrentToken;
		CurrentToken = GetBuildToken(ScriptChar);
	}
	while(CurrentToken->Tok != TOK_EOF);

}

#define caseAlpha \
case 'a': case 'b': case 'c': case 'd': \
case 'e': case 'f': case 'g': case 'h': \
case 'i': case 'j': case 'k': case 'l': \
case 'm': case 'n': case 'o': case 'p': \
case 'q': case 'r': case 's': case 't': \
case 'u': case 'v': case 'w': case 'x': \
case 'y': case 'z': case 'A': case 'B': \
case 'C': case 'D': case 'E': case 'F': \
case 'G': case 'H': case 'I': case 'J': \
case 'K': case 'L': case 'M': case 'N': \
case 'O': case 'P': case 'Q': case 'R': \
case 'S': case 'T': case 'U': case 'V': \
case 'W': case 'X': case 'Y': case 'Z'

#define caseNumeric \
case '0': case '1': case '2': case '3': \
case '4': case '5': case '6': case '7': \
case '8': case '9'

Token* GetBuildToken(const char* CurrentChar)
{
	Token* CurrentToken = new Token();
	while(	CurrentChar[0] == ' ' ||
			CurrentChar[0] == '\t' ||
			CurrentChar[0] == '\n')
	{
		CurrentChar++;
	}

	switch(CurrentChar[0])
	{
	case '\0':
		CurrentToken->Tok = TOK_EOF;
		break;
	case '(':
		CurrentToken->Tok = TOK_LParen;
		break;
	case ')':
		CurrentToken->Tok = TOK_RParen;
		break;
	case '=':
		CurrentToken->Tok = TOK_Equal;
		break;
	caseAlpha:
		CurrentToken->Tok = TOK_ID;
		break;
	case '"':
		CurrentToken->Tok = TOK_String;
		break;
	caseNumeric:
		CurrentToken->Tok = TOK_Int;
		CurrentToken->Tok = TOK_Float;
		break;

	}

	assert(CurrentToken->Tok != TOK_Invalid);
	return CurrentToken;
}