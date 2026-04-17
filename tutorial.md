# C++ 模板元编程完全教程

> **从零开始，由浅入深，掌握 C++ 模板编程的所有核心知识**
>
> 基于 C++11 标准，涉及更高版本特性时会明确标注。
> 所有示例代码均已通过编译和运行验证。

---

## 目录

- [第一部分：模板基础](#第一部分模板基础)
  - [第1章 函数模板](#第1章-函数模板)
  - [第2章 多类型参数与返回类型推导](#第2章-多类型参数与返回类型推导)
  - [第3章 模板参数推导规则](#第3章-模板参数推导规则)
  - [第4章 函数模板重载](#第4章-函数模板重载)
- [第二部分：类模板](#第二部分类模板)
  - [第5章 类模板基础](#第5章-类模板基础)
  - [第6章 类模板成员详解](#第6章-类模板成员详解)
  - [第7章 默认模板参数与成员模板](#第7章-默认模板参数与成员模板)
- [第三部分：非类型参数与特化](#第三部分非类型参数与特化)
  - [第8章 非类型模板参数](#第8章-非类型模板参数)
  - [第9章 完全特化](#第9章-完全特化)
  - [第10章 偏特化](#第10章-偏特化)
  - [第11章 模板模板参数](#第11章-模板模板参数)
- [第四部分：可变参数模板](#第四部分可变参数模板)
  - [第12章 可变参数模板基础](#第12章-可变参数模板基础)
  - [第13章 可变参数模板高级用法](#第13章-可变参数模板高级用法)
- [第五部分：类型特征与 SFINAE](#第五部分类型特征与-sfinae)
  - [第14章 标准库类型特征](#第14章-标准库类型特征)
  - [第15章 自定义类型特征](#第15章-自定义类型特征)
  - [第16章 SFINAE 原则](#第16章-sfinae-原则)
- [第六部分：模板元编程](#第六部分模板元编程)
  - [第17章 编译期计算](#第17章-编译期计算)
  - [第18章 constexpr 与 TMP 对比](#第18章-constexpr-与-tmp-对比)
  - [第19章 类型列表](#第19章-类型列表)
- [第七部分：高级设计模式](#第七部分高级设计模式)
  - [第20章 CRTP（奇异递归模板模式）](#第20章-crtp奇异递归模板模式)
  - [第21章 标签分发](#第21章-标签分发)
  - [第22章 基于策略的设计](#第22章-基于策略的设计)
  - [第23章 表达式模板](#第23章-表达式模板)
  - [第24章 C++17 模板新特性](#第24章-c17-模板新特性)

---

# 第一部分：模板基础

## 第1章 函数模板

> 📄 示例代码：[examples/01_basic_function_template.cpp](examples/01_basic_function_template.cpp)

### 1.1 为什么需要模板？

假设你需要写一个函数来比较两个值的大小并返回较大的那个。对于 `int` 类型，你会写：

```cpp
int maxInt(int a, int b) {
    return (a > b) ? a : b;
}
```

对于 `double` 类型，你又要写一个几乎一模一样的函数：

```cpp
double maxDouble(double a, double b) {
    return (a > b) ? a : b;
}
```

这两个函数的**逻辑完全相同**，只是**类型不同**。如果还需要支持 `float`、`char`、`std::string` 等类型，你将不得不复制粘贴大量代码。这就是模板要解决的问题。

### 1.2 函数模板的语法

```cpp
template <typename T>
T myMax(T a, T b) {
    return (a > b) ? a : b;
}
```

**逐行解释：**

- `template` —— 关键字，告诉编译器"接下来是一个模板定义"
- `<typename T>` —— **模板参数列表**。`typename T` 声明了一个**类型参数** `T`
- `T` 是一个**占位符**，代表"某种类型"。在调用时，`T` 会被替换为具体的类型
- 函数签名 `T myMax(T a, T b)` 中的所有 `T` 都指同一个类型

> **注意**：`typename` 也可以写成 `class`，两者在模板参数声明中**完全等价**：
> ```cpp
> template <class T>  // 和 template <typename T> 完全相同
> T myMax(T a, T b) { ... }
> ```

### 1.3 调用函数模板

**方式一：显式指定类型参数**

```cpp
myMax<int>(3, 5);      // 把 T 替换为 int，调用 int myMax(int, int)
myMax<double>(3.14, 2.72);  // 把 T 替换为 double
```

**方式二：让编译器自动推导（推荐）**

```cpp
myMax(3, 5);           // 编译器看到两个 int，推导 T = int
myMax(3.14, 2.72);     // 编译器看到两个 double，推导 T = double
myMax('a', 'z');       // 推导 T = char
```

### 1.4 模板实例化

当你调用 `myMax<int>(3, 5)` 时，编译器会**生成**一个专门针对 `int` 的函数：

```cpp
int myMax(int a, int b) {
    return (a > b) ? a : b;
}
```

这个过程叫做**模板实例化**（Template Instantiation）。每个不同的类型参数都会生成一个独立的函数。

---

## 第2章 多类型参数与返回类型推导

> 📄 示例代码：[examples/02_multiple_type_params.cpp](examples/02_multiple_type_params.cpp)

### 2.1 多个类型参数

模板可以有多个类型参数：

```cpp
template <typename T1, typename T2>
void printPair(T1 first, T2 second) {
    std::cout << "(" << first << ", " << second << ")" << std::endl;
}

printPair(42, 3.14);      // T1=int, T2=double
printPair("hello", 100);  // T1=const char*, T2=int
```

### 2.2 返回类型的问题

当两个参数类型不同时，返回类型是什么？

```cpp
// 方法一：返回类型固定为 T1（可能丢失精度）
template <typename T1, typename T2>
T1 addV1(T1 a, T2 b) { return a + b; }

addV1(1, 2.7);    // 返回 int，结果为 3（小数被截断！）
addV1(1.0, 2);    // 返回 double，结果正确
```

```cpp
// 方法二：额外模板参数指定返回类型
template <typename ReturnType, typename T1, typename T2>
ReturnType addV2(T1 a, T2 b) { return a + b; }

addV2<double>(1, 2.7);  // 显式指定返回 double
```

```cpp
// 方法三（C++11 推荐）：尾置返回类型 + decltype
template <typename T1, typename T2>
auto addV3(T1 a, T2 b) -> decltype(a + b) {
    return a + b;
}

addV3(1, 2.7);  // 自动推导返回类型为 double
```

`decltype(a + b)` 让编译器在编译期推导出 `a + b` 的结果类型。`int + double = double`。

---

## 第3章 模板参数推导规则

> 📄 示例代码：[examples/03_template_argument_deduction.cpp](examples/03_template_argument_deduction.cpp)

### 3.1 按值传递的推导

```cpp
template <typename T>
void show(T param) { ... }

int x = 42;
const int cx = 100;
const int& rcx = cx;

show(x);    // T = int
show(cx);   // T = int  （const 被忽略！）
show(rcx);  // T = int  （const 和 & 都被忽略！）
```

**规则：按值传递时，`T` 推导的结果会去掉 `const`、`volatile` 和引用。** 这是因为按值传递会创建副本，副本不需要保持原变量的 `const` 属性。

### 3.2 按引用传递的推导

```cpp
template <typename T>
void show(T& param) { ... }

int x = 42;
const int cx = 100;

show(x);   // T = int       （param 类型为 int&）
show(cx);  // T = const int （param 类型为 const int&，保留 const！）
```

**规则：按引用传递时，`T` 保留 `const` 属性。**

### 3.3 数组的推导

```cpp
int arr[10] = {};

// 按值传递：数组退化为指针
template <typename T> void f(T param);
f(arr);  // T = int*

// 按引用传递：保留完整数组类型
template <typename T> void g(T& param);
g(arr);  // T = int[10]
```

利用引用传递获取数组大小的技巧：

```cpp
template <typename T, int N>
int getArraySize(T (&arr)[N]) {
    return N;
}

int nums[] = {1, 2, 3, 4, 5};
getArraySize(nums);  // 返回 5
```

### 3.4 推导失败的情况

```cpp
template <typename T>
T myMax(T a, T b) { return (a > b) ? a : b; }

myMax(3, 5.0);  // 错误！3 是 int，5.0 是 double，无法统一推导 T
```

解决方法：
1. 显式指定：`myMax<double>(3, 5.0)`
2. 强制转换：`myMax(static_cast<double>(3), 5.0)`

---

## 第4章 函数模板重载

> 📄 示例代码：[examples/04_function_template_overload.cpp](examples/04_function_template_overload.cpp)

### 4.1 模板函数与普通函数共存

```cpp
// 普通函数
int myMax(int a, int b) { return (a > b) ? a : b; }

// 模板函数
template <typename T>
T myMax(T a, T b) { return (a > b) ? a : b; }
```

**重载决议规则：**
1. 当两者都能匹配时，**优先选择普通函数**
2. 如果模板能提供更好的匹配，则选择模板
3. 使用 `myMax<>()` 可以强制使用模板版本

```cpp
myMax(3, 5);      // 调用普通函数（完美匹配）
myMax<>(3, 5);    // 强制使用模板版本
myMax(3.0, 5.0);  // 调用模板（普通函数不匹配 double）
```

### 4.2 函数模板之间的重载

```cpp
template <typename T>
T myMax(T a) { return a; }

template <typename T>
T myMax(T a, T b) { return (a > b) ? a : b; }

template <typename T>
T myMax(T a, T b, T c) { return myMax(myMax(a, b), c); }
```

---

# 第二部分：类模板

## 第5章 类模板基础

> 📄 示例代码：[examples/05_basic_class_template.cpp](examples/05_basic_class_template.cpp)

### 5.1 定义类模板

```cpp
template <typename T>
class Stack {
private:
    static const int MAX_SIZE = 100;
    T elements[MAX_SIZE];  // 用类型参数 T 定义数据
    int topIndex;

public:
    Stack() : topIndex(-1) {}
    void push(const T& value);
    T pop();
    const T& top() const;
    bool isEmpty() const;
};
```

### 5.2 使用类模板

与函数模板不同，类模板**必须显式指定类型参数**（C++17 之前）：

```cpp
Stack<int> intStack;         // T = int
Stack<std::string> strStack; // T = std::string
Stack<double> doubleStack;   // T = double
```

> **C++17**（`CTAD`，类模板参数推导）之后才可以省略类型参数，详见第24章。

### 5.3 重要概念

- `Stack<int>` 和 `Stack<double>` 是**完全不同的类型**，它们之间没有任何关系
- 模板类的成员函数只有在**被调用时才会实例化**
- 模板代码通常需要放在头文件中（因为编译器需要在实例化时看到完整定义）

---

## 第6章 类模板成员详解

> 📄 示例代码：[examples/06_class_template_members.cpp](examples/06_class_template_members.cpp)

### 6.1 在类外定义成员函数

```cpp
template <typename T>
class Container {
public:
    void add(const T& value);  // 类内声明
};

// 类外定义：必须重复 template 前缀，并使用 Container<T>::
template <typename T>
void Container<T>::add(const T& value) {
    // 实现...
}
```

### 6.2 静态成员

每个模板实例有**独立的静态成员**：

```cpp
template <typename T>
class Counter {
public:
    static int instanceCount;  // 每个 Counter<T> 有自己的一份
};

template <typename T>
int Counter<T>::instanceCount = 0;  // 类外初始化

// Counter<int>::instanceCount 和 Counter<double>::instanceCount 是独立的！
```

### 6.3 嵌套类型

```cpp
template <typename T>
class DataWrapper {
public:
    typedef T value_type;          // C++98 风格
    using reference = T&;          // C++11 风格（推荐）
    using const_reference = const T&;

    class Iterator { ... };        // 嵌套类
};

// 使用嵌套类型
DataWrapper<int>::value_type val;   // 即 int
DataWrapper<int>::Iterator it;
```

---

## 第7章 默认模板参数与成员模板

> 📄 示例代码：[examples/07_class_template_default_args.cpp](examples/07_class_template_default_args.cpp)

### 7.1 默认模板参数

```cpp
template <typename T, typename ContainerType = std::vector<T>>
class MyStack {
    ContainerType container;  // 底层容器，默认用 vector
    // ...
};

MyStack<int> s1;                          // 使用默认容器 vector
MyStack<int, std::deque<int>> s2;         // 显式指定用 deque
MyStack<std::string, std::list<std::string>> s3;  // 用 list
```

### 7.2 成员模板

成员模板允许类模板的成员函数有自己独立的模板参数：

```cpp
template <typename T>
class Wrapper {
    T value;
public:
    Wrapper(const T& v) : value(v) {}

    // 成员模板：允许从不同类型的 Wrapper 构造
    template <typename U>
    Wrapper(const Wrapper<U>& other) : value(other.getValue()) {}

    T getValue() const { return value; }
};

Wrapper<int> wi(42);
Wrapper<double> wd = wi;  // 从 Wrapper<int> 构造 Wrapper<double>
```

---

# 第三部分：非类型参数与特化

## 第8章 非类型模板参数

> 📄 示例代码：[examples/08_nontype_template_params.cpp](examples/08_nontype_template_params.cpp)

### 8.1 什么是非类型模板参数

模板参数不仅可以是类型（`typename T`），还可以是**编译期常量值**：

```cpp
template <typename T, int N>
class FixedArray {
    T data[N];  // 数组大小 N 在编译期确定
public:
    int size() const { return N; }
};

FixedArray<int, 5> arr5;   // 大小为 5 的 int 数组
FixedArray<int, 3> arr3;   // 大小为 3 的 int 数组
// 注意：arr5 和 arr3 是完全不同的类型！
```

### 8.2 允许的非类型参数类型

C++11 中允许的非类型模板参数类型：
- **整数类型**（`int`、`unsigned`、`long`、`char`、`bool`、枚举等）
- **指向对象/函数的指针**
- **左值引用**
- `std::nullptr_t`

**不允许**：`float`、`double`、类类型等（C++20 放宽了此限制）。

### 8.3 编译期计算

```cpp
template <int Base, int Exp>
struct Power {
    static const int value = Base * Power<Base, Exp - 1>::value;
};

template <int Base>
struct Power<Base, 0> {
    static const int value = 1;
};

// 编译期计算 2^10 = 1024
int arr[Power<2, 10>::value];  // 等价于 int arr[1024]
```

### 8.4 `bool` 非类型参数

```cpp
template <typename T, bool Ascending = true>
void sortTwo(T& a, T& b) {
    if (Ascending) {
        if (a > b) std::swap(a, b);
    } else {
        if (a < b) std::swap(a, b);
    }
}
```

---

## 第9章 完全特化

> 📄 示例代码：[examples/09_full_specialization.cpp](examples/09_full_specialization.cpp)

### 9.1 什么是特化

特化（Specialization）是指为模板的某些特定类型参数提供**专门的实现**。

### 9.2 类模板的完全特化

```cpp
// 主模板（Primary Template）：通用版本
template <typename T>
class TypeInfo {
public:
    static std::string name() { return "unknown"; }
};

// 针对 int 的完全特化
// template <> 表示"没有模板参数需要推导"
template <>
class TypeInfo<int> {
public:
    static std::string name() { return "int"; }
};

// 针对 double 的完全特化
template <>
class TypeInfo<double> {
public:
    static std::string name() { return "double"; }
};
```

**语法要点：**
- `template <>` 表示这是一个完全特化（所有模板参数都已确定）
- `TypeInfo<int>` 指定了特化的具体类型
- 特化版本**完全独立**，可以有与主模板不同的成员

### 9.3 函数模板的完全特化

```cpp
// 主模板
template <typename T>
bool isEqual(const T& a, const T& b) {
    return a == b;
}

// 针对 double 的特化：浮点数用 epsilon 比较
template <>
bool isEqual<double>(const double& a, const double& b) {
    const double epsilon = 1e-9;
    return (a - b < epsilon) && (b - a < epsilon);
}
```

**实际应用：** `0.1 + 0.2 == 0.3` 直接比较可能返回 `false`（浮点精度问题），但特化版本使用 epsilon 比较会返回 `true`。

### 9.4 特化用于优化

标准库中 `std::vector<bool>` 就是一个著名的完全特化例子——它用位压缩存储来节省内存，8 个 `bool` 只占 1 个字节。

---

## 第10章 偏特化

> 📄 示例代码：[examples/10_partial_specialization.cpp](examples/10_partial_specialization.cpp)

### 10.1 偏特化 vs 完全特化

- **完全特化**：所有模板参数都确定，如 `Template<int>`
- **偏特化**：部分模板参数确定，或对参数施加约束，如 `Template<T*>`

**重要限制：只有类模板可以偏特化，函数模板不能偏特化。**

### 10.2 针对指针类型的偏特化

```cpp
// 主模板
template <typename T>
class Storage {
    T value;
public:
    Storage(const T& v) : value(v) {}
};

// 偏特化：当 T 是指针类型时
template <typename U>
class Storage<U*> {   // U* 匹配任何指针类型
    U* ptr;
public:
    Storage(U* p) : ptr(p) {}
    U& get() const { return *ptr; }
};
```

编译器根据实际类型选择最匹配的版本：
- `Storage<int>` → 使用主模板
- `Storage<int*>` → 使用偏特化版本 `Storage<U*>`（U=int）
- `Storage<double*>` → 使用偏特化版本（U=double）

### 10.3 多参数偏特化

```cpp
template <typename T1, typename T2>
class Pair { ... };                   // 主模板

template <typename T>
class Pair<T, T> { ... };            // 偏特化1：两个类型相同

template <typename T>
class Pair<T, int> { ... };          // 偏特化2：第二个类型是 int

template <typename T1, typename T2>
class Pair<T1*, T2*> { ... };        // 偏特化3：两个都是指针
```

**歧义注意：** `Pair<int, int>` 同时匹配 `Pair<T, T>` 和 `Pair<T, int>`，导致编译错误！编译器无法决定哪个"更特化"。

### 10.4 偏特化实现类型特征

```cpp
template <typename T>
struct RemovePointer { typedef T type; };

template <typename U>
struct RemovePointer<U*> { typedef U type; };

// RemovePointer<int*>::type 就是 int
```

---

## 第11章 模板模板参数

> 📄 示例代码：[examples/11_template_template_params.cpp](examples/11_template_template_params.cpp)

### 11.1 问题引入

```cpp
// 方式一：传入完整容器类型——冗余！
template <typename T, typename Container = std::vector<T>>
class MyStack { ... };

MyStack<int, std::deque<int>> s;  // int 写了两次

// 方式二：模板模板参数——更简洁
template <typename T,
          template <typename, typename> class Container = std::vector>
class MyStack { ... };

MyStack<int, std::deque> s;  // int 只写一次
```

### 11.2 语法详解

```cpp
template <typename T,
          template <typename, typename> class Container = std::vector>
class MyStack {
    Container<T, std::allocator<T>> data;  // 实例化容器
    // ...
};
```

- `template <typename, typename> class Container` —— Container 本身是一个模板，接受两个类型参数
- 标准库容器通常有两个参数：元素类型和分配器
- `Container = std::vector` 是默认值

---

# 第四部分：可变参数模板

## 第12章 可变参数模板基础

> 📄 示例代码：[examples/12_variadic_templates_basic.cpp](examples/12_variadic_templates_basic.cpp)
>
> **C++11 引入**

### 12.1 参数包

```cpp
template <typename... Args>  // Args 是「类型参数包」
void func(Args... args) {}   // args 是「函数参数包」
```

- `typename... Args` 声明了可以接受 **0 个或多个** 类型参数
- `Args... args` 对应 0 个或多个函数参数

### 12.2 递归展开模式

可变参数模板最常见的使用方式是**递归**：

```cpp
// 递归终止条件
void print() {
    std::cout << std::endl;
}

// 递归展开：取出第一个参数，递归处理剩余
template <typename T, typename... Args>
void print(const T& first, const Args&... rest) {
    std::cout << first;
    if (sizeof...(rest) > 0) std::cout << ", ";
    print(rest...);  // 递归
}

print(1, 2.5, "hello", 'A');  // 输出：1, 2.5, hello, A
```

**展开过程：**
1. `print(1, 2.5, "hello", 'A')` → T=int, first=1, rest={2.5, "hello", 'A'}
2. `print(2.5, "hello", 'A')` → T=double, first=2.5, rest={"hello", 'A'}
3. `print("hello", 'A')` → T=const char*, first="hello", rest={'A'}
4. `print('A')` → T=char, first='A', rest={}
5. `print()` → 调用终止版本

### 12.3 `sizeof...` 运算符

```cpp
template <typename... Args>
void count(Args... args) {
    std::cout << sizeof...(Args) << std::endl;  // 类型参数个数
    std::cout << sizeof...(args) << std::endl;  // 值参数个数
}
```

### 12.4 可变参数求和

```cpp
template <typename T>
T sum(T value) { return value; }  // 终止

template <typename T, typename... Args>
T sum(T first, Args... rest) {
    return first + sum(rest...);
}

sum(1, 2, 3, 4, 5);  // = 15
```

---

## 第13章 可变参数模板高级用法

> 📄 示例代码：[examples/13_variadic_templates_advanced.cpp](examples/13_variadic_templates_advanced.cpp)

### 13.1 初始化列表展开技巧

C++11 中没有折叠表达式（那是 C++17），可以用初始化列表来展开参数包：

```cpp
template <typename... Args>
void printSquares(const Args&... args) {
    // 利用初始化列表展开参数包
    int dummy[] = { (std::cout << (args * args) << " ", 0)... };
    (void)dummy;
    std::cout << std::endl;
}
```

`{ (expression, 0)... }` 对参数包中的每个元素都执行 `expression`。

### 13.2 完美转发

可变参数模板 + `std::forward` = 完美转发：

```cpp
template <typename T, typename... Args>
std::unique_ptr<T> myMakeUnique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
```

- `Args&&...` 是**万能引用**
- `std::forward<Args>(args)...` 保持每个参数的左值/右值属性
- 这就是 `std::make_unique`（C++14）的实现原理

### 13.3 递归继承实现 Tuple

`std::tuple` 的实现原理是递归继承：

```cpp
template <typename... Types>
class MyTuple;

template <>
class MyTuple<> {};  // 空 tuple，递归终止

template <typename Head, typename... Tail>
class MyTuple<Head, Tail...> : private MyTuple<Tail...> {
    Head value;
public:
    MyTuple(const Head& h, const Tail&... t) : MyTuple<Tail...>(t...), value(h) {}
    Head head() const { return value; }
};
```

`MyTuple<int, double, string>` 的继承链：
```
MyTuple<int, double, string>
  └── MyTuple<double, string>
        └── MyTuple<string>
              └── MyTuple<>
```

---

# 第五部分：类型特征与 SFINAE

## 第14章 标准库类型特征

> 📄 示例代码：[examples/14_type_traits.cpp](examples/14_type_traits.cpp)
>
> **C++11 引入 `<type_traits>` 头文件**

### 14.1 类型查询特征

```cpp
#include <type_traits>

std::is_integral<int>::value        // true  —— 是否为整数类型
std::is_floating_point<double>::value // true  —— 是否为浮点类型
std::is_arithmetic<int>::value      // true  —— 是否为算术类型
std::is_pointer<int*>::value        // true  —— 是否为指针
std::is_reference<int&>::value      // true  —— 是否为引用
std::is_const<const int>::value     // true  —— 是否有 const 修饰
std::is_class<std::string>::value   // true  —— 是否为类类型
std::is_array<int[10]>::value       // true  —— 是否为数组
```

### 14.2 类型修改特征

```cpp
std::remove_const<const int>::type     // int
std::remove_reference<int&>::type      // int
std::remove_pointer<int*>::type        // int
std::add_const<int>::type              // const int
std::add_pointer<int>::type            // int*
```

### 14.3 类型关系特征

```cpp
std::is_same<int, int>::value          // true
std::is_base_of<Base, Derived>::value  // true
std::is_convertible<int, double>::value // true
```

### 14.4 `std::conditional` — 编译期三元表达式

```cpp
// 如果条件为 true，选择第一个类型；否则选择第二个
std::conditional<true, int, double>::type   // int
std::conditional<false, int, double>::type  // double

// 实际应用：根据类型选择更安全的计算类型
template <typename T>
using SafeType = typename std::conditional<
    std::is_integral<T>::value,
    long long,    // 整数用 long long 防溢出
    double        // 其他用 double
>::type;
```

### 14.5 `std::enable_if` — 条件启用

```cpp
// 只对整数类型启用的函数
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
safeDiv(T a, T b) {
    if (b == 0) return 0;
    return a / b;
}

// 只对浮点类型启用的函数
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
safeDiv(T a, T b) {
    if (b == 0.0) return 0.0;
    return a / b;
}
```

---

## 第15章 自定义类型特征

> 📄 示例代码：[examples/15_custom_type_traits.cpp](examples/15_custom_type_traits.cpp)

### 15.1 实现原理

标准库的类型特征都是通过**模板特化**实现的：

```cpp
// 自定义 is_pointer
template <typename T>
struct my_is_pointer { static const bool value = false; };

template <typename U>
struct my_is_pointer<U*> { static const bool value = true; };
```

### 15.2 检测成员函数是否存在

```cpp
template <typename T>
class has_size_method {
private:
    template <typename U>
    static auto test(int)
        -> decltype(std::declval<U>().size(), std::true_type{});

    template <typename>
    static std::false_type test(...);

public:
    static const bool value = decltype(test<T>(0))::value;
};
```

**工作原理：**
1. `test(int)` 尝试调用 `T` 的 `size()` 方法
2. 如果 `T` 有 `size()` → `decltype` 成功 → 返回 `true_type`
3. 如果 `T` 没有 `size()` → 替换失败（SFINAE） → 退到 `test(...)` → 返回 `false_type`
4. `test<T>(0)` 优先匹配 `test(int)`（0 是 `int`）

### 15.3 自定义 `enable_if`

```cpp
template <bool Condition, typename T = void>
struct my_enable_if {};  // 条件为 false：没有 type 成员

template <typename T>
struct my_enable_if<true, T> {
    typedef T type;       // 条件为 true：有 type 成员
};
```

---

## 第16章 SFINAE 原则

> 📄 示例代码：[examples/16_sfinae.cpp](examples/16_sfinae.cpp)

### 16.1 什么是 SFINAE

**SFINAE** = **S**ubstitution **F**ailure **I**s **N**ot **A**n **E**rror
（替换失败不是错误）

当编译器尝试用模板参数替换模板定义时，如果替换导致了无效的代码，编译器**不会报错**，而是**忽略这个模板**，继续尝试其他候选。

### 16.2 基本示例

```cpp
template <typename T>
typename std::enable_if<std::is_integral<T>::value, std::string>::type
classify(T value) {
    return std::to_string(value) + " 是整数类型";
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, std::string>::type
classify(T value) {
    return std::to_string(value) + " 是浮点类型";
}

classify(42);    // 调用第一个版本（int 是 integral）
classify(3.14);  // 调用第二个版本（double 是 floating_point）
```

**工作过程（以 `classify(42)` 为例）：**
1. 编译器尝试版本1：`is_integral<int>::value = true` → `enable_if` 有 `type` 成员 → ✓ 有效
2. 编译器尝试版本2：`is_floating_point<int>::value = false` → `enable_if` 没有 `type` 成员 → 替换失败 → SFINAE 忽略
3. 只剩版本1，选择版本1

### 16.3 `enable_if` 的三种位置

```cpp
// 位置A：在返回类型中
template <typename T>
typename std::enable_if<std::is_integral<T>::value, bool>::type
isPositive(T value) { return value > 0; }

// 位置B：在模板参数中（推荐，不污染返回类型）
template <typename T,
          typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
bool isPositive(T value) { return value > 0; }

// 位置C：在函数参数中
template <typename T>
bool isPositive(T value,
    typename std::enable_if<std::is_integral<T>::value, int>::type = 0)
{ return value > 0; }
```

### 16.4 `void_t` 技巧

```cpp
// C++11 手动实现 void_t（C++17 标准化为 std::void_t）
template <typename...>
struct make_void { typedef void type; };

template <typename... Ts>
using my_void_t = typename make_void<Ts...>::type;

// 检测 T 是否有 push_back 方法
template <typename T, typename = void>
struct has_push_back : std::false_type {};

template <typename T>
struct has_push_back<T, my_void_t<
    decltype(std::declval<T>().push_back(std::declval<typename T::value_type>()))
>> : std::true_type {};
```

---

# 第六部分：模板元编程

## 第17章 编译期计算

> 📄 示例代码：[examples/17_compile_time_computation.cpp](examples/17_compile_time_computation.cpp)

### 17.1 模板元编程的本质

模板元编程（Template Metaprogramming, TMP）是利用 C++ 模板系统在**编译期**进行计算的技术。

核心思想：
- **变量** → 模板参数（值或类型）
- **函数** → 模板类/结构体
- **返回值** → `static const` 成员或 `typedef`
- **循环** → 递归模板实例化
- **条件判断** → 模板特化

### 17.2 阶乘（经典入门）

```cpp
template <int N>
struct Factorial {
    static const int value = N * Factorial<N - 1>::value;
};

template <>  // 递归终止
struct Factorial<0> {
    static const int value = 1;
};

// Factorial<5>::value 在编译期就等于 120
static_assert(Factorial<5>::value == 120, "5! should be 120");
```

### 17.3 斐波那契

```cpp
template <int N>
struct Fibonacci {
    static const long long value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};
template <> struct Fibonacci<0> { static const long long value = 0; };
template <> struct Fibonacci<1> { static const long long value = 1; };
```

### 17.4 最大公约数

```cpp
template <int A, int B>
struct GCD {
    static const int value = GCD<B, A % B>::value;
};
template <int A>
struct GCD<A, 0> { static const int value = A; };
```

### 17.5 编译期条件选择

```cpp
template <bool Condition, int TrueVal, int FalseVal>
struct If { static const int value = TrueVal; };

template <int TrueVal, int FalseVal>
struct If<false, TrueVal, FalseVal> { static const int value = FalseVal; };

// 编译期绝对值
template <int N>
struct Abs { static const int value = If<(N >= 0), N, -N>::value; };
```

### 17.6 素数判断

详见示例代码，展示了更复杂的编译期算法。

### 17.7 关键特征

编译期计算的结果可以用于：
- 数组大小：`int arr[Factorial<5>::value]`（等价于 `int arr[120]`）
- `static_assert`：`static_assert(IsPrime<7>::value, "7 is prime")`
- 模板参数：`MyArray<Factorial<3>::value>`

---

## 第18章 constexpr 与 TMP 对比

> 📄 示例代码：[examples/18_constexpr_vs_tmp.cpp](examples/18_constexpr_vs_tmp.cpp)
>
> **C++11 引入 `constexpr`**

### 18.1 `constexpr` 函数

```cpp
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

constexpr int result = factorial(5);  // 编译期计算
int n = 7;
int runtime = factorial(n);           // 运行期计算（也可以！）
```

> **C++11 限制**：`constexpr` 函数只能有一个 `return` 语句。
> **C++14 放宽**：允许局部变量、循环、多个 return 等。

### 18.2 对比

| 特性 | TMP (模板元编程) | constexpr |
|------|-----------------|-----------|
| 语法 | 复杂（递归模板特化） | 简单（像普通函数） |
| 使用场景 | 编译期 only | 编译期 + 运行期 |
| 浮点运算 | 不支持 | 支持 |
| 类型操作 | 支持 | 不支持 |
| 错误信息 | 难以理解 | 友好 |

**最佳实践：**
- 纯数值计算 → 用 `constexpr`
- 类型计算/类型变换 → 用 TMP
- 两者经常结合使用

### 18.3 `constexpr` 类

```cpp
class Point {
    double x_, y_;
public:
    constexpr Point(double x, double y) : x_(x), y_(y) {}
    constexpr double x() const { return x_; }
    constexpr double y() const { return y_; }
};

constexpr Point p(3.0, 4.0);              // 编译期构造
constexpr double d = p.x() * p.x() + p.y() * p.y();  // 编译期计算 = 25
```

---

## 第19章 类型列表

> 📄 示例代码：[examples/19_type_list.cpp](examples/19_type_list.cpp)

### 19.1 TypeList 定义

TypeList 是模板元编程中的"链表"，存储一系列类型：

```cpp
template <typename... Types>
struct TypeList {};

typedef TypeList<int, double, char> MyList;
```

### 19.2 基本操作

```cpp
// 获取长度
template <typename... Types>
struct Length<TypeList<Types...>> {
    static const int value = sizeof...(Types);
};

// 获取第 N 个类型
template <typename Head, typename... Tail>
struct TypeAt<TypeList<Head, Tail...>, 0> {
    typedef Head type;
};

// 头部添加
template <typename... Types, typename New>
struct PushFront<TypeList<Types...>, New> {
    typedef TypeList<New, Types...> type;
};

// 判断是否包含
template <typename T>
struct Contains<TypeList<>, T> { static const bool value = false; };

template <typename... Tail, typename T>
struct Contains<TypeList<T, Tail...>, T> { static const bool value = true; };
```

### 19.3 更多操作

示例代码还包含：`PushBack`、`PopFront`、`IndexOf`、`Concat`、`Reverse` 等完整实现。

---

# 第七部分：高级设计模式

## 第20章 CRTP（奇异递归模板模式）

> 📄 示例代码：[examples/20_crtp.cpp](examples/20_crtp.cpp)

### 20.1 什么是 CRTP

CRTP（Curiously Recurring Template Pattern）：**派生类把自己作为模板参数传给基类**。

```cpp
template <typename Derived>
class Base { ... };

class MyClass : public Base<MyClass> { ... };
```

### 20.2 静态多态（无虚函数表的多态）

```cpp
template <typename Derived>
class Shape {
public:
    double area() const {
        return static_cast<const Derived*>(this)->areaImpl();
    }
};

class Circle : public Shape<Circle> {
    double radius;
public:
    double areaImpl() const { return 3.14159 * radius * radius; }
};

class Rectangle : public Shape<Rectangle> {
    double w, h;
public:
    double areaImpl() const { return w * h; }
};
```

**优势**：没有虚函数表（vtable）开销，所有调用在编译期确定。

### 20.3 混入功能（Mixin）

```cpp
template <typename Derived>
class Comparable {
public:
    bool operator>(const Derived& other) const {
        return other < static_cast<const Derived&>(*this);
    }
    bool operator<=(const Derived& other) const { ... }
    bool operator>=(const Derived& other) const { ... }
    bool operator==(const Derived& other) const { ... }
};

class Temperature : public Comparable<Temperature> {
public:
    bool operator<(const Temperature& other) const { ... }
    // 自动获得 >, <=, >=, ==, != 五个运算符！
};
```

### 20.4 对象计数

```cpp
template <typename Derived>
class ObjectCounter {
    static int count;
protected:
    ObjectCounter() { ++count; }
    ~ObjectCounter() { --count; }
public:
    static int getCount() { return count; }
};

class Dog : public ObjectCounter<Dog> { ... };
class Cat : public ObjectCounter<Cat> { ... };
// Dog 和 Cat 各自独立计数
```

---

## 第21章 标签分发

> 📄 示例代码：[examples/21_tag_dispatch.cpp](examples/21_tag_dispatch.cpp)

### 21.1 基本思想

使用空的**标签类型**来选择函数重载：

```cpp
// 不同迭代器类型的实现
template <typename Iter>
void advance_impl(Iter& it, int n, std::random_access_iterator_tag) {
    it += n;  // O(1)
}

template <typename Iter>
void advance_impl(Iter& it, int n, std::bidirectional_iterator_tag) {
    while (n-- > 0) ++it;  // O(n)
}

// 统一入口
template <typename Iter>
void myAdvance(Iter& it, int n) {
    advance_impl(it, n,
        typename std::iterator_traits<Iter>::iterator_category{});
}
```

### 21.2 标签分发 vs enable_if

| 特性 | 标签分发 | enable_if |
|------|---------|-----------|
| 可读性 | 更好 | 较差 |
| 错误信息 | 更友好 | 不友好 |
| 扩展性 | 更容易添加新分支 | 需要修改条件表达式 |
| 灵活性 | 受限于标签类型 | 可以用任意布尔条件 |

---

## 第22章 基于策略的设计

> 📄 示例代码：[examples/22_policy_based_design.cpp](examples/22_policy_based_design.cpp)

### 22.1 概念

通过模板参数注入**不同的行为策略**，实现高度可配置的类：

```cpp
struct ConsoleLogPolicy {
    static void log(const std::string& msg) { std::cout << msg << std::endl; }
};

struct NullLogPolicy {
    static void log(const std::string&) {}  // 什么都不做
};

template <typename LogPolicy = ConsoleLogPolicy>
class Application {
public:
    void doWork() {
        LogPolicy::log("开始工作...");
        // ...
        LogPolicy::log("工作完成!");
    }
};

Application<ConsoleLogPolicy> debug_app;  // 输出日志
Application<NullLogPolicy> release_app;   // 不输出日志
```

### 22.2 多策略组合

```cpp
template <typename T,
          template <typename> class StoragePolicy = DynamicStoragePolicy,
          typename ThreadPolicy = NoLockPolicy>
class SmartContainer : private StoragePolicy<T>, private ThreadPolicy {
    // 组合了存储策略和线程安全策略
};
```

---

## 第23章 表达式模板

> 📄 示例代码：[examples/23_expression_templates.cpp](examples/23_expression_templates.cpp)

### 23.1 问题

朴素的向量加法 `v4 = v1 + v2 + v3` 会创建 2 个临时对象，执行 3 次遍历。

### 23.2 表达式模板解决方案

**核心思想：不立即计算，而是构建表达式树，在最终赋值时一次性计算。**

```cpp
// 加法表达式对象（不实际计算）
template <typename E1, typename E2>
class VecSum {
    const E1& lhs;
    const E2& rhs;
public:
    VecSum(const E1& l, const E2& r) : lhs(l), rhs(r) {}
    double operator[](int i) const { return lhs[i] + rhs[i]; }  // 延迟计算
};

// 赋值时才真正计算
template <typename E>
Vec& operator=(const E& expr) {
    for (int i = 0; i < size(); ++i)
        data[i] = expr[i];  // 此时才逐元素计算 v1[i] + v2[i] + v3[i]
    return *this;
}
```

**效果：** 0 个临时向量对象，1 次遍历，1 次内存分配。这就是 Eigen、Blaze 等数学库的核心技术。

---

## 第24章 C++17 模板新特性

> 📄 示例代码：[examples/cpp17/24_cpp17_features.cpp](examples/cpp17/24_cpp17_features.cpp)
>
> **⚠️ 本章所有特性均为 C++17 引入，需要 `-std=c++17` 编译**

### 24.1 `if constexpr` — 编译期条件分支

```cpp
template <typename T>
std::string describe(const T& value) {
    if constexpr (std::is_integral<T>::value) {
        return std::to_string(value) + " 是整数";
    } else if constexpr (std::is_floating_point<T>::value) {
        return std::to_string(value) + " 是浮点数";
    } else {
        return "未知类型";
    }
}
```

**对比 C++11**：需要 3 个独立的 `enable_if` 函数。C++17 一个函数搞定！

### 24.2 折叠表达式（Fold Expressions）

```cpp
// C++11：需要递归终止函数
// C++17：一行搞定

template <typename... Args>
auto sum(Args... args) { return (args + ...); }       // 右折叠

template <typename... Args>
bool allTrue(Args... args) { return (args && ...); }   // 逻辑与

template <typename... Args>
void printAll(const Args&... args) {
    ((std::cout << args << " "), ...);                  // 逗号折叠
}
```

折叠方向：
- `(args + ...)` → **右折叠**：`a + (b + (c + d))`
- `(... + args)` → **左折叠**：`((a + b) + c) + d`

### 24.3 CTAD（类模板参数推导）

```cpp
// C++11/14
std::pair<int, double> p(42, 3.14);

// C++17：编译器自动推导模板参数
std::pair p(42, 3.14);  // 推导为 pair<int, double>
```

### 24.4 `auto` 非类型模板参数

```cpp
template <auto Value>
struct Constant {
    static constexpr auto value = Value;
};

Constant<42> ci;    // Value 类型自动推导为 int
Constant<'A'> cc;   // Value 类型自动推导为 char
Constant<true> cb;  // Value 类型自动推导为 bool
```

---

## 构建和运行

### 前提条件

- 支持 C++11 的编译器（GCC 4.8+, Clang 3.3+, MSVC 2015+）
- C++17 示例需要 GCC 7+, Clang 5+, MSVC 2017 15.7+
- CMake 3.10+

### 构建步骤

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 运行单个示例

```bash
./01_basic_function_template
./17_compile_time_computation
./24_cpp17_features
```

---

## 学习路径建议

1. **入门**（第1-7章）：理解模板的基本语法和使用方式
2. **进阶**（第8-13章）：掌握特化、非类型参数、可变参数模板
3. **深入**（第14-19章）：理解类型特征、SFINAE、编译期计算
4. **精通**（第20-24章）：学习高级设计模式和现代 C++ 特性

每一章的示例代码都可以独立编译运行，建议边看教程边运行代码，修改参数观察效果。

---

> 版本说明：
> - 本教程主要基于 **C++11** 标准
> - `constexpr` 函数基础 → C++11（C++14 放宽限制）
> - `if constexpr`、折叠表达式、CTAD、`auto` 非类型参数 → **C++17**
> - `std::void_t` 标准化 → **C++17**（C++11 中可手动实现）
