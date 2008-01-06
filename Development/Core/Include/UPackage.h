#ifndef _UPACKAGE_H_
#define _UPACKAGE_H_
#include "UTypes.h"
#include "Name.h"

class PkStream
{
public:
	virtual void Serialize(void* Src, psize Length)=0;
	template<typename T>
	PkStream& operator <<(T& Src)
	{
		Serialize(&Src, sizeof(T));
		return *this;
	}
	virtual bool IsLoading()=0;
	virtual bool IsSaving()=0;
};

class UPackage
{
	KArray<class UAsset*> Assets;
	Name PackageName;
	bool bLoaded;
public:
	UPackage(Name Pkg) : PackageName(Pkg){}
	bool Save();
	bool Load();
	void Serialize(PkStream& Stream);
	Name Name()
	{
		return PackageName;
	}
};

class UAsset
{
	UPackage* Package;
};

class UPackageManager
{
	KArray<UPackage*> Packages;
public:
	UPackage* FindPackage(Name Pack);
	void SaveAll();
	void LoadList();
	UPackage* LoadPackage(Name Pack);
	void CompleteLoad();
};

class IPkStream : public PkStream
{
	BinaryFile* File;
public:
	IPkStream(BinaryFile* file) : File(file) {}
	virtual void Serialize(void* Src, psize Length);
	virtual bool IsLoading()
	{return true;}
	virtual bool IsSaving()
	{return false;}
};

class OPkStream : public PkStream
{
	BinaryFile* File;
public:
	OPkStream(BinaryFile* file) : File(file) {}
	virtual void Serialize(void* Src, psize Length);
	virtual bool IsLoading()
	{return false;}
	virtual bool IsSaving()
	{return true;}
};

extern UPackageManager* GPackageManager;

#endif //_UPACKAGE_H_