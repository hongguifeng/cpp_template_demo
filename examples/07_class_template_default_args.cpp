// 07_class_template_default_args.cpp
// 演示：类模板的默认模板参数和成员模板

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>

// ============================================================
// 1. 默认模板参数
// ============================================================

// Container 的默认底层容器是 std::vector
// 用户可以替换为 std::deque、std::list 等
template <typename T, typename ContainerType = std::vector<T>>
class MyStack {
private:
    ContainerType container;

public:
    void push(const T& value) {
        container.push_back(value);
    }

    T pop() {
        T val = container.back();
        container.pop_back();
        return val;
    }

    const T& top() const {
        return container.back();
    }

    bool empty() const {
        return container.empty();
    }

    int size() const {
        return static_cast<int>(container.size());
    }
};

// ============================================================
// 2. 成员模板（Member Template）
// ============================================================

template <typename T>
class Wrapper {
private:
    T value;

public:
    // 普通构造函数
    Wrapper(const T& v) : value(v) {}

    // 成员模板：允许从不同类型的 Wrapper 进行转换
    // 例如：Wrapper<double> 可以从 Wrapper<int> 构造
    template <typename U>
    Wrapper(const Wrapper<U>& other) : value(other.getValue()) {
        std::cout << "  [类型转换构造: 从 Wrapper<U> 到 Wrapper<T>]" << std::endl;
    }

    T getValue() const { return value; }

    // 成员模板函数：与任意类型的 Wrapper 相加
    template <typename U>
    auto add(const Wrapper<U>& other) const -> decltype(value + other.getValue()) {
        return value + other.getValue();
    }
};

// ============================================================
// 3. 多个默认参数
// ============================================================

// 键值对容器，默认使用 int 键和 std::string 值
template <typename Key = int, typename Value = std::string>
class SimpleMap {
private:
    struct Entry {
        Key key;
        Value value;
    };
    std::vector<Entry> entries;

public:
    void insert(const Key& k, const Value& v) {
        entries.push_back({k, v});
    }

    const Value* find(const Key& k) const {
        for (const auto& entry : entries) {
            if (entry.key == k) {
                return &entry.value;
            }
        }
        return nullptr;
    }

    void print() const {
        for (const auto& entry : entries) {
            std::cout << "  " << entry.key << " -> " << entry.value << std::endl;
        }
    }
};

int main() {
    // ============================================================
    // 1. 默认模板参数
    // ============================================================
    std::cout << "=== 默认模板参数 ===" << std::endl;

    // 使用默认容器（vector）
    MyStack<int> stack1;
    stack1.push(1);
    stack1.push(2);
    stack1.push(3);
    std::cout << "vector 栈顶: " << stack1.top() << std::endl;

    // 显式指定使用 deque 作为底层容器
    MyStack<int, std::deque<int>> stack2;
    stack2.push(10);
    stack2.push(20);
    std::cout << "deque 栈顶: " << stack2.top() << std::endl;

    // 使用 list 作为底层容器
    MyStack<std::string, std::list<std::string>> stack3;
    stack3.push("Hello");
    stack3.push("World");
    std::cout << "list 栈顶: " << stack3.top() << std::endl;

    // ============================================================
    // 2. 成员模板
    // ============================================================
    std::cout << "\n=== 成员模板 ===" << std::endl;

    Wrapper<int> wi(42);
    std::cout << "Wrapper<int>: " << wi.getValue() << std::endl;

    // 从 Wrapper<int> 构造 Wrapper<double>（隐式类型转换）
    Wrapper<double> wd = wi;
    std::cout << "Wrapper<double>(从int转换): " << wd.getValue() << std::endl;

    // 成员模板函数：不同类型相加
    Wrapper<int> wa(10);
    Wrapper<double> wb(3.14);
    auto sum = wa.add(wb);
    std::cout << "10 + 3.14 = " << sum << std::endl;

    // ============================================================
    // 3. 多个默认参数
    // ============================================================
    std::cout << "\n=== 多个默认参数 ===" << std::endl;

    // 使用全部默认参数：Key=int, Value=std::string
    SimpleMap<> defaultMap;
    defaultMap.insert(1, "one");
    defaultMap.insert(2, "two");
    std::cout << "默认类型 (int -> string):" << std::endl;
    defaultMap.print();

    // 指定 Key=std::string, 使用默认 Value=std::string
    SimpleMap<std::string> strMap;
    strMap.insert("name", "Alice");
    strMap.insert("city", "Beijing");
    std::cout << "string -> string:" << std::endl;
    strMap.print();

    // 全部自定义
    SimpleMap<std::string, int> strIntMap;
    strIntMap.insert("age", 25);
    strIntMap.insert("score", 100);
    std::cout << "string -> int:" << std::endl;
    strIntMap.print();

    return 0;
}
