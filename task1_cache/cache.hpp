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
    size_t b1_min_sz_; // minimal size of b1 cache (1 by default)
    size_t b2_min_sz_; // min size of b2 (1 by default)
    // Note that neither of them should be more than sz_ / 2 for cache to work

    std::list<std::pair<KeyT, T>> live_cache_;
    std::list<KeyT> ghost_cache_;

    using LiveIt = typename std::list<std::pair<KeyT, T>>::iterator;
    using GhostIt = typename std::list<KeyT>::iterator;

    LiveIt base_;
    GhostIt mid_;

    std::unordered_map<KeyT, LiveIt> live_hash_;
    std::unordered_map<KeyT, GhostIt> ghost_hash_;

    cache_t (size_t size_, size_t b1_min_size_ = 1, size_t b2_min_size_ = 2);

    template <typename F> bool add_check (KeyT key, F slow_getT);

    template <typename F>
    void killL1 (KeyT key, F slow_getT);

    template <typename F>
    void killL2 (KeyT key, F slow_getT);

    template <typename F>
    void revive (GhostIt hit, F slow_getT);

    void killL2_blank ();

    void log ();
};}

//This kinda allows to write funcs in another file so....
//As a famous quote says "I'm not gay, but 20 bucks is 20 bucks"
#include "cache_lib.cpp"
