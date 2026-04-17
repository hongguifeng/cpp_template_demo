// 13_variadic_templates_advanced.cpp
// 演示：可变参数模板的高级用法

#include <iostream>
#include <string>
#include <tuple>
#include <memory>
#include <vector>

// ============================================================
// 1. 参数包展开（Pack Expansion）的多种模式
// ============================================================

// 对参数包中每个元素应用操作，然后展开
template <typename T>
T square(T x) {
    return x * x;
}

// 打印每个参数平方后的结果
template <typename... Args>
void printSquares(const Args&... args) {
    // 逗号表达式技巧（C++11 中没有 fold expression）
    // 利用初始化列表来展开参数包
    // {(expression, 0)...} 对每个参数执行 expression，返回 0 存入数组
    int dummy[] = { (std::cout << square(args) << " ", 0)... };
    (void)dummy;  // 避免未使用变量警告
    std::cout << std::endl;
}

// ============================================================
// 2. 完美转发与可变参数模板：自己实现 make_unique
// ============================================================
// （std::make_unique 是 C++14 引入的，我们用 C++11 来实现）

// std::forward 实现完美转发，保持参数的左值/右值属性
// Args&&... 是万能引用（universal reference）
template <typename T, typename... Args>
std::unique_ptr<T> myMakeUnique(Args&&... args) {
    // std::forward<Args>(args)... 展开为对每个参数的完美转发
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

// 测试用的类
class Person {
    std::string name;
    int age;
public:
    Person(const std::string& n, int a) : name(n), age(a) {
        std::cout << "  Person 构造: " << name << ", " << age << std::endl;
    }
    void print() const {
        std::cout << "  Person: " << name << ", 年龄 " << age << std::endl;
    }
};

// ============================================================
// 3. 可变参数类模板：Tuple 的简化实现
// ============================================================

// 前向声明
template <typename... Types>
class MyTuple;

// 空 tuple（递归终止）
template <>
class MyTuple<> {
public:
    void print() const { std::cout << "()"; }
    void printImpl() const {}  // 空操作，供递归终止时调用

    template <typename... Ts>
    friend class MyTuple;
};

// 递归定义：head + tail
// 第一个类型 Head，剩余类型打包成 Tail...
template <typename Head, typename... Tail>
class MyTuple<Head, Tail...> : private MyTuple<Tail...> {
    Head value;

    typedef MyTuple<Tail...> Base;

public:
    MyTuple(const Head& h, const Tail&... t)
        : Base(t...), value(h) {}

    Head head() const { return value; }
    const Base& tail() const { return *this; }

    void print() const {
        std::cout << "(";
        printImpl();
        std::cout << ")";
    }

    void printImpl() const {
        std::cout << value;
        if (sizeof...(Tail) > 0) {
            std::cout << ", ";
            Base::printImpl();
        }
    }

    // 让基类能访问 printImpl
    template <typename... Ts>
    friend class MyTuple;
};

// ============================================================
// 4. 使用可变参数模板实现类型安全的容器
// ============================================================

// emplace 风格：在容器中原地构造元素
template <typename T>
class TypeSafeVector {
    std::vector<T> data;

public:
    // 使用可变参数模板直接在容器内构造对象
    template <typename... Args>
    void emplace_back(Args&&... args) {
        data.emplace_back(std::forward<Args>(args)...);
    }

    void print() const {
        std::cout << "[";
        for (size_t i = 0; i < data.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
    }
};

int main() {
    // ============================================================
    // 1. 参数包展开模式
    // ============================================================
    std::cout << "=== 参数包展开 ===" << std::endl;
    std::cout << "平方值: ";
    printSquares(1, 2, 3, 4, 5);
    std::cout << "平方值: ";
    printSquares(1.5, 2.5);

    // ============================================================
    // 2. 完美转发 + make_unique
    // ============================================================
    std::cout << "\n=== myMakeUnique ===" << std::endl;

    auto p1 = myMakeUnique<Person>("Alice", 25);
    p1->print();

    auto p2 = myMakeUnique<Person>(std::string("Bob"), 30);
    p2->print();

    // 也可以用于基本类型
    auto pi = myMakeUnique<int>(42);
    std::cout << "  unique_ptr<int>: " << *pi << std::endl;

    // ============================================================
    // 3. MyTuple
    // ============================================================
    std::cout << "\n=== MyTuple ===" << std::endl;

    MyTuple<int, double, std::string> t(42, 3.14, "hello");
    std::cout << "tuple: ";
    t.print();
    std::cout << std::endl;

    std::cout << "head: " << t.head() << std::endl;

    MyTuple<int> single(100);
    std::cout << "single: ";
    single.print();
    std::cout << std::endl;

    // ============================================================
    // 4. emplace_back
    // ============================================================
    std::cout << "\n=== TypeSafeVector emplace ===" << std::endl;

    TypeSafeVector<std::string> sv;
    sv.emplace_back("hello");
    sv.emplace_back(5, 'A');    // string(5, 'A') = "AAAAA"
    sv.emplace_back("world");
    sv.print();

    // ============================================================
    // 5. std::tuple（标准库版本）
    // ============================================================
    std::cout << "\n=== std::tuple ===" << std::endl;

    auto stdTuple = std::make_tuple(1, 3.14, std::string("hello"));
    std::cout << "std::get<0>: " << std::get<0>(stdTuple) << std::endl;
    std::cout << "std::get<1>: " << std::get<1>(stdTuple) << std::endl;
    std::cout << "std::get<2>: " << std::get<2>(stdTuple) << std::endl;

    return 0;
}
