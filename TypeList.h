#ifndef TypeLists_h__
#define TypeLists_h__

template <class T, class U>
struct TypeList
{
	typedef T head;
	typedef U Tail;
};

class NullType{};
struct EmptyType{};

#define TYPELIST_1(T1) TypeList<T1, NullType>
#define TYPELIST_2(T1, T2) TypeList<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3) TypeList<T1, TYPELIST_2(T2, T3)>
#define TYPELIST_4(T1, T2, T3, T4) TypeList<T1, TYPELIST_3(T2, T3, T4)>
#define TYPELIST_5(T1, T2, T3, T4, T5) TypeList<T1, TYPELIST_4(T2, T3, T4, T5)>

// Сравнение списков типов
template <class T1, class T2> struct CompareType;
template <class T1, class T2>
struct CompareType
{
	enum { value = 0 };
};
template<class T>
struct CompareType<T, T>
{
	enum { value = 1 };
};


// Длина списка типов
template <class TList> struct Lenght;
template <> struct Lenght<NullType>
{
	enum { value = 0 };
};
template <class T, class U>
struct Lenght<TypeList<T, U>>
{
	enum { value = 1 + Lenght<U>::value };
};


// Индексированный доступ
template <class TList, unsigned int index> struct TypeAt;
template <class Head, class Tail>
struct TypeAt<TypeList<Head, Tail>, 0>
{
	typedef Head Result;
};
template <class Head, class Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail>, i>
{
	typedef typename TypeAt<Tail, i - 1>::Result Result;
};


// Поиск элемента
template <class TList, class T> struct Find;
template <class T>
struct Find<NullType, T>
{
	enum { value = -1 };
};
template <class T, class Tail>
struct Find<TypeList<T, Tail>, T>
{
	enum { value = 0 };
};
template <class Head, class Tail, class T>
struct Find<TypeList<Head, Tail>, T>
{
private:
	enum { temp = Find<Tail, T>::value };
public:
	enum {value = temp == -1 ? -1 : 1 + temp };
};



// Добавление элемента
template <class TList, class T> struct Append;

template <> struct Append<NullType, NullType>
{
	typedef NullType Result;
};

template <class T> struct Append<NullType, T>
{
	typedef TYPELIST_1(T) Result;
};

template <class Head, class Tail>
struct Append<NullType, TypeList<Head, Tail>>
{
	typedef TypeList<Head, Tail> Result;
};

template <class Head, class Tail, class T>
struct Append<TypeList<Head, Tail>, T>
{
	typedef TypeList<Head, typename Append<Tail, T>::Result> Result;
};


// Удаление элемента
template <class TLict, class T> struct Erase;

template <class T>
struct Erase<NullType, T>
{
	typedef NullType Result;
};

template <class T, class Tail>
struct Erase<TypeList<T, Tail>, T>
{
	typedef Tail Result;
};

template <class Head, class Tail, class T>
struct Erase<TypeList<Head, Tail>, T>
{
	typedef TypeList<Head, typename Erase<Tail, T>::Result> Result;
};


// Удаление дубликатов
template <class TList> struct NoDublicates;

template <> struct NoDublicates<NullType>
{
	typedef NullType Result;
};

template <class Head, class Tail>
struct NoDublicates<TypeList<Head, Tail>>
{
private:
	typedef typename NoDublicates<Tail>::Result L1;
	typedef typename Erase<L1, Head>::Result L2;
public:
	typedef TypeList<Head, L2> Result;
};


// Проверка на наличие дубликатов в списке
template <class TList> struct CheckDublicates;

template <class TList> 
struct CheckDublicates
{
	enum 
	{
		value = CompareType<TList, NoDublicates<TList>::Result>::value ? 0 : 1
	};
};


void TypeListTest();

#endif // TypeLists_h__