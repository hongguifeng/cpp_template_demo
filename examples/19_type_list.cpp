// 19_type_list.cpp
// 演示：类型列表（TypeList）—— 模板元编程中的"数据结构"

#include <iostream>
#include <string>
#include <type_traits>

// ============================================================
// 1. TypeList 的定义
// ============================================================

// TypeList 是模板元编程中的"链表"，存储一系列类型
// 使用可变参数模板实现（C++11 风格）
template <typename... Types>
struct TypeList {};

// 空的 TypeList
using EmptyList = TypeList<>;

// ============================================================
// 2. 基本操作：获取长度
// ============================================================

// Length: 计算 TypeList 中类型的个数
template <typename List>
struct Length;

template <typename... Types>
struct Length<TypeList<Types...>> {
    static const int value = sizeof...(Types);
};

// ============================================================
// 3. 获取第 N 个类型（TypeAt）
// ============================================================

// 主模板（递归）
template <typename List, int N>
struct TypeAt;

// 取第一个（N=0）
template <typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0> {
    typedef Head type;
};

// 递归：跳过第一个，在剩余中取第 N-1 个
template <typename Head, typename... Tail, int N>
struct TypeAt<TypeList<Head, Tail...>, N> {
    typedef typename TypeAt<TypeList<Tail...>, N - 1>::type type;
};

// ============================================================
// 4. 在头部添加类型（PushFront）
// ============================================================

template <typename List, typename NewType>
struct PushFront;

template <typename... Types, typename NewType>
struct PushFront<TypeList<Types...>, NewType> {
    typedef TypeList<NewType, Types...> type;
};

// ============================================================
// 5. 在尾部添加类型（PushBack）
// ============================================================

template <typename List, typename NewType>
struct PushBack;

template <typename... Types, typename NewType>
struct PushBack<TypeList<Types...>, NewType> {
    typedef TypeList<Types..., NewType> type;
};

// ============================================================
// 6. 移除第一个类型（PopFront）
// ============================================================

template <typename List>
struct PopFront;

template <typename Head, typename... Tail>
struct PopFront<TypeList<Head, Tail...>> {
    typedef TypeList<Tail...> type;
};

// ============================================================
// 7. 判断是否包含某个类型（Contains）
// ============================================================

template <typename List, typename T>
struct Contains;

// 空列表不包含任何类型
template <typename T>
struct Contains<TypeList<>, T> {
    static const bool value = false;
};

// 头部匹配
template <typename... Tail, typename T>
struct Contains<TypeList<T, Tail...>, T> {
    static const bool value = true;
};

// 头部不匹配，递归检查尾部
template <typename Head, typename... Tail, typename T>
struct Contains<TypeList<Head, Tail...>, T> {
    static const bool value = Contains<TypeList<Tail...>, T>::value;
};

// ============================================================
// 8. 获取类型的索引（IndexOf）
// ============================================================

template <typename List, typename T>
struct IndexOf;

// 没找到
template <typename T>
struct IndexOf<TypeList<>, T> {
    static const int value = -1;
};

// 头部匹配
template <typename... Tail, typename T>
struct IndexOf<TypeList<T, Tail...>, T> {
    static const int value = 0;
};

// 递归查找
template <typename Head, typename... Tail, typename T>
struct IndexOf<TypeList<Head, Tail...>, T> {
private:
    static const int temp = IndexOf<TypeList<Tail...>, T>::value;
public:
    static const int value = (temp == -1) ? -1 : 1 + temp;
};

// ============================================================
// 9. 连接两个 TypeList（Concat）
// ============================================================

template <typename List1, typename List2>
struct Concat;

template <typename... Types1, typename... Types2>
struct Concat<TypeList<Types1...>, TypeList<Types2...>> {
    typedef TypeList<Types1..., Types2...> type;
};

// ============================================================
// 10. 反转 TypeList（Reverse）
// ============================================================

template <typename List>
struct Reverse;

template <>
struct Reverse<TypeList<>> {
    typedef TypeList<> type;
};

template <typename Head, typename... Tail>
struct Reverse<TypeList<Head, Tail...>> {
    // 反转尾部，然后把头部加到末尾
    typedef typename PushBack<
        typename Reverse<TypeList<Tail...>>::type,
        Head
    >::type type;
};

// ============================================================
// 辅助：打印类型名（用于展示）
// ============================================================

template <typename T>
struct TypeName { static const char* name() { return "unknown"; } };

