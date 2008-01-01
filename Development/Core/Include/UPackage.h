#ifndef _UPACKAGE_H_
#define _UPACKAGE_H_
#include "UTypes.h"
#include "Name.h"

class UPackage
{
	KArray<class UAsset*> Assets;
	Name PackageName;
	bool bLoaded;
public:
	bool Save();
	bool Load();
};

UPackage* LoadPackage(Name Pack);

class UAsset
{
	UPackage* Package;
};

class UPackageManager
{
	KArray<UPackage*> Packages;
public:
	void SaveAll();
	void LoadList();
	void CompleteLoad();
};

extern UPackageManager* GPackageManager;

#endif //_UPACKAGE_H_