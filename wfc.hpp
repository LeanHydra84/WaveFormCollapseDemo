#ifndef _WFC_HPP_
#define _WFC_HPP_

#include <ostream>
#include <vector>

using Tile = char;
using BitRange = unsigned int;
using Droplet = std::pair<BitRange, Tile>;


// sequential-type Wave where tiles can only be bordered
// by droplets either directly above or below in the vector<>
class Waves
{
private:
    std::vector<Droplet> _wave;

public:
    Waves();

    void add_droplet(Tile tile);

    int tile_index(Tile tile) const;
    BitRange get_expanded_entropy(BitRange bits) const;
    BitRange get_droplet(Tile tile) const;
    BitRange get_adjacency(BitRange bits) const;
    BitRange get_adjacency_tile(Tile tile) const;
    Tile get_random_from_adj(BitRange wave) const;
};

#include <memory>

class Board
{
private:
    Tile* _tiles;
    size_t _bytes;
    size_t _width;
    size_t _height;

public:
    Board(size_t w, size_t h);
private:
    void propogate(BitRange* table, size_t index, BitRange entropy, const Waves& waves);
    void recur_prop(BitRange* table, size_t index, BitRange entropy, const Waves& waves, bool* hasPropogated);
    size_t get_random_minimum_entropy(const BitRange* range, size_t length);
    
    void print_bsboard(const std::unique_ptr<BitRange[]>& table);
    
    size_t xytoi(size_t x, size_t y);
    std::pair<size_t, size_t> itoxy(size_t index);

public:
    size_t width() const;
    size_t height() const;
    size_t bytes() const;
    size_t length() const;

    void reset();
    void collapse(const Waves& waves);

    friend std::ostream& operator<<(std::ostream& stream, const Board& brd);
};

#endif