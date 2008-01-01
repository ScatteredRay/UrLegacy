#include "UPackage.h"
#include "UPlatform.h"

const char* PakDirectory = "..\\Packages\\*";
const char* PackageExt = ".ubs";

bool UPackage::Save()
{
	return false;
}

bool UPackage::Load()
{
	return false;
}

UPackage* LoadPackage(Name Pack)
{
	BinaryFile* PakFile = BinaryFile::OpenFile(GetSubDirPath(PakDirectory, Pack, PackageExt));
	return false;
}


void UPackageManager::SaveAll()
{
	for(int i=0; i<Packages.Num(); i++)
		Packages[i]->Save();
}

void UPackageManager::LoadList()
{
}

void UPackageManager::CompleteLoad()
{
	for(int i=0; i<Packages.Num(); i++)
		Packages[i]->Load();
}