template <>
struct TypeName<int> { static const char* name() { return "int"; } };
template <>
struct TypeName<double> { static const char* name() { return "double"; } };
template <>
struct TypeName<char> { static const char* name() { return "char"; } };
template <>
struct TypeName<float> { static const char* name() { return "float"; } };
template <>
struct TypeName<bool> { static const char* name() { return "bool"; } };
template <>
struct TypeName<std::string> { static const char* name() { return "string"; } };
template <>
struct TypeName<long> { static const char* name() { return "long"; } };

// 打印 TypeList
void printTypeList(TypeList<>) {
    std::cout << "(空)";
}

template <typename Head>
void printTypeList(TypeList<Head>) {
    std::cout << TypeName<Head>::name();
}

template <typename Head, typename Second, typename... Tail>
void printTypeList(TypeList<Head, Second, Tail...>) {
    std::cout << TypeName<Head>::name() << ", ";
    printTypeList(TypeList<Second, Tail...>{});
}

// 包装打印
template <typename List>
void showList(const std::string& label, List list) {
    std::cout << label << ": [";
    printTypeList(list);
    std::cout << "]" << std::endl;
}

int main() {
    // ============================================================
    // 定义一些 TypeList
    // ============================================================
    typedef TypeList<int, double, char> MyList;
    typedef TypeList<float, bool> List2;

    // ============================================================
    // Length
    // ============================================================
    std::cout << "=== TypeList 操作 ===" << std::endl;
    showList("MyList", MyList{});
    std::cout << "Length: " << Length<MyList>::value << std::endl;

    // ============================================================
    // TypeAt
    // ============================================================
    std::cout << "\n=== TypeAt ===" << std::endl;
    std::cout << "TypeAt<0>: " << TypeName<TypeAt<MyList, 0>::type>::name() << std::endl;
    std::cout << "TypeAt<1>: " << TypeName<TypeAt<MyList, 1>::type>::name() << std::endl;
    std::cout << "TypeAt<2>: " << TypeName<TypeAt<MyList, 2>::type>::name() << std::endl;

    // ============================================================
    // PushFront / PushBack
    // ============================================================
    std::cout << "\n=== Push 操作 ===" << std::endl;
    typedef PushFront<MyList, bool>::type WithBoolFront;
    showList("PushFront<bool>", WithBoolFront{});

    typedef PushBack<MyList, float>::type WithFloatBack;
    showList("PushBack<float>", WithFloatBack{});

    // ============================================================
    // PopFront
    // ============================================================
    std::cout << "\n=== PopFront ===" << std::endl;
    typedef PopFront<MyList>::type AfterPop;
    showList("PopFront", AfterPop{});

    // ============================================================
    // Contains
    // ============================================================
    std::cout << "\n=== Contains ===" << std::endl;
    std::cout << "Contains<int>:    " << Contains<MyList, int>::value << std::endl;
    std::cout << "Contains<double>: " << Contains<MyList, double>::value << std::endl;
    std::cout << "Contains<float>:  " << Contains<MyList, float>::value << std::endl;

    // ============================================================
    // IndexOf
    // ============================================================
    std::cout << "\n=== IndexOf ===" << std::endl;
    std::cout << "IndexOf<int>:    " << IndexOf<MyList, int>::value << std::endl;
    std::cout << "IndexOf<double>: " << IndexOf<MyList, double>::value << std::endl;
    std::cout << "IndexOf<char>:   " << IndexOf<MyList, char>::value << std::endl;
    std::cout << "IndexOf<float>:  " << IndexOf<MyList, float>::value << " (不存在)" << std::endl;

    // ============================================================
    // Concat
    // ============================================================
    std::cout << "\n=== Concat ===" << std::endl;
    typedef Concat<MyList, List2>::type Combined;
    showList("MyList + List2", Combined{});
    std::cout << "Combined Length: " << Length<Combined>::value << std::endl;

    // ============================================================
    // Reverse
    // ============================================================
    std::cout << "\n=== Reverse ===" << std::endl;
    typedef Reverse<MyList>::type Reversed;
    showList("原始", MyList{});
    showList("反转", Reversed{});

    // ============================================================
    // 编译期验证
    // ============================================================
    static_assert(Length<MyList>::value == 3, "MyList should have 3 types");
    static_assert(Contains<MyList, int>::value, "MyList should contain int");
    static_assert(!Contains<MyList, float>::value, "MyList should not contain float");
    static_assert(IndexOf<MyList, double>::value == 1, "double should be at index 1");
    std::cout << "\n所有 static_assert 通过！" << std::endl;

    return 0;
}
