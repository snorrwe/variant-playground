#include <array>
#include <stdio.h>
#include <variant>
#include <vector>

struct Value;
using Variant = std::variant<int, float, std::vector<Value>>;
struct Value : Variant {
    using Variant::Variant;
};

template <typename T>
Value get(T i) { return i; }
Variant get()
{
    static int counter = 0;
    if ((++counter & 1) == 0) {
        return 420;
    } else {
        return std::vector { get(counter), get(counter * 1.5f) };
    }
}
Variant get(Value v) { return v; }

struct Visitor {
    template <typename... Args>
    void operator()(int i, Args&&... Values)
    {
        printf("Int %i\n", i);
        (*this)(Values...);
    }

    template <typename... Args>
    void operator()(float i, Args&&... Values)
    {
        printf("Float %f\n", i);
        (*this)(Values...);
    }

    template <typename... Args>
    void operator()(std::vector<Value> const& v, Args&&... Values)
    {
        puts("Vector!");
        for (auto& i : v) {
            printf("    ");
            std::visit(*this, static_cast<Variant>(i));
        }
        (*this)(Values...);
    }

    void operator()() {}
};

int main()
{
    std::visit(Visitor {}, get(), get(), get(), get(get(5)), get(get(3.14f)));
}

