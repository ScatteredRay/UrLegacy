#include "UParse.h" 
#include "UPlatform.h"

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

bool ParseInt(char** Char, int& ParsedInt)
{
	size_t TokenLen = 0;
	//ParsedInt = 0;
	while(IsNumeric((*Char)[TokenLen]))
		TokenLen++;
	
	if(TokenLen>0)
	{
		char* str = new char[TokenLen+1];
		str = appStrncpy(str, (*Char), TokenLen);
		ParsedInt = atoi(str);
		delete str;
		(*Char) += TokenLen;
		return true;
	}
	return false;
}

bool ParseFloat(char** Char, float& ParsedFloat)
{
	size_t TokenLen = 0;
	//ParsedFloat = 0.0f;
	while(IsNumeric((*Char)[TokenLen]))
		TokenLen++;
	if((*Char)[TokenLen] == '.')
	{
		TokenLen++;
	}
	while(IsNumeric((*Char)[TokenLen]))
		TokenLen++;
	
	if(TokenLen>0)
	{
		char* str = new char[TokenLen+1];
		str = appStrncpy(str, (*Char), TokenLen);
		ParsedFloat = (float)atof(str);
		delete str;
		(*Char) += TokenLen;
		return true;
	}
	return false;
}

void EatWhiteSpace(char** Char)
{
	while(	(*Char)[0] == ' ' ||
			(*Char)[0] == '\t' ||
			(*Char)[0] == '\n')
	{
		(*Char)++;
	}
}
