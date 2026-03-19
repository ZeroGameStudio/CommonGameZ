// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface ICompoliteOwner<TSchema> where TSchema : class
{
	void RegisterCompolite(ICompolite<TSchema> compolite);
	void UnregisterCompolite(ICompolite<TSchema> compolite);
	T? GetCompolite<T>() where T : class;
	IEnumerable<ICompolite<TSchema>> Compolites { get; }
}


