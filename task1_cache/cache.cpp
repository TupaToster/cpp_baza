#include "cache.hpp"

int slowGetT (int a) {

    return a;
}

int main () {

    // breaks when size is 4 (fix neededlive_cache_.emplace_front ();)
    caches::cache_t<int> lol (6);

    for (;;) {

        std::cout << "Insert number to add to cache: ";
        int key = 0;
        std::cin >> key;

        lol.add_check (key, slowGetT);

        lol.log ();
    }

}