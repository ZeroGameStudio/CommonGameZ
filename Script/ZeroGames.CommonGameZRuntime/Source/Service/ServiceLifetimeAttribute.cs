// Copyright Zero Games. All Rights Reserved.

namespace ZeroGames.CommonGameZRuntime;

public enum EServiceLifetime : uint8
{
	/// <summary>
	/// Service is available during the whole lifetime of owner, including constructor.
	/// Can be safely cached after first fetch until owner goes out of scope.
	/// </summary>
	Static,
	
	/// <summary>
	/// Same as Static, but can be lazy created.
	/// Has visible difference to Static in replication case.
	/// Client should use GetServiceAsync() instead of GetService().
	/// Can be safely cached after first fetch until owner goes out of scope.
	/// </summary>
	Implicit,
	
	/// <summary>
	/// Service is available after owner initialized.
	/// Can be safely cached after first fetch until owner goes out of scope.
	/// </summary>
	Injected,
	
	/// <summary>
	/// Service is dynamically created and destroyed.
	/// Can be cached, but take care of invalidation.
	/// </summary>
	Dynamic,
}

[AttributeUsage(AttributeTargets.Class)]
public class ServiceLifetimeAttribute(EServiceLifetime lifetime) : Attribute
{
	public EServiceLifetime Lifetime { get; } = lifetime;
}


