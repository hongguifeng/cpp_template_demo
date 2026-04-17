// 05_basic_class_template.cpp
// 演示：最基本的类模板定义和使用

#include <iostream>
#include <string>
#include <stdexcept>

// ============================================================
// 1. 定义一个类模板：简单的栈（Stack）
// ============================================================

// template <typename T> 声明这是一个类模板
// T 是类型参数，代表栈中存储的元素类型
template <typename T>
class Stack {
private:
    static const int MAX_SIZE = 100;
    T elements[MAX_SIZE];  // 用数组存储元素，类型为 T
    int topIndex;          // 栈顶索引

public:
    // 构造函数
    Stack() : topIndex(-1) {}

    // 压栈
    void push(const T& value) {
        if (topIndex >= MAX_SIZE - 1) {
            throw std::overflow_error("Stack overflow");
        }
        elements[++topIndex] = value;
    }

    // 弹栈
    T pop() {
        if (isEmpty()) {
            throw std::underflow_error("Stack underflow");
        }
        return elements[topIndex--];
    }

    // 查看栈顶元素
    const T& top() const {
        if (isEmpty()) {
            throw std::underflow_error("Stack is empty");
        }
        return elements[topIndex];
    }

    // 判断是否为空
    bool isEmpty() const {
        return topIndex < 0;
    }

    // 获取大小
    int size() const {
        return topIndex + 1;
    }
};

int main() {
    // ============================================================
    // 2. 使用类模板：必须显式指定类型参数
    // ============================================================

    // Stack<int> 是一个「模板实例」，存储 int 类型
    std::cout << "=== int 栈 ===" << std::endl;
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    std::cout << "栈顶: " << intStack.top() << std::endl;
    std::cout << "大小: " << intStack.size() << std::endl;
    std::cout << "弹出: " << intStack.pop() << std::endl;
    std::cout << "弹出: " << intStack.pop() << std::endl;
    std::cout << "弹出: " << intStack.pop() << std::endl;
    std::cout << "空?   " << (intStack.isEmpty() ? "是" : "否") << std::endl;

    // Stack<std::string> 是另一个「模板实例」，存储 string 类型
    std::cout << "\n=== string 栈 ===" << std::endl;
    Stack<std::string> strStack;
    strStack.push("Hello");
    strStack.push("World");
    strStack.push("Template");
    while (!strStack.isEmpty()) {
        std::cout << "弹出: " << strStack.pop() << std::endl;
    }

    // Stack<double> 又是另一个实例
    std::cout << "\n=== double 栈 ===" << std::endl;
    Stack<double> doubleStack;
    doubleStack.push(3.14);
    doubleStack.push(2.72);
    std::cout << "栈顶: " << doubleStack.top() << std::endl;

    // ============================================================
    // 3. 异常处理示例
    // ============================================================
    std::cout << "\n=== 异常处理 ===" << std::endl;
    try {
        Stack<int> emptyStack;
        emptyStack.pop();  // 空栈弹出，抛出异常
    } catch (const std::underflow_error& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
    }

    return 0;
}
