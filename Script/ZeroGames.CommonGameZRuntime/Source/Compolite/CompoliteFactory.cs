// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public class CompoliteFactory : ICompoliteFactory
{
	public static readonly CompoliteFactory Instance = new();
	
	private CompoliteFactory(){}
	
	#region ICompoliteFactory Implementations
	
	public bool TryAllocateCompolite(Type type, object? owner, [NotNullWhen(true)] out ICompolite? compolite)
	{
		throw new NotImplementedException();
	}
	
	#endregion
}


