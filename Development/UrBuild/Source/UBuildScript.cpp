#include "UBuildScript.h"
#include "Core.h"
#include "UPlatform.h"
#include "UMemory.h"
#include "Names.h"
#include "UParse.h"
#include "UrImporters.h"
#include <stdio.h>

const char* ContentDirectory = "..\\Content\\*";
const char* BuildScriptExt = ".ubs";

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
			char* ScriptPath = GetSubDirPath(CurrDir, ContentDir.FileName());
			BuildState* CurrentState = new BuildState(CurrDir);
			RunBuildScript(ScriptPath, CurrentState);
			delete CurrentState;
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
	Name IdName; //TODO: Didn't want to be in union, copy constructor?
	union
	{
		char* String;
		int Int;
		float Float;
	};
	Token() : Tok(TOK_Invalid), String(NULL) {}
	~Token()
	{
		if(Tok == TOK_String)
			delete String;
	}
};

BuildVar* ParseBuildVar(char** ScriptChar, Name VarName)
{
	BuildVar* CurrentVar = new BuildVar();
	CurrentVar->VarName = VarName;
	Token* CurrentToken = GetBuildToken(ScriptChar);
	if(CurrentToken->Tok != TOK_Equal)
		BuildError("Expected '=' in variable decleration");
	delete CurrentToken;
	CurrentToken = GetBuildToken(ScriptChar);
	switch(CurrentToken->Tok)
	{
	case TOK_String:
		CurrentVar->ValueType = BuildVar::Var_String;
		CurrentVar->StringVal = CurrentToken->String;
		CurrentToken->String = NULL; // So that it doesn't get deleted.
		break;
	case TOK_Int:
		CurrentVar->ValueType = BuildVar::Var_Int;
		CurrentVar->IntVal = CurrentToken->Int;
		break;
	case TOK_Float:
		CurrentVar->ValueType = BuildVar::Var_Float;
		CurrentVar->FloatVal = CurrentToken->Float;
		break;
	default:
		BuildError("Unexpected Token in variable decleration");
		break;
	}
	delete CurrentToken;
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
			(*Next) = ParseBuildVar(ScriptChar, CurrentToken->IdName);
			if((*Next) != NULL)
				Next = &((*Next)->Next);
			break;
		case TOK_RParen:
			bContinue = false;
			break;
		case TOK_EOF:
			BuildError("Unexpected EOF in Var def.\n");
			bContinue = false;
			break;
		default:
			BuildError("Unexpected Token.\n");
			bContinue = false;
			break;
		}
		delete CurrentToken;
	}
	return Vars;
}

bool ParseBuildAsset(char** ScriptChar, BuildState* BState)
{
	Token* CurrentToken = GetBuildToken(ScriptChar);
	switch(CurrentToken->Tok)
	{
	case TOK_ID:
		{
			Token* NextToken = GetBuildToken(ScriptChar);
			if(NextToken->Tok != TOK_LParen)
				BuildError("Expected '(' after Build Decleration\n");
			BuildVar* Vars = ParseBuildVars(ScriptChar);
			RunImporter(CurrentToken->IdName, Vars, BState);
		}
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

void RunBuildScript(const char* Filename, BuildState* BState)
{
	File* ScriptFile = File::OpenFile(Filename);
	char* ScriptBuffer = ScriptFile->ReadAll();
	char* ScriptChar = ScriptBuffer;
	delete ScriptFile;

	while(ParseBuildAsset(&ScriptChar, BState)){}

}

Token* GetBuildToken(char** CurrentChar)
{
	EatWhiteSpace(CurrentChar);
	char* Char = *CurrentChar;
	int TokenLen = 0;
	Token* CurrentToken = new Token();


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
			CurrentToken->IdName = Name(str);
			delete str;
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