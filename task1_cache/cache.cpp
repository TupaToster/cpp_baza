#include "cache.hpp"

int slowGetT (int a) {

    return a;
}

int main () {

    caches::cache_t<int> lol (4);

    lol.add_check (14, slowGetT);
    lol.add_check (1, slowGetT);
    lol.add_check (15, slowGetT);
    lol.add_check (16, slowGetT);
    lol.add_check (17, slowGetT);

    lol.log ();

}