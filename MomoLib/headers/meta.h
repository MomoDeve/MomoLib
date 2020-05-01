#pragma once

#include <iostream>

/*
This file contains useful classes for metaprogramming from Alexandrescu book (see Modern C++ design)
It includes: TypeList class with bunch of methods (Length, Get, Find, Erase, Replace, Unique),
TypeList and other types printing with print_list and typeof<T>
ScatterHierarchy template generator and most derived class search
*/
namespace momo
{
	template<typename T>
	const char* typeof()
	{
		return typeid(T).name();
	}

	struct NullType { };

	template<typename T, typename U>
	struct TypeList
	{
		using Head = T;
		using Tail = U;
	};

	template<typename T>
	struct TypeList<T, NullType>
	{
		using Head = T;
		using IsEnd = NullType;
	};

	// list declaration

	// stores any but not zero types
	// usage : using MyList = ToList<char, float, int>::Result;
	template<typename T, typename... Args>
	struct ToList
	{
		using Result = TypeList<T, typename ToList<Args...>::Result>;
	};

	template<typename T>
	struct ToList<T>
	{
		using Result = TypeList<T, NullType>;
	};

	// length of list

	template<typename T>
	struct Length;

	template<>
	struct Length<NullType>
	{
		enum { value = 0 };
	};

	template<typename T, typename U>
	struct Length<TypeList<T, U>>
	{
		enum { value = 1 + Length<U>::value };
	};

	// get element of list

	template<size_t Idx, typename List>
	struct Get;

	template<size_t Idx, typename T, typename U>
	struct Get<Idx, TypeList<T, U>>
	{
		using Result = typename Get<Idx - 1, U>::Result;
	};

	template<typename T, typename U>
	struct Get<0, TypeList<T, U>>
	{
		using Result = T;
	};

	// get index of element

	template<typename Type, typename List>
	struct IndexOf;

	template<typename Type>
	struct IndexOf<Type, NullType>
	{
		enum { value = -1 };
	};

	template<typename Type, typename T, typename U>
	struct IndexOf<Type, TypeList<T, U>>
	{
	private:
		enum { idx = IndexOf<Type, U>::value };
	public:
		enum { value = (idx == -1) ? -1 : 1 + idx };
	};

	template<typename Type, typename U>
	struct IndexOf<Type, TypeList<Type, U>>
	{
		enum { value = 0 };
	};

	template<typename List, typename Type>
	struct Append;

	// [empty] [empty]
	template<>
	struct Append<NullType, NullType>
	{
		using Result = NullType;
	};

	// [list_impl] [empty]
	template<typename T, typename  U>
	struct Append<TypeList<T, U>, NullType>
	{
		using Result = TypeList<T, U>;
	};

	// [list_impl[1]] [empty]
	template<typename Type, typename T>
	struct Append<TypeList<T, NullType>, Type>
	{
		using Result = TypeList<T, TypeList<Type, NullType>>;
	};
	// [list_impl] [list_impl]
	template<typename T1, typename U1, typename T2, typename U2>
	struct Append<TypeList<T1, U1>, TypeList<T2, U2>>
	{
	private:
		using List = typename Append<TypeList<T1, U1>, T2>::Result;
	public:
		using Result = typename Append<List, U2>::Result;
	};

	// [list_impl] [T]
	template<typename T, typename U, typename Type>
	struct Append<TypeList<T, U>, Type>
	{
		using Result = TypeList<T, typename Append<U, Type>::Result>;
	};

	template<typename TList, typename Type>
	struct Erase;

	template<typename Type>
	struct Erase<NullType, Type>
	{
		using Result = NullType;
	};

	template<typename Type, typename U>
	struct Erase<TypeList<Type, U>, Type>
	{
		using Result = U;
	};

	template<typename T, typename U, typename Type>
	struct Erase<TypeList<T, U>, Type>
	{
		using Result = TypeList<T, typename Erase<U, Type>::Result>;
	};

	template<typename TList, typename Type>
	struct EraseAll;

	template<typename Type>
	struct EraseAll<NullType, Type>
	{
		using Result = NullType;
	};

	template<typename Type, typename U>
	struct EraseAll<TypeList<Type, U>, Type>
	{
		using Result = typename EraseAll<U, Type>::Result;
	};

