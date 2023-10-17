#pragma once

#include <cassert>
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <iterator>
#include <cmath>
#include <iomanip>

namespace caches {

/// @brief This cache thing uses ARC method
/// @tparam T type of stored data
/// @tparam KeyT keys for map (int by default)
template <typename T, typename KeyT = int> struct cache_t {

    const static size_t GHOST_SIZE = 3; // amount of tracked ghosts on each side

    size_t sz_; // size of real hash

    std::list<std::pair<KeyT, T>> live_cache_;
    std::list<KeyT> ghost_cache_;

    using LiveIt = typename std::list<std::pair<KeyT, T>>::iterator;
    using GhostIt = typename std::list<KeyT>::iterator;

    LiveIt base_;
    GhostIt mid_;

    std::unordered_map<KeyT, LiveIt> live_hash_;
    std::unordered_map<KeyT, GhostIt> ghost_hash_;

    cache_t (size_t size_) : sz_(size_) {

        live_cache_.resize (sz_);

        base_ = live_cache_.begin();
        std::advance (base_, sz_ / 2);

        ghost_cache_.resize (GHOST_SIZE * 2);
        mid_ = ghost_cache_.begin ();
        std::advance (mid_, GHOST_SIZE);
    }

    template <typename F> bool add_check (KeyT key, F slow_getT) {

        auto hit_live = live_hash_.find (key);

        if (hit_live == live_hash_.end ()) {

            auto hit_dead = ghost_hash_.find (key);

            if (hit_dead == ghost_hash_.end ()) {

                ghost_hash_.erase (ghost_cache_.front ());
                ghost_cache_.pop_front ();
                ghost_hash_.emplace (live_cache_.front ().first, ghost_cache_.emplace (mid_, live_cache_.front ().first));

                live_hash_.erase (live_cache_.front ().first);
                live_cache_.pop_front ();
                live_hash_.emplace (key, live_cache_.emplace (base_, key, slow_getT (key)));

                return false;
            }
        }
        return true;
    }

    void log ();
};

#include "cache_lib.cpp"

}

// {

//         std::cout << "Cache log -------------\n<";

//         for (auto i : live_cache_) std::cout << i << (&i == live_cache_.begin () ? "" : " ,");

//         std::cout << ">\n[";

//         for (auto i : live_cache_) {

//             if (i != live_cache_.begin ()) std::cout << "  ";

//             if (&i < base_) std::cout << ".";
//             else if (&i > base_) std::cout << ":";
//             else std::cout << "!";
//         }

//         std::cout << "]\nGhost cache B1: \n";

//         for (auto i : ghost_cache_) {

//             if (i == mid_) std::cout << "Ghost cache B2: \n";
//             std::cout << ">" << i << '\n';
//         }

//         std::cout << "--------------------------\n";
//     }