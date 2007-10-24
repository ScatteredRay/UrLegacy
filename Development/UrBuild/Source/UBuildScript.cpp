#include "UBuildScript.h"
#include "Core.h"
#include "UPlatform.h"
#include "UMemory.h"
#include <stdio.h>

const char* ContentDirectory = "..\\Content\\*";
const char* BuildScriptExt = ".ubs";

//#define BuildError(...) {printf(__VA_ARGS__); throw "Compile Error";}
#define BuildError printf

char* GetSubDirPath(const char* Dir, const char* SubDir, const char* Ext)
{
			psize CDirLen = appStrlen(Dir);
			psize FileNameLen = appStrlen(SubDir);
			psize ExtLen = appStrlen(Ext);
			psize DirBufLen = CDirLen + FileNameLen + ExtLen; // - ('*') + ('\\', '*', '\0')
			char* NewDir = (char*)appMalloc(DirBufLen * sizeof(char));
			appStrcpy(NewDir, Dir);
			appStrcpy(NewDir+CDirLen-1, SubDir);
			appStrcpy(NewDir+CDirLen+FileNameLen-1, Ext);
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
			char* SubDir = GetSubDirPath(CurrDir, ContentDir.FileName(), "\\*");
			RunBuildScriptsFor(SubDir);
			delete SubDir;
		}
		else if(HasExtension(ContentDir.FileName(), BuildScriptExt))
		{
			char* ScriptPath = GetSubDirPath(CurrDir, ContentDir.FileName(), "");
			RunBuildScript(ScriptPath);
			delete ScriptPath;
		}
	}
	while(ContentDir.Next());
}

void RunBuildScripts()
{
	RunBuildScriptsFor(ContentDirectory);
}

struct Token* GetBuildToken(char** CurrentChar);

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

inline const char* BuildTokenString(BuildToken Tok)
{
#define MapToken( tkn, str) case tkn: return str;
	switch(Tok)
	{
		MapToken(TOK_Invalid, "Invalid");
		MapToken(TOK_EOF, "End of File");
		MapToken(TOK_LParen, "Open Paren");
		MapToken(TOK_RParen, "Close Paren");
		MapToken(TOK_Equal, "Equals");
		MapToken(TOK_ID, "Identifier");
		MapToken(TOK_String, "String");
		MapToken(TOK_Int, "Int");
		MapToken(TOK_Float, "Float");
	}
	return "Bad Token";
#undef MapToken
}

struct Token
{
	BuildToken Tok;
	union
	{
		char* String;
		int Int;
		float Float;
	};
	Token() : Tok(TOK_Invalid), String(NULL) {}
	~Token()
	{
		if(Tok == TOK_ID || Tok == TOK_String)
			delete String;
	}
};

struct BuildVar
{
	char* VarName;
	enum 
	{
		Var_None,
		Var_String,
		Var_Int,
		Var_Float
	} ValueType;
	union
	{
		char* StringVal;
		int IntVal;
		float FloatVal;
	};
	BuildVar* Next;
	BuildVar() : VarName(NULL), Next(NULL), ValueType(Var_None) {}
	~BuildVar()
	{
		delete VarName;
		if(ValueType == Var_String)
			delete StringVal;
		delete Next;
	}
};

BuildVar* ParseBuildVar(char** ScriptChar)
{
	BuildVar* CurrentVar = new BuildVar();
	return CurrentVar;
}

BuildVar* ParseBuildVars(char** ScriptChar)
{
	BuildVar* Vars = NULL;
	BuildVar** Next = &Vars;
	Token* CurrentToken;
	bool bContinue = true;
	while(bContinue)
	{
		CurrentToken = GetBuildToken(ScriptChar);
		switch(CurrentToken->Tok)
		{
		case TOK_LParen:
			(*Next) = ParseBuildVars(ScriptChar);
			if((*Next) != NULL)
				Next = &((*Next)->Next);
			break;
		case TOK_ID:
			(*Next) = ParseBuildVar(ScriptChar);
			if((*Next) != NULL)
				Next = &((*Next)->Next);
			break;
		case TOK_RParen:
			bContinue = false;
			break;
		}
		delete CurrentToken;
	}
	return Vars;
}