	template<typename T, typename U, typename Type>
	struct EraseAll<TypeList<T, U>, Type>
	{
		using Result = TypeList<T, typename EraseAll<U, Type>::Result>;
	};

	template<typename TList>
	struct Unique;

	template<>
	struct Unique<NullType>
	{
		using Result = NullType;
	};

	template<typename T, typename U>
	struct Unique<TypeList<T, U>>
	{
	private:
		using Tail = typename Unique<U>::Result;
		using UniqueTail = typename EraseAll<Tail, T>::Result;
	public:
		using Result = TypeList<T, UniqueTail>;
	};

	template<typename TList, typename From, typename To>
	struct Replace;

	template<typename From, typename To>
	struct Replace<NullType, From, To>
	{
		using Result = NullType;
	};

	template<typename From, typename To, typename U>
	struct Replace<TypeList<From, U>, From, To>
	{
		using Result = TypeList<To, U>;
	};

	template<typename From, typename To, typename T, typename U>
	struct Replace<TypeList<T, U>, From, To>
	{
		using Result = TypeList<T, typename Replace<U, From, To>::Result>;
	};

	template<typename TList, typename From, typename To>
	struct ReplaceAll;

	template<typename From, typename To>
	struct ReplaceAll<NullType, From, To>
	{
		using Result = NullType;
	};

	template<typename From, typename To, typename U>
	struct ReplaceAll<TypeList<From, U>, From, To>
	{
		using Result = TypeList<To, typename ReplaceAll<U, From, To>::Result>;
	};

	template<typename From, typename To, typename T, typename U>
	struct ReplaceAll<TypeList<T, U>, From, To>
	{
		using Result = TypeList<T, typename ReplaceAll<U, From, To>::Result>;
	};

	template<typename T>
	void print_list(typename T::Head = typename T::Head(), typename T::Tail::IsEnd = typename T::Tail::IsEnd())
	{
		std::cout << typeof<typename T::Head>() << ", ";
		std::cout << typeof<typename T::Tail::Head>() << std::endl;
	}

	template<typename T>
	void print_list(typename T::Head = typename T::Head(), typename T::Tail::Tail = typename T::Tail::Tail())
	{
		std::cout << typeof<typename T::Head>() << ", ";
		print_list<typename T::Tail>();
	}

	template<typename TList, typename T>
	struct MostDerived;

	template<typename T>
	struct MostDerived<NullType, T>
	{
		using Result = T;
	};

	template<typename Head, typename Tail, typename T>
	struct MostDerived<TypeList<Head, Tail>, T>
	{
	private:
		using Candidate = typename MostDerived<Tail, T>::Result;
	public:
		using Result = typename std::conditional<std::is_base_of<Candidate, Head>::value, Head, Candidate>::type;
	};

	template<typename T>
	struct DerivedToFront;

	template<>
	struct DerivedToFront<NullType>
	{
		using Result = NullType;
	};

	template<typename Head, typename Tail>
	struct DerivedToFront<TypeList<Head, Tail>>
	{
	private:
		using TheMostDerived = typename MostDerived<Tail, Head>::Result;
		using List = typename Replace<Tail, TheMostDerived, Head>::Result;
	public:
		using Result = TypeList<TheMostDerived, List>;
	};

	#define TypeList(...) ToList<__VA_ARGS__>::Result

	template<typename TList, template<typename> typename Unit>
	class GenScatterHierarchy;

	template<typename T1, typename T2, template<typename> typename Unit>
	class GenScatterHierarchy<TypeList<T1, T2>, Unit>
		: public GenScatterHierarchy<T1, Unit>,
		  public GenScatterHierarchy<T2, Unit>
	{
	public:
		using TList = TypeList<T1, T2>;
		using LeftBase  = GenScatterHierarchy<T1, Unit>;
		using RightBase = GenScatterHierarchy<T2, Unit>;
	};

	template<typename Type, template<typename> typename Unit>
	class GenScatterHierarchy : public Unit<Type>
	{
		using LeftBase = Unit<Type>;
	};

	template<template <typename> typename Unit>
	class GenScatterHierarchy<NullType, Unit> { };		
}