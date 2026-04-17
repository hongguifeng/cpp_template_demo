// 20_crtp.cpp
// 演示：CRTP（Curiously Recurring Template Pattern，奇异递归模板模式）

#include <iostream>
#include <string>

// ============================================================
// 1. CRTP 基本概念
// ============================================================
// CRTP 是一种设计模式：基类是一个模板，派生类把自己作为模板参数传给基类
// template <typename Derived>
// class Base { ... };
// class MyClass : public Base<MyClass> { ... };
//
// 核心思想：基类在编译期就知道派生类的类型，实现"静态多态"

// ============================================================
// 2. CRTP 实现静态多态（无虚函数的多态）
// ============================================================

// 基类模板：Derived 是派生类自己
template <typename Derived>
class Shape {
public:
    // 基类通过 static_cast 调用派生类的方法
    // 注意：这里没有 virtual！
    double area() const {
        return static_cast<const Derived*>(this)->areaImpl();
    }

    double perimeter() const {
        return static_cast<const Derived*>(this)->perimeterImpl();
    }

    void describe() const {
        std::cout << static_cast<const Derived*>(this)->name()
                  << ": 面积=" << area()
                  << ", 周长=" << perimeter() << std::endl;
    }
};

// 派生类：Circle
class Circle : public Shape<Circle> {
    double radius;
public:
    Circle(double r) : radius(r) {}

    // 提供 CRTP 需要的实现
    double areaImpl() const { return 3.14159265 * radius * radius; }
    double perimeterImpl() const { return 2 * 3.14159265 * radius; }
    std::string name() const { return "Circle(r=" + std::to_string(radius) + ")"; }
};

// 派生类：Rectangle
class Rectangle : public Shape<Rectangle> {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double areaImpl() const { return width * height; }
    double perimeterImpl() const { return 2 * (width + height); }
    std::string name() const {
        return "Rectangle(" + std::to_string(width) + "x" + std::to_string(height) + ")";
    }
};

// 可以写模板函数，对任何 Shape<Derived> 工作
template <typename Derived>
void printShapeInfo(const Shape<Derived>& shape) {
    shape.describe();
}

// ============================================================
// 3. CRTP 实现"混入"功能（Mixin）：自动添加运算符
// ============================================================

// 混入基类：自动从 < 推导出 >, <=, >=
template <typename Derived>
class Comparable {
public:
    // 只需要派生类实现 operator<，其余自动推导
    bool operator>(const Derived& other) const {
        return other < static_cast<const Derived&>(*this);
    }
    bool operator<=(const Derived& other) const {
        return !(static_cast<const Derived&>(*this) > other);
    }
    bool operator>=(const Derived& other) const {
        return !(static_cast<const Derived&>(*this) < other);
    }
    bool operator==(const Derived& other) const {
        return !(static_cast<const Derived&>(*this) < other) &&
               !(other < static_cast<const Derived&>(*this));
    }
    bool operator!=(const Derived& other) const {
        return !(static_cast<const Derived&>(*this) == other);
    }
};

class Temperature : public Comparable<Temperature> {
    double value;
public:
    Temperature(double v) : value(v) {}
    double getValue() const { return value; }

    // 只需实现 operator<
    bool operator<(const Temperature& other) const {
        return value < other.value;
    }
};

// ============================================================
// 4. CRTP 实现对象计数
// ============================================================

template <typename Derived>
class ObjectCounter {
    static int count;
protected:
    ObjectCounter() { ++count; }
    ObjectCounter(const ObjectCounter&) { ++count; }
    ~ObjectCounter() { --count; }
public:
    static int getCount() { return count; }
};

template <typename Derived>
int ObjectCounter<Derived>::count = 0;

class Dog : public ObjectCounter<Dog> {
    std::string name;
public:
    Dog(const std::string& n) : name(n) {}
    std::string getName() const { return name; }
};

class Cat : public ObjectCounter<Cat> {
    std::string name;
public:
    Cat(const std::string& n) : name(n) {}
};

// ============================================================
// 5. CRTP 实现链式调用（Fluent Interface）
// ============================================================

template <typename Derived>
class Builder {
public:
    Derived& set(const std::string& key, const std::string& value) {
        std::cout << "  设置 " << key << " = " << value << std::endl;
        return static_cast<Derived&>(*this);
    }
};

class HttpRequestBuilder : public Builder<HttpRequestBuilder> {
    std::string method;
    std::string url;
public:
    HttpRequestBuilder& setMethod(const std::string& m) {
        method = m;
        set("method", m);
        return *this;
    }

    HttpRequestBuilder& setUrl(const std::string& u) {
        url = u;
        set("url", u);
        return *this;
    }

    void build() {
        std::cout << "  构建请求: " << method << " " << url << std::endl;
    }
};

int main() {
    // ============================================================
    // 2. 静态多态
    // ============================================================
    std::cout << "=== CRTP 静态多态 ===" << std::endl;

    Circle c(5.0);
    Rectangle r(3.0, 4.0);

    printShapeInfo(c);
    printShapeInfo(r);

    // 注意：这里没有虚函数表（vtable）开销！
    // 每个函数调用在编译期就已经确定

    // ============================================================
    // 3. Comparable 混入
    // ============================================================
    std::cout << "\n=== CRTP Comparable 混入 ===" << std::endl;

    Temperature t1(36.5);
    Temperature t2(37.2);
    Temperature t3(36.5);

    std::cout << "36.5 < 37.2:  " << (t1 < t2) << std::endl;
    std::cout << "36.5 > 37.2:  " << (t1 > t2) << std::endl;
    std::cout << "36.5 <= 37.2: " << (t1 <= t2) << std::endl;
    std::cout << "36.5 >= 37.2: " << (t1 >= t2) << std::endl;
    std::cout << "36.5 == 36.5: " << (t1 == t3) << std::endl;
    std::cout << "36.5 != 37.2: " << (t1 != t2) << std::endl;

    // 只实现了 operator<，其余 5 个运算符自动获得！

    // ============================================================
    // 4. 对象计数
    // ============================================================
    std::cout << "\n=== CRTP 对象计数 ===" << std::endl;

    {
        Dog d1("旺财"), d2("小白");
        Cat c1("咪咪");

        std::cout << "Dog 数量: " << Dog::getCount() << std::endl;
        std::cout << "Cat 数量: " << Cat::getCount() << std::endl;

        {
            Dog d3("大黄");
            std::cout << "添加一只 Dog 后: " << Dog::getCount() << std::endl;
        }

        std::cout << "Dog 离开作用域后: " << Dog::getCount() << std::endl;
    }
    std::cout << "全部离开作用域: Dog=" << Dog::getCount()
              << ", Cat=" << Cat::getCount() << std::endl;

    // ============================================================
    // 5. 链式调用
    // ============================================================
    std::cout << "\n=== CRTP 链式调用 ===" << std::endl;

    HttpRequestBuilder()
        .setMethod("GET")
        .setUrl("https://example.com/api")
        .set("header", "Content-Type: json")
        .build();

    return 0;
}
