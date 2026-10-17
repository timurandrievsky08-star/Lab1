#include <cassert>
#include <iostream>

int countAtLeast(int value, int threshold) {
    int count = 0;
    if (value >= threshold) {
        ++count;
    }
    return count;
}

int main() {
    std::cout << "Failing test (value=50, threshold=50): expected=1, actual="
              << countAtLeast(50, 50) << '\n';
    std::cout << "Below threshold (value=49, threshold=50): expected=0, actual="
              << countAtLeast(49, 50) << '\n';
    std::cout << "Above threshold (value=51, threshold=50): expected=1, actual="
              << countAtLeast(51, 50) << '\n';

    assert(countAtLeast(50, 50) == 1);
    assert(countAtLeast(49, 50) == 0);
    assert(countAtLeast(51, 50) == 1);

    std::cout << "All regression tests passed!\n";
    return 0;
}