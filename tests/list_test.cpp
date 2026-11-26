#include "../MyList.hpp"
#include <cassert>
#include <random>
#include <vector>

static void check(MyList<int> list, const std::vector<int> &values) {
    assert(list.size() == static_cast<int>(values.size()));
    for (int v : values) {
        assert(list.front() == v);
        list.pop_front();
    }
    assert(list.empty());
}

int main() {
    std::mt19937 rng(1363);
    MyList<int> list;
    std::vector<int> values;
    for (int step = 0; step < 20000; ++step) {
        int op = rng() % 6;
        int index = rng() % (values.size() + 1);
        int value = rng();
        if (op < 2) {
            list.insert(index, value);
            values.insert(values.begin() + index, value);
        } else if (op == 2 && !values.empty()) {
            index %= values.size();
            list.erase(index);
            values.erase(values.begin() + index);
        } else if (op == 3) {
            auto suffix = list.cut(index);
            check(list, std::vector<int>(values.begin(), values.begin() + index));
            check(suffix, std::vector<int>(values.begin() + index, values.end()));
            list.link(suffix);
        } else if (op == 4) {
            MyList<int> moved(std::move(list));
            assert(list.empty());
            list = std::move(moved);
        } else if (values.size() < 100) {
            list.link(list);
            auto copy = values;
            values.insert(values.end(), copy.begin(), copy.end());
        }
        check(list, values);
        if (!values.empty()) assert(list.back() == values.back());
    }
    list.clear();
    assert(list.empty());
}
