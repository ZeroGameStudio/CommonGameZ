// Copyright Zero Games. All Rights Reserved.

using System.Reflection;

namespace ZeroGames.CommonGameZRuntime;

public readonly record struct ServiceId(string? Value)
{
	public bool Matches(ServiceId requiredId) => requiredId == default || this == requiredId;

	public string Value { get => field ?? string.Empty; } = Value;
}

// Service 表明类型是一个可动态查询的能力，你可以在任何“服务定位器”上查询一个服务。
//
// 与传统的接口类相比，服务有以下特性：
// 1. Service 不要求必须由对象本身实现，而是可以由它的组件或与其关联的任何对象来实现，这意味着一个对象的服务不是编译期决定，而是运行时决定。
// 2. Service 的查询逻辑由你调用的定位器决定，定位器的实现方有责任保证在一族定位器的每个实例（比如Actor和它的每个组件）上的查询结果一致。
//
// 何时实现服务：
// 1. 你使用“功能容器（如Actor）”而不是拥有明确的、静态的职责的实例来组织功能。
//
// 何时不应该实现服务：
// 1. 你的能力是自我驱动的（如挂载到引擎事件循环），而不是被调用的。
// 2. 你的用户需要关注能力的实际实现者是谁，而不能是由定位器查询的未知对象。
//
// 泛型参数 TSelf 的作用是让每个服务有独立的 IsServiceAvailable 状态。
// 实现这个接口的类必须显式标记 [ServiceLifetime] 特性。
public interface IService<TSelf> where TSelf : class, IService<TSelf>
{
	ServiceId ServiceId => default;
	bool IsServiceAvailable { get; }
}


