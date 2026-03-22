// Copyright Zero Games. All Rights Reserved.

using System.Diagnostics.CodeAnalysis;

namespace ZeroGames.CommonGameZRuntime;

public interface ICompoliteOwner
{
	bool RegisterCompolite(Type? type, ICompoliteFactory? factory, [NotNullWhen(true)] out ICompolite? compolite);
	bool UnregisterCompolite(ICompolite compolite);
	bool GetCompolite(Type type, [NotNullWhen(true)] out ICompolite? compolite);
	
	IEnumerable<ICompolite> Compolites { get; }
}

public static class CompoliteOwnerExtensions
{
	extension(ICompoliteOwner @this)
	{
		public bool RegisterCompolite<T>([NotNullWhen(true)] out T? compolite) where T : class, ICompolite
			=> @this.RegisterCompolite(null, out compolite);
		
		public bool RegisterCompolite<T>(ICompoliteFactory? factory, [NotNullWhen(true)] out T? compolite) where T : class, ICompolite
			=> (compolite = @this.RegisterCompolite(typeof(T), factory, out var comp) ? (T?)comp : null) is not null;

		public T? RegisterCompolite<T>() where T : class, ICompolite
			=> @this.RegisterCompolite<T>(out var compolite) ? compolite : null;
		
		public T? RegisterCompolite<T>(ICompoliteFactory? factory) where T : class, ICompolite
			=> @this.RegisterCompolite<T>(factory, out var compolite) ? compolite : null;

		public bool GetCompolite<T>([NotNullWhen(true)] out T? compolite) where T : class, ICompolite
			=> (compolite = @this.GetCompolite(typeof(T), out var comp) ? (T?)comp : null) is not null;

		public T? GetCompolite<T>() where T : class, ICompolite
			=> @this.GetCompolite<T>(out var compolite) ? compolite : null;
	}
}


