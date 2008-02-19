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
	UPackage(Name Pkg);
	~UPackage();
	bool Save();
	bool Load();
	void Serialize(PkStream& Stream);
	void AddAsset(UAsset* Asset);
	void RemoveAsset(UAsset* Asset);
	Name GetName()
	{
		return PackageName;
	}
};

struct AssetInitializer
{
	uint ID;
	UAsset* (*Initialize)();
	AssetInitializer(uint id, UAsset* (*initialize)()) : ID(id), Initialize(initialize) {}
};

class UAssetType
{
	uint AssetID;
	static KArray<AssetInitializer*> Initializers;
public:
	UAssetType() : AssetID(0){}
	UAssetType(uint ID) : AssetID(ID){}
	UAsset* CreateAsset();
	static UAssetType AddInitializer(AssetInitializer* Initializer);
};

#define DECLARE_INITIALIZER(num, cls) static UAsset* CreateNew##cls(){return new cls();} \
	static UAssetType cls##Type = UAssetType::AddInitializer(new AssetInitializer(num, &CreateNew##cls)); \
	virtual UAssetType GetType() { return cls##Type; }

class UAsset
{
	UPackage* Package;
public:
	UAsset(UPackage* Owner);
	~UAsset();
	virtual void Serialize(PkStream& Stream)=0;
	virtual UAssetType GetType()=0;
};

class UPackageManager
{
	KArray<UPackage*> Packages;
public:
	UPackage* FindPackage(Name Pack);
	void AddPackage(UPackage* Pack);
	void RemovePackage(UPackage* Pack);
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