bool ParseBuildAsset(char** ScriptChar)
{
	Token* CurrentToken = GetBuildToken(ScriptChar);
	switch(CurrentToken->Tok)
	{
	case TOK_ID:
		ParseBuildVars(ScriptChar);
		break; 
	case TOK_EOF:
		delete CurrentToken;
		return false;
	default:
		BuildError("Unexpected '%s' in input stream!", BuildTokenString(CurrentToken->Tok));
		break;
	}
	delete CurrentToken;
	return true;
}

void RunBuildScript(const char* Filename)
{
	File* ScriptFile = File::OpenFile(Filename);
	char* ScriptBuffer = ScriptFile->ReadAll();
	char* ScriptChar = ScriptBuffer;
	delete ScriptFile;

	while(ParseBuildAsset(&ScriptChar)){}

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

bool IsNumeric(char c)
{
	return (c >= '0' && c <= '9');
}

bool IsAlphaNumeric(char c)
{
	return ((c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		IsNumeric(c));
}

Token* GetBuildToken(char** CurrentChar)
{
	char* Char = *CurrentChar;
	int TokenLen = 0;
	Token* CurrentToken = new Token();
	while(	Char[0] == ' ' ||
			Char[0] == '\t' ||
			Char[0] == '\n')
	{
		Char++;
		(*CurrentChar)++;
	}

	switch(Char[0])
	{
	case '\0':
		CurrentToken->Tok = TOK_EOF;
		TokenLen++;
		break;
	case '(':
		CurrentToken->Tok = TOK_LParen;
		TokenLen++;
		break;
	case ')':
		CurrentToken->Tok = TOK_RParen;
		TokenLen++;
		break;
	case '=':
		CurrentToken->Tok = TOK_Equal;
		TokenLen++;
		break;
	caseAlpha:
		CurrentToken->Tok = TOK_ID;
		TokenLen++;
		while(IsAlphaNumeric(Char[TokenLen]))
		{
			TokenLen++;
		}
		{
			char* str = new char[TokenLen+1];
			appStrncpy(str, Char, TokenLen);
			CurrentToken->String = str;
		}
		break;
	case '"':
		{
			CurrentToken->Tok = TOK_String;
			TokenLen++;
			size_t StringLen = 0;
			while(true)
			{
				if(Char[TokenLen] == '"')
					break;
				else if(Char[TokenLen] == '\\')
				{
					TokenLen++;
				}
				StringLen++;
				TokenLen++;
			}
			TokenLen++;
			{
				char* str = new char[StringLen+1];
				int j = 0;
				for(int i=1; i<=(TokenLen-2); i++)
				{
					if(Char[i] == '\\')
						continue;
					else
					{
						str[j] = Char[i];
						j++;
					}
						
				}
				str[StringLen] = '\0';
				CurrentToken->String = str;
			}
		}
		break;
	case '+':
	case '-':
	caseNumeric:
		{
			bool bIsFloat = false;
			TokenLen++;
			while(IsNumeric(Char[TokenLen]))
				TokenLen++;
			if(Char[TokenLen] == '.')
			{
				TokenLen++;
				bIsFloat = true;
			}
			while(IsNumeric(Char[TokenLen]))
				TokenLen++;
			
			if(bIsFloat)
				CurrentToken->Tok = TOK_Float;
			else
				CurrentToken->Tok = TOK_Int;
			
			{
				char* str = new char[TokenLen+1];
				str = appStrncpy(str, Char, TokenLen);
				if(bIsFloat)
					CurrentToken->Float = (float)atof(str);
				else
					CurrentToken->Int = atoi(str);
				delete str;
			}
		}
		break;
	default:
		BuildError("Unrecogonized char '%c' in file.\n", Char[0]); 
	}

	*CurrentChar += TokenLen;
	assert(CurrentToken->Tok != TOK_Invalid);
	return CurrentToken;
}