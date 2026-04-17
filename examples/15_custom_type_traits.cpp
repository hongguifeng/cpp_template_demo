// 15_custom_type_traits.cpp
// 演示：自定义类型特征（Custom Type Traits）

#include <iostream>
#include <string>
#include <vector>
#include <type_traits>

// ============================================================
// 1. 自定义 is_pointer（理解标准库的实现原理）
// ============================================================

// 主模板：默认不是指针
template <typename T>
struct my_is_pointer {
    static const bool value = false;
};

// 偏特化：如果 T 是 U* 形式，就是指针
template <typename U>
struct my_is_pointer<U*> {
    static const bool value = true;
};

// 偏特化：const 指针也要处理
template <typename U>
struct my_is_pointer<U* const> {
    static const bool value = true;
};

// ============================================================
// 2. 自定义 is_same
// ============================================================

template <typename T, typename U>
struct my_is_same {
    static const bool value = false;
};

template <typename T>
struct my_is_same<T, T> {
    static const bool value = true;
};

// ============================================================
// 3. 自定义 remove_const / add_const
// ============================================================

// remove_const
template <typename T>
struct my_remove_const {
    typedef T type;
};

template <typename T>
struct my_remove_const<const T> {
    typedef T type;
};

// add_const
template <typename T>
struct my_add_const {
    typedef const T type;
};

// ============================================================
// 4. 自定义 remove_reference
// ============================================================

template <typename T>
struct my_remove_reference {
    typedef T type;
};

template <typename T>
struct my_remove_reference<T&> {
    typedef T type;
};

template <typename T>
struct my_remove_reference<T&&> {
    typedef T type;
};

// ============================================================
// 5. 自定义 enable_if
// ============================================================

// 主模板：condition 为 false 时，没有 type 成员
template <bool Condition, typename T = void>
struct my_enable_if {};

// 偏特化：condition 为 true 时，有 type 成员
template <typename T>
struct my_enable_if<true, T> {
    typedef T type;
};

// ============================================================
// 6. 更复杂的类型特征：检测是否有某个成员函数
// ============================================================

// 检测类型 T 是否有 size() 成员函数
// 使用 SFINAE 技巧

// 辅助结构
template <typename T>
class has_size_method {
private:
    // 如果 U 有 size() 方法，这个重载会成功（SFINAE）
    template <typename U>
    static auto test(int) -> decltype(std::declval<U>().size(), std::true_type{});

    // 后备重载：总是成功
    template <typename>
    static std::false_type test(...);

public:
    static const bool value = decltype(test<T>(0))::value;
};

// ============================================================
// 7. 检测是否可以用 << 输出到 ostream
// ============================================================

template <typename T>
class is_printable {
private:
    template <typename U>
    static auto test(int)
        -> decltype(std::declval<std::ostream&>() << std::declval<U>(), std::true_type{});

    template <typename>
    static std::false_type test(...);

public:
    static const bool value = decltype(test<T>(0))::value;
};

// ============================================================
// 8. 实际应用：根据类型特征选择不同的行为
// ============================================================

// 辅助函数：有 size() 的类型
template <typename T>
typename my_enable_if<has_size_method<T>::value>::type
printInfo(const T& obj) {
    std::cout << "  有 size() 方法, size = " << obj.size() << std::endl;
}

// 辅助函数：没有 size() 的类型
template <typename T>
typename my_enable_if<!has_size_method<T>::value>::type
printInfo(const T& obj) {
    (void)obj;
    std::cout << "  没有 size() 方法" << std::endl;
}

// 测试用的类
class WithSize {
public:
    int size() const { return 42; }
};

class WithoutSize {
public:
    int value() const { return 10; }
};

int main() {
    // ============================================================
    // 1. my_is_pointer
    // ============================================================
    std::cout << "=== my_is_pointer ===" << std::endl;
    std::cout << "int:        " << my_is_pointer<int>::value << std::endl;
    std::cout << "int*:       " << my_is_pointer<int*>::value << std::endl;
    std::cout << "int**:      " << my_is_pointer<int**>::value << std::endl;
    std::cout << "int* const: " << my_is_pointer<int* const>::value << std::endl;

    // ============================================================
    // 2. my_is_same
    // ============================================================
    std::cout << "\n=== my_is_same ===" << std::endl;
    std::cout << "int, int:       " << my_is_same<int, int>::value << std::endl;
    std::cout << "int, double:    " << my_is_same<int, double>::value << std::endl;
    std::cout << "int, const int: " << my_is_same<int, const int>::value << std::endl;

    // ============================================================
    // 3. my_remove_const
    // ============================================================
    std::cout << "\n=== my_remove_const ===" << std::endl;
    std::cout << "remove_const<const int> == int: "
              << my_is_same<my_remove_const<const int>::type, int>::value << std::endl;
    std::cout << "remove_const<int> == int: "
              << my_is_same<my_remove_const<int>::type, int>::value << std::endl;

    // ============================================================
    // 4. my_remove_reference
    // ============================================================
    std::cout << "\n=== my_remove_reference ===" << std::endl;
    std::cout << "remove_ref<int&> == int: "
              << my_is_same<my_remove_reference<int&>::type, int>::value << std::endl;
    std::cout << "remove_ref<int&&> == int: "
              << my_is_same<my_remove_reference<int&&>::type, int>::value << std::endl;
    std::cout << "remove_ref<int> == int: "
              << my_is_same<my_remove_reference<int>::type, int>::value << std::endl;

    // ============================================================
    // 5. my_enable_if（已在 printInfo 中使用）
    // ============================================================

    // ============================================================
    // 6. has_size_method
    // ============================================================
    std::cout << "\n=== has_size_method ===" << std::endl;
    std::cout << "std::string: " << has_size_method<std::string>::value << std::endl;
    std::cout << "std::vector<int>: " << has_size_method<std::vector<int>>::value << std::endl;
    std::cout << "int: " << has_size_method<int>::value << std::endl;
    std::cout << "WithSize: " << has_size_method<WithSize>::value << std::endl;
    std::cout << "WithoutSize: " << has_size_method<WithoutSize>::value << std::endl;

    // ============================================================
    // 7. is_printable
    // ============================================================
    std::cout << "\n=== is_printable ===" << std::endl;
    std::cout << "int: " << is_printable<int>::value << std::endl;
    std::cout << "std::string: " << is_printable<std::string>::value << std::endl;
    std::cout << "std::vector<int>: " << is_printable<std::vector<int>>::value << std::endl;

    // ============================================================
    // 8. 根据类型特征选择行为
    // ============================================================
    std::cout << "\n=== printInfo (根据是否有 size) ===" << std::endl;

    std::string str = "hello";
    std::cout << "std::string:";
    printInfo(str);

    std::vector<int> vec = {1, 2, 3};
    std::cout << "vector<int>:";
    printInfo(vec);

    int x = 42;
    std::cout << "int:";
    printInfo(x);

    WithSize ws;
    std::cout << "WithSize:";
    printInfo(ws);

    WithoutSize wos;
    std::cout << "WithoutSize:";
    printInfo(wos);

    return 0;
}
