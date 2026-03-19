// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public interface ICompolite<TSchema> : ICompoliteLifecycle<TSchema> where TSchema : class
{
	TSchema Owner { get; }
}


