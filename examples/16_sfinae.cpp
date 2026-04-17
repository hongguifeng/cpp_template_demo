// 16_sfinae.cpp
// 演示：SFINAE（Substitution Failure Is Not An Error）原则
// SFINAE 是 C++ 模板编程中最重要的原则之一

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================
// 1. SFINAE 基本概念
// ============================================================

// SFINAE 的含义：
// 当编译器尝试用模板参数替换模板定义时，如果替换导致了无效的代码，
// 编译器不会报错，而是忽略这个模板（从候选集中移除），继续尝试其他候选。

// 例子：声明两个重载，通过 SFINAE 让编译器选择正确的版本

// 版本1：只对整数类型有效
template <typename T>
typename std::enable_if<std::is_integral<T>::value, std::string>::type
classify(T value) {
    return std::to_string(value) + " 是整数类型";
}

// 版本2：只对浮点类型有效
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, std::string>::type
classify(T value) {
    return std::to_string(value) + " 是浮点类型";
}

// 版本3：只对其他类型有效（既不是整数也不是浮点）
template <typename T>
typename std::enable_if<
    !std::is_integral<T>::value && !std::is_floating_point<T>::value,
    std::string
>::type
classify(T) {
    return "其他类型";
}

// ============================================================
// 2. SFINAE 与成员函数检测（更详细的步骤说明）
// ============================================================

// 检测类型 T 是否有 begin() 和 end() 方法（即是否是可迭代容器）
template <typename T>
class is_iterable {
private:
    // 第一步：尝试调用 T 的 begin() 和 end()
    // 如果 T 没有这些方法，decltype 会导致替换失败 -> SFINAE 忽略此重载
    template <typename U>
    static auto test(int)
        -> decltype(
            std::declval<U>().begin(),  // 要求有 begin()
            std::declval<U>().end(),    // 要求有 end()
            std::true_type{}            // 如果以上都成功，返回 true_type
        );

    // 第二步：后备重载，参数用 ...（优先级最低），总是成功
    template <typename>
    static std::false_type test(...);

public:
    // test<T>(0) 会优先尝试第一个重载（参数是 int，0 完美匹配）
    // 如果失败（SFINAE），会退到第二个重载
    static const bool value = decltype(test<T>(0))::value;
};

// ============================================================
// 3. enable_if 在不同位置的用法
// ============================================================

// 用法 A：在返回类型中使用（前面已经展示过）
template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
isPositive_A(T value) {
    return value > 0;
}

// 用法 B：在模板参数中使用（更推荐，不污染返回类型）
template <typename T,
          typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
bool isPositive_B(T value) {
    return value > 0;
}

// 用法 C：在函数参数中使用
template <typename T>
bool isPositive_C(T value,
                  typename std::enable_if<std::is_integral<T>::value, int>::type = 0) {
    return value > 0;
}

// ============================================================
// 4. SFINAE 实现函数重载分派
// ============================================================

// 对可迭代类型（排除 string）：打印所有元素
template <typename T>
typename std::enable_if<
    is_iterable<T>::value && !std::is_same<T, std::string>::value
>::type
smartPrint(const T& container) {
    std::cout << "[";
    bool first = true;
    for (const auto& elem : container) {
        if (!first) std::cout << ", ";
        std::cout << elem;
        first = false;
    }
    std::cout << "]";
}

// 对算术类型：直接打印值
template <typename T>
typename std::enable_if<std::is_arithmetic<T>::value>::type
smartPrint(const T& value) {
    std::cout << value;
}

// 对字符串类型：加引号打印
// 注意：std::string 同时也是可迭代的，所以需要排除
template <typename T>
typename std::enable_if<
    std::is_same<T, std::string>::value
>::type
smartPrint(const T& str) {
    std::cout << "\"" << str << "\"";
}

// ============================================================
// 5. void_t 技巧（C++11 中手动实现，C++17 标准化）
// ============================================================

// C++11 中自己实现 void_t
template <typename...>
struct make_void { typedef void type; };

template <typename... Ts>
using my_void_t = typename make_void<Ts...>::type;

// 使用 void_t 检测是否有 push_back 方法
// 主模板：默认没有
template <typename T, typename = void>
struct has_push_back : std::false_type {};

// 偏特化：如果 T 有 push_back，void_t<...> 有效
template <typename T>
struct has_push_back<T, my_void_t<
    decltype(std::declval<T>().push_back(std::declval<typename T::value_type>()))
>> : std::true_type {};

int main() {
    // ============================================================
    // 1. SFINAE 基本例子
    // ============================================================
    std::cout << "=== classify (SFINAE) ===" << std::endl;
    std::cout << classify(42) << std::endl;
    std::cout << classify(3.14) << std::endl;
    std::cout << classify(std::string("hello")) << std::endl;
    std::cout << classify(true) << std::endl;  // bool 也是整数类型

    // ============================================================
    // 2. is_iterable 检测
    // ============================================================
    std::cout << "\n=== is_iterable ===" << std::endl;
    std::cout << "vector<int>:  " << is_iterable<std::vector<int>>::value << std::endl;
    std::cout << "string:       " << is_iterable<std::string>::value << std::endl;
    std::cout << "int:          " << is_iterable<int>::value << std::endl;
    std::cout << "double:       " << is_iterable<double>::value << std::endl;

    // ============================================================
    // 3. enable_if 三种用法
    // ============================================================
    std::cout << "\n=== enable_if 三种位置 ===" << std::endl;
    std::cout << "isPositive_A(42):  " << isPositive_A(42) << std::endl;
    std::cout << "isPositive_B(-5):  " << isPositive_B(-5) << std::endl;
    std::cout << "isPositive_C(0):   " << isPositive_C(0) << std::endl;

    // 以下会编译失败（SFINAE 排除了非整数类型）：
    // isPositive_A(3.14);  // 错误！double 不是整数类型

    // ============================================================
    // 4. smartPrint
    // ============================================================
    std::cout << "\n=== smartPrint ===" << std::endl;

    std::cout << "int:     ";
    smartPrint(42);
    std::cout << std::endl;

    std::cout << "double:  ";
    smartPrint(3.14);
    std::cout << std::endl;

    std::cout << "string:  ";
    smartPrint(std::string("hello"));
    std::cout << std::endl;

    std::cout << "vector:  ";
    smartPrint(std::vector<int>{1, 2, 3, 4, 5});
    std::cout << std::endl;

    // ============================================================
    // 5. has_push_back（void_t 技巧）
    // ============================================================
    std::cout << "\n=== has_push_back (void_t) ===" << std::endl;
    std::cout << "vector<int>:  " << has_push_back<std::vector<int>>::value << std::endl;
    std::cout << "string:       " << has_push_back<std::string>::value << std::endl;

    return 0;
}
