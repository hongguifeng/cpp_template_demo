// 11_template_template_params.cpp
// 演示：模板模板参数（Template Template Parameters）

#include <iostream>
#include <vector>
#include <deque>
#include <list>
#include <string>

// ============================================================
// 1. 什么是模板模板参数
// ============================================================
// 问题：我们想写一个类，允许用户选择底层容器类型
// 方法一（前面见过）：传入完整的容器类型
//   template <typename T, typename Container = std::vector<T>>
//   这种方式用户需要写 MyClass<int, std::deque<int>>
//   T 出现了两次，有点冗余

// 方法二：使用模板模板参数
// Container 本身是一个模板，不是一个具体类型
// 用户只需要写 MyClass<int, std::deque>，更简洁

template <typename T,
          template <typename, typename> class Container = std::vector>
class MyStack {
private:
    // Container<T, std::allocator<T>> 是 Container 的实例化
    // 标准库容器通常有两个模板参数：元素类型 T 和分配器
    Container<T, std::allocator<T>> data;

public:
    void push(const T& val) { data.push_back(val); }

    T pop() {
        T val = data.back();
        data.pop_back();
        return val;
    }

    const T& top() const { return data.back(); }
    bool empty() const { return data.empty(); }
    int size() const { return static_cast<int>(data.size()); }

    void print() const {
        std::cout << "Stack[";
        bool first = true;
        for (const auto& elem : data) {
            if (!first) std::cout << ", ";
            std::cout << elem;
            first = false;
        }
        std::cout << "] (从底到顶)" << std::endl;
    }
};

// ============================================================
// 2. 更通用的例子：适配器
// ============================================================

// 用模板模板参数创建一个通用的容器适配器
template <typename T,
          template <typename, typename> class Container>
class ContainerAdapter {
private:
    Container<T, std::allocator<T>> container;

public:
    void add(const T& val) { container.push_back(val); }

    int size() const { return static_cast<int>(container.size()); }

    // 打印容器中的所有元素
    void printAll(const std::string& label) const {
        std::cout << label << ": [";
        bool first = true;
        for (const auto& elem : container) {
            if (!first) std::cout << ", ";
            std::cout << elem;
            first = false;
        }
        std::cout << "]" << std::endl;
    }
};

// ============================================================
// 3. 模板模板参数与普通模板参数混合
// ============================================================

// 创建一对使用不同底层容器的集合
template <typename T,
          template <typename, typename> class Container1,
          template <typename, typename> class Container2>
class DualContainer {
private:
    Container1<T, std::allocator<T>> first;
    Container2<T, std::allocator<T>> second;

public:
    void addToFirst(const T& val) { first.push_back(val); }
    void addToSecond(const T& val) { second.push_back(val); }

    void print() const {
        std::cout << "First:  [";
        for (const auto& e : first) std::cout << e << " ";
        std::cout << "]" << std::endl;

        std::cout << "Second: [";
        for (const auto& e : second) std::cout << e << " ";
        std::cout << "]" << std::endl;
    }
};

int main() {
    // ============================================================
    // 1. 模板模板参数基本使用
    // ============================================================
    std::cout << "=== MyStack with template template params ===" << std::endl;

    // 使用默认容器（vector）
    MyStack<int> vecStack;
    vecStack.push(1);
    vecStack.push(2);
    vecStack.push(3);
    std::cout << "vector栈: ";
    vecStack.print();

    // 使用 deque 作为底层容器
    MyStack<int, std::deque> dequeStack;
    dequeStack.push(10);
    dequeStack.push(20);
    dequeStack.push(30);
    std::cout << "deque栈: ";
    dequeStack.print();

    // 使用 list 作为底层容器
    MyStack<std::string, std::list> listStack;
    listStack.push("Hello");
    listStack.push("Template");
    listStack.push("World");
    std::cout << "list栈: ";
    listStack.print();

    // ============================================================
    // 2. 容器适配器
    // ============================================================
    std::cout << "\n=== ContainerAdapter ===" << std::endl;

    ContainerAdapter<int, std::vector> va;
    va.add(1); va.add(2); va.add(3);
    va.printAll("vector适配器");

    ContainerAdapter<int, std::deque> da;
    da.add(10); da.add(20); da.add(30);
    da.printAll("deque适配器");

    ContainerAdapter<int, std::list> la;
    la.add(100); la.add(200); la.add(300);
    la.printAll("list适配器");

    // ============================================================
    // 3. 双容器
    // ============================================================
    std::cout << "\n=== DualContainer ===" << std::endl;

    DualContainer<int, std::vector, std::list> dual;
    dual.addToFirst(1);
    dual.addToFirst(2);
    dual.addToSecond(10);
    dual.addToSecond(20);
    dual.addToSecond(30);
    dual.print();

    return 0;
}
