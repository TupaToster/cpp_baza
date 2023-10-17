#pragma once
#include "cache.hpp"


template <typename T, typename KeyT>
void caches::cache_t<T, KeyT>::log () {

    {

        std::cout << "Cache log -------------\n<";

        int max_width = 0;

        for (auto i : live_cache_) {

            max_width = std::max<int> (max_width, std::log10<int> (i.second) + 1);
        }

        for (auto i = live_cache_.begin (); i != live_cache_.end (); i++)
            std::cout << (i == live_cache_.begin () ? "" : ", ") << std::setw (max_width) << i->second;

        std::cout << ">\n[";

        for (auto i = live_cache_.begin (); i != base_; i++) {

            if (i != live_cache_.begin ()) std::cout << "  ";
            std::cout << std::setw (max_width) << ".";
        }

        for (auto i = base_; i != live_cache_.end (); i++) {

            std::cout << std::setw (max_width + 2) << ":";
        }

        std::cout << "]\nGhost cache B1: \n";

        for (auto i = std::reverse_iterator(mid_); i != std::reverse_iterator (ghost_cache_.begin ()); i++) {

            std::cout << ">" << *i << '\n';
        }

        std::cout << "Ghost cache B2: \n";

        for (auto i = mid_; i != ghost_cache_.end (); i++) {

            std::cout << ">" << *i << '\n';
        }

        std::cout << "------------------------\n";
    }
}
