// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public interface ICompoliteFactory
{
	bool TryAllocateCompolite(Type type, object? owner, [NotNullWhen(true)] out ICompolite? compolite);
}


