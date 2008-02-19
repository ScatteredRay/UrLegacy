#include "UPackage.h"
#include "UPlatform.h"
#include "Assert.h"

const char* PakDirectory = "..\\Packages\\*";
const char* PackageExt = ".ubs";
const uint	PackageSignature = 0xf31115ba;

UPackage::UPackage(Name Pkg) : PackageName(Pkg)
{
	
}

UPackage::~UPackage()
{

}

void UPackage::AddAsset(UAsset* Asset)
{
	Assets.AddItem(Asset);
}

void UPackage::RemoveAsset(UAsset* Asset)
{
	Assets.RemoveAllItem(Asset);
}

bool UPackage::Save()
{
	BinaryFile* PakFile = BinaryFile::OpenFile(GetSubDirPath(PakDirectory, PackageName, PackageExt), true);
	PkStream* Stream = new IPkStream(PakFile);
	Serialize(*Stream);
	return true;
}

bool UPackage::Load()
{
	if(bLoaded)
		return true;

	BinaryFile* PakFile = BinaryFile::OpenFile(GetSubDirPath(PakDirectory, PackageName, PackageExt), false);
	PkStream* Stream = new OPkStream(PakFile);
	Serialize(*Stream);
	return true;
}

void UPackage::Serialize(PkStream &Stream)
{
	uint Sign = PackageSignature;
	Stream << Sign;
	if (Sign != PackageSignature)
		return;
	
	uint NumAssets = Assets.Num();
	Stream << NumAssets;
	if(Stream.IsLoading())
		Assets.Add(NumAssets - Assets.Num());

	for(uint i=0; i<NumAssets; i++)
	{
		UAssetType Type;
		if(Stream.IsSaving())
			Type = Assets[i]->GetType();
		
		Stream << Type;

		if(Stream.IsLoading())
			Assets[i] = Type.CreateAsset();

		Assets[i]->Serialize(Stream);
	}

	bLoaded = true;
}

UPackage* UPackageManager::FindPackage(Name Pack)
{
	for(uint i=0; i<Packages.Num(); i++)
	{
		if(Packages[i]->GetName() == Pack)
			return Packages[i];
	}
	return NULL;
}

UAsset::UAsset(UPackage* Owner) : Package(Owner)
{
	assert(Package);
	Package->AddAsset(this);
}

UAsset::~UAsset()
{
	if(Package)
		Package->RemoveAsset(this);
}

UAsset* UAssetType::CreateAsset()
{
	assert(AssetID < Initializers.Num());
	assert(Initializers[AssetID]);
	assert(Initializers[AssetID]->Initialize);

	return Initializers[AssetID]->Initialize();

}

UAssetType UAssetType::AddInitializer(AssetInitializer* Initializer)
{
	if(Initializer->ID >= Initializers.Num())
	{
		Initializers.Add(1 + Initializer->ID - Initializers.Num());
	}

	Initializers[Initializer->ID] = Initializer;

	return UAssetType(Initializer->ID);
}

void UPackageManager::SaveAll()
{
	for(uint i=0; i<Packages.Num(); i++)
		Packages[i]->Save();
}

void UPackageManager::LoadList()
{
}

UPackage* UPackageManager::LoadPackage(Name Pack)
{
	UPackage* LoadedPackage = FindPackage(Pack);
	if(LoadedPackage == NULL)
	{
		LoadedPackage = new UPackage(Pack);
		Packages.AddItem(LoadedPackage);
	}
	LoadedPackage->Load();
	return LoadedPackage;
}

void UPackageManager::CompleteLoad()
{
	for(uint i=0; i<Packages.Num(); i++)
		Packages[i]->Load();
}

void IPkStream::Serialize(void* Src, psize Length)
{
	File->Read(Src, Length);
}

void OPkStream::Serialize(void* Src, psize Length)
{
	File->Write(Src, Length);
}