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


template <typename T, typename KeyT>
template <typename F>
bool caches::cache_t<T, KeyT>::add_check (KeyT key, F slow_getT) {

    auto hit_live = live_hash_.find (key);

    if (hit_live == live_hash_.end ()) {

        auto hit_dead = ghost_hash_.find (key);

        if (hit_dead == ghost_hash_.end ()) {

            killL1 (key, slow_getT);

            return false;
        }
        else {

            revive (hit_dead->second, slow_getT);

            return false;
        }
    }
    else {

        if (hit_live->second == base_) return true;

        if (std::distance (hit_live->second, base_) < 0) {

            live_cache_.splice (base_, live_cache_, hit_live->second);
            --base_;
        }
        else {

            live_cache_.splice (base_, live_cache_, hit_live->second);
            --base_;

            killL2_blank ();
        }
    }
    return true;
}

template <typename T, typename KeyT>
void caches::cache_t<T, KeyT>::killL2_blank () {

    ghost_hash_.erase (ghost_cache_.back ());
    ghost_cache_.pop_back ();
    ghost_hash_.emplace (live_cache_.back ().first, ghost_cache_.emplace (mid_, live_cache_.back ().first));
    mid_--;

    live_hash_.erase (live_cache_.back ().first);
    live_cache_.pop_back ();
    live_cache_.emplace_front ();
}

template <typename T, typename KeyT>
template <typename F>
void caches::cache_t<T, KeyT>::revive (GhostIt hit, F slow_getT) {

    if (hit == mid_) {

        killL1 (*hit, slow_getT);
        --base_;

        mid_++;
        ghost_cache_.erase (hit);
        ghost_cache_.emplace_back ();
    }
    else if (std::distance (hit, mid_) < 0) { //hit is right from mid!!!!!

        killL1 (*hit, slow_getT);
        --base_;

        ghost_cache_.erase (hit);
        ghost_cache_.emplace_back ();
    }
    else {

        killL2_blank ();

        killL2 (*hit, slow_getT);

        ghost_cache_.erase (hit);
        ghost_cache_.emplace_front ();
    }
}

template <typename T, typename KeyT>
template <typename F>
void caches::cache_t<T, KeyT>::killL1 (KeyT key, F slow_getT) {

    ghost_hash_.erase (ghost_cache_.front ());
    ghost_cache_.pop_front ();
    ghost_hash_.emplace (live_cache_.front ().first, ghost_cache_.emplace (mid_, live_cache_.front ().first));

    live_hash_.erase (live_cache_.front ().first);
    live_cache_.pop_front ();
    live_hash_.emplace (key, live_cache_.emplace (base_, key, slow_getT (key)));
}

template <typename T, typename KeyT>
template <typename F>
void caches::cache_t<T, KeyT>::killL2 (KeyT key, F slow_getT) {

    ghost_hash_.erase (ghost_cache_.back ());
    ghost_cache_.pop_back ();
    ghost_hash_.emplace (live_cache_.back ().first, ghost_cache_.emplace (mid_, live_cache_.back ().first));
    mid_--;

    live_hash_.erase (live_cache_.back ().first);
    live_cache_.pop_back ();
    live_hash_.emplace (key, live_cache_.emplace (base_, key, slow_getT (key)));
    base_--;
}

template <typename T, typename KeyT>
caches::cache_t<T, KeyT>::cache_t (size_t size_, size_t b1_min_size_ = 1, size_t b2_min_size_ = 1) : sz_ (size_) {

    assert (size_ >= 2);

    if (b1_min_size_ < 1) b1_min_sz_ = 1;
    else if (b1_min_size_ >= size_t (sz/2)) b1_min_size = sz / 2 - 1;

    if (b2_min_size_ < 1) b2_min_sz_ = 1;
    else if (b2_min_size_ >= size_t (sz / 2)) b2_min_sz_ = sz / 2 - 1;


    live_cache_.resize (sz_);

    base_ = live_cache_.begin();
    std::advance (base_, sz_ / 2);

    ghost_cache_.resize (GHOST_SIZE * 2);
    mid_ = ghost_cache_.begin ();
    std::advance (mid_, GHOST_SIZE);
}