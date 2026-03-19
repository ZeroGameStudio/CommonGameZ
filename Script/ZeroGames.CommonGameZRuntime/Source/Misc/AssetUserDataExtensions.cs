// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public static class AssetUserDataExtensions
{
	extension(IInterface_AssetUserData @this)
	{
		public T? GetAssetUserData<T>() where T : UAssetUserData, IStaticClass
		{
			UClass cls = T.StaticClass;
			return (T?)@this.GetAssetUserDataOfClass(cls);
		}
		
		public T EnsureAssetUserData<T>() where T : UAssetUserData, IStaticClass
		{
			UClass cls = T.StaticClass;
			if (!@this.HasAssetUserDataOfClass(cls))
			{
				@this.AddAssetUserDataOfClass(cls);
			}

			return (T)@this.GetAssetUserDataOfClass(cls)!;
		}
	}
}


