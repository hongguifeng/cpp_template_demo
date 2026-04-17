// 14_type_traits.cpp
// 演示：类型特征（Type Traits）—— C++11 <type_traits> 头文件

#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ============================================================
// 1. 基本类型查询（Primary Type Categories）
// ============================================================

template <typename T>
void analyzeType(const std::string& name) {
    std::cout << name << ":" << std::endl;
    std::cout << "  is_integral:         " << std::is_integral<T>::value << std::endl;
    std::cout << "  is_floating_point:   " << std::is_floating_point<T>::value << std::endl;
    std::cout << "  is_arithmetic:       " << std::is_arithmetic<T>::value << std::endl;
    std::cout << "  is_pointer:          " << std::is_pointer<T>::value << std::endl;
    std::cout << "  is_reference:        " << std::is_reference<T>::value << std::endl;
    std::cout << "  is_const:            " << std::is_const<T>::value << std::endl;
    std::cout << "  is_class:            " << std::is_class<T>::value << std::endl;
    std::cout << "  is_array:            " << std::is_array<T>::value << std::endl;
    std::cout << "  is_void:             " << std::is_void<T>::value << std::endl;
}

// ============================================================
// 2. 类型修改特征（Type Modifications）
// ============================================================

template <typename T>
void showTypeModifications(const std::string& name) {
    std::cout << name << ":" << std::endl;

    // remove_const: 去掉 const 修饰
    std::cout << "  remove_const -> is_const: "
              << std::is_const<typename std::remove_const<T>::type>::value << std::endl;

    // remove_reference: 去掉引用
    std::cout << "  remove_reference -> is_reference: "
              << std::is_reference<typename std::remove_reference<T>::type>::value << std::endl;

    // remove_pointer: 去掉指针
    std::cout << "  remove_pointer -> is_pointer: "
              << std::is_pointer<typename std::remove_pointer<T>::type>::value << std::endl;
}

// ============================================================
// 3. 类型关系特征（Type Relationships）
// ============================================================

class Base {
public:
    virtual ~Base() {}
};
class Derived : public Base {};
class Unrelated {};

void showRelationships() {
    std::cout << "=== 类型关系 ===" << std::endl;

    // is_same: 判断两个类型是否完全相同
    std::cout << "is_same<int, int>:       " << std::is_same<int, int>::value << std::endl;
    std::cout << "is_same<int, long>:      " << std::is_same<int, long>::value << std::endl;
    std::cout << "is_same<int, const int>: " << std::is_same<int, const int>::value << std::endl;

    // is_base_of: 判断继承关系
    std::cout << "is_base_of<Base, Derived>:   "
              << std::is_base_of<Base, Derived>::value << std::endl;
    std::cout << "is_base_of<Derived, Base>:   "
              << std::is_base_of<Derived, Base>::value << std::endl;
    std::cout << "is_base_of<Base, Unrelated>: "
              << std::is_base_of<Base, Unrelated>::value << std::endl;

    // is_convertible: 判断是否可以隐式转换
    std::cout << "is_convertible<int, double>:    "
              << std::is_convertible<int, double>::value << std::endl;
    std::cout << "is_convertible<double, int>:    "
              << std::is_convertible<double, int>::value << std::endl;
    std::cout << "is_convertible<Derived*, Base*>: "
              << std::is_convertible<Derived*, Base*>::value << std::endl;
}

// ============================================================
// 4. 使用 type_traits 进行条件编译
// ============================================================

// 根据类型是否为整数，选择不同的实现
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
safeDiv(T a, T b) {
    if (b == 0) {
        std::cout << "  [整数除法: 除数为0，返回0]" << std::endl;
        return 0;
    }
    return a / b;
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
safeDiv(T a, T b) {
    if (b == 0.0) {
        std::cout << "  [浮点除法: 除数为0，返回0.0]" << std::endl;
        return 0.0;
    }
    return a / b;
}

// ============================================================
// 5. conditional：编译期三元表达式
// ============================================================

// std::conditional<condition, TypeIfTrue, TypeIfFalse>::type
// 如果 condition 为 true，type 就是 TypeIfTrue
// 否则 type 就是 TypeIfFalse

template <typename T>
void showConditional() {
    // 如果 T 是整数类型，选择 long long；否则选择 double
    typedef typename std::conditional<
        std::is_integral<T>::value,
        long long,
        double
    >::type SafeType;

    std::cout << "  原始类型大小: " << sizeof(T)
              << ", SafeType 大小: " << sizeof(SafeType) << std::endl;
}

int main() {
    // ============================================================
    // 1. 类型查询
    // ============================================================
    std::cout << "=== 类型查询 ===" << std::endl;
    analyzeType<int>("int");
    analyzeType<double>("double");
    analyzeType<int*>("int*");
    analyzeType<const int>("const int");
    analyzeType<std::string>("std::string");
    analyzeType<int[10]>("int[10]");

    // ============================================================
    // 2. 类型修改
    // ============================================================
    std::cout << "\n=== 类型修改 ===" << std::endl;
    showTypeModifications<const int>("const int");
    showTypeModifications<int&>("int&");
    showTypeModifications<int*>("int*");

    // ============================================================
    // 3. 类型关系
    // ============================================================
    std::cout << std::endl;
    showRelationships();

    // ============================================================
    // 4. enable_if 条件编译
    // ============================================================
    std::cout << "\n=== safeDiv (enable_if) ===" << std::endl;
    std::cout << "10 / 3 = " << safeDiv(10, 3) << std::endl;
    std::cout << "10 / 0 = " << safeDiv(10, 0) << std::endl;
    std::cout << "10.0 / 3.0 = " << safeDiv(10.0, 3.0) << std::endl;
    std::cout << "10.0 / 0.0 = " << safeDiv(10.0, 0.0) << std::endl;

    // ============================================================
    // 5. conditional
    // ============================================================
    std::cout << "\n=== std::conditional ===" << std::endl;
    std::cout << "int -> SafeType: ";
    showConditional<int>();
    std::cout << "short -> SafeType: ";
    showConditional<short>();
    std::cout << "float -> SafeType: ";
    showConditional<float>();

    return 0;
}
