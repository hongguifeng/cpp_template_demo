// 17_compile_time_computation.cpp
// 演示：编译期计算（Template Metaprogramming 的核心）

#include <iostream>

// ============================================================
// 1. 编译期阶乘（经典入门例子）
// ============================================================

// 模板元编程的"函数"：通过递归模板实例化来计算
// 每个 Factorial<N> 是一个不同的类型，各自有一个 value

// 主模板：递归情况
template <int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

// 完全特化：递归终止条件
template <>
struct Factorial<0> {
    static const int value = 1;
};

// ============================================================
// 2. 编译期斐波那契数列
// ============================================================

template <int N>
struct Fibonacci {
    static const long long value =
        Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

template <>
struct Fibonacci<0> {
    static const long long value = 0;
};

template <>
struct Fibonacci<1> {
    static const long long value = 1;
};

// ============================================================
// 3. 编译期最大公约数（GCD）
// ============================================================

template <int A, int B>
struct GCD {
    static const int value = GCD<B, A % B>::value;
};

template <int A>
struct GCD<A, 0> {
    static const int value = A;
};

// ============================================================
// 4. 编译期判断素数
// ============================================================

// 辅助模板：检查 N 是否能被 D 到 sqrt(N) 之间的数整除
template <int N, int D>
struct IsPrimeHelper {
    static const bool value =
        (N % D != 0) && IsPrimeHelper<N, D - 1>::value;
};

// 递归终止：除数为1，没有找到因子
template <int N>
struct IsPrimeHelper<N, 1> {
    static const bool value = true;
};

// 计算整数平方根的辅助（粗略近似）
template <int N, int Lo = 1, int Hi = N>
struct Sqrt {
    static const int mid = (Lo + Hi + 1) / 2;
    static const int value =
        (mid <= N / mid)  // 避免溢出，用 mid <= N/mid 代替 mid*mid <= N
            ? Sqrt<N, mid, Hi>::value
            : Sqrt<N, Lo, mid - 1>::value;
};

template <int N, int M>
struct Sqrt<N, M, M> {
    static const int value = M;
};

// 主模板
template <int N>
struct IsPrime {
    // 从 sqrt(N) 开始往下检查
    static const bool value = (N >= 2) && IsPrimeHelper<N, Sqrt<N>::value>::value;
};

template <>
struct IsPrime<0> {
    static const bool value = false;
};

template <>
struct IsPrime<1> {
    static const bool value = false;
};

template <>
struct IsPrime<2> {
    static const bool value = true;
};

template <>
struct IsPrime<3> {
    static const bool value = true;
};

// ============================================================
// 5. 编译期幂运算
// ============================================================

template <long long Base, int Exp>
struct Power {
    static const long long value = Base * Power<Base, Exp - 1>::value;
};

template <long long Base>
struct Power<Base, 0> {
    static const long long value = 1;
};

// ============================================================
// 6. 编译期条件选择（if-else）
// ============================================================

// 类似于 std::conditional，但用于值
template <bool Condition, int TrueVal, int FalseVal>
struct If {
    static const int value = TrueVal;
};

template <int TrueVal, int FalseVal>
struct If<false, TrueVal, FalseVal> {
    static const int value = FalseVal;
};

// 编译期绝对值
template <int N>
struct Abs {
    static const int value = If<(N >= 0), N, -N>::value;
};

// 编译期最大值
template <int A, int B>
struct Max {
    static const int value = If<(A > B), A, B>::value;
};

// 编译期最小值
template <int A, int B>
struct Min {
    static const int value = If<(A < B), A, B>::value;
};

int main() {
    // ============================================================
    // 1. 阶乘
    // ============================================================
    std::cout << "=== 编译期阶乘 ===" << std::endl;
    std::cout << "0! = " << Factorial<0>::value << std::endl;
    std::cout << "1! = " << Factorial<1>::value << std::endl;
    std::cout << "5! = " << Factorial<5>::value << std::endl;
    std::cout << "10! = " << Factorial<10>::value << std::endl;
    std::cout << "12! = " << Factorial<12>::value << std::endl;

    // ============================================================
    // 2. 斐波那契
    // ============================================================
    std::cout << "\n=== 编译期斐波那契 ===" << std::endl;
    std::cout << "F(0)  = " << Fibonacci<0>::value << std::endl;
    std::cout << "F(1)  = " << Fibonacci<1>::value << std::endl;
    std::cout << "F(5)  = " << Fibonacci<5>::value << std::endl;
    std::cout << "F(10) = " << Fibonacci<10>::value << std::endl;
    std::cout << "F(20) = " << Fibonacci<20>::value << std::endl;
    std::cout << "F(40) = " << Fibonacci<40>::value << std::endl;

    // ============================================================
    // 3. GCD
    // ============================================================
    std::cout << "\n=== 编译期最大公约数 ===" << std::endl;
    std::cout << "GCD(12, 8)  = " << GCD<12, 8>::value << std::endl;
    std::cout << "GCD(100, 75) = " << GCD<100, 75>::value << std::endl;
    std::cout << "GCD(17, 13) = " << GCD<17, 13>::value << std::endl;

    // ============================================================
    // 4. 素数判断
    // ============================================================
    std::cout << "\n=== 编译期素数判断 ===" << std::endl;
    std::cout << "IsPrime<2>  = " << IsPrime<2>::value << std::endl;
    std::cout << "IsPrime<3>  = " << IsPrime<3>::value << std::endl;
    std::cout << "IsPrime<4>  = " << IsPrime<4>::value << std::endl;
    std::cout << "IsPrime<7>  = " << IsPrime<7>::value << std::endl;
    std::cout << "IsPrime<10> = " << IsPrime<10>::value << std::endl;
    std::cout << "IsPrime<13> = " << IsPrime<13>::value << std::endl;
    std::cout << "IsPrime<97> = " << IsPrime<97>::value << std::endl;

    // ============================================================
    // 5. 幂运算
    // ============================================================
    std::cout << "\n=== 编译期幂运算 ===" << std::endl;
    std::cout << "2^10 = " << Power<2, 10>::value << std::endl;
    std::cout << "3^5  = " << Power<3, 5>::value << std::endl;
    std::cout << "10^6 = " << Power<10, 6>::value << std::endl;

    // ============================================================
    // 6. 条件选择
    // ============================================================
    std::cout << "\n=== 编译期条件运算 ===" << std::endl;
    std::cout << "Abs<-5>     = " << Abs<-5>::value << std::endl;
    std::cout << "Abs<3>      = " << Abs<3>::value << std::endl;
    std::cout << "Max<3, 7>   = " << Max<3, 7>::value << std::endl;
    std::cout << "Min<3, 7>   = " << Min<3, 7>::value << std::endl;

    // ============================================================
    // 证明这些是编译期常量：可以用于数组大小
    // ============================================================
    std::cout << "\n=== 编译期常量验证 ===" << std::endl;
    int arrFactorial[Factorial<5>::value];  // int arr[120]
    std::cout << "Factorial<5> 数组大小: "
              << sizeof(arrFactorial) / sizeof(int) << std::endl;

    // 可以用 static_assert 在编译期验证（C++11）
    static_assert(Factorial<5>::value == 120, "5! should be 120");
    static_assert(Fibonacci<10>::value == 55, "F(10) should be 55");
    static_assert(GCD<12, 8>::value == 4, "GCD(12,8) should be 4");
    static_assert(IsPrime<7>::value == true, "7 should be prime");
    static_assert(IsPrime<4>::value == false, "4 should not be prime");
    std::cout << "所有 static_assert 通过！" << std::endl;

    return 0;
}
