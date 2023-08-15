#include <iostream>
#include <memory>
#include <queue>
#include "wfc.hpp"

// Board
#define BOARD_DEFAULT_VALUE (int)'X'

Board::Board(size_t w, size_t h) : _width(w), _height(h)
{
    _bytes = (w * h) * sizeof(Tile);
    _tiles = new Tile[w * h];
    reset();
}

size_t Board::height() const { return _height; }
size_t Board::width() const { return _width; }
size_t Board::bytes() const { return _bytes; }
size_t Board::length() const { return _width * _height; }

void Board::reset()
{
    // this only works if sizeof(Tile_t) == 1 but this is a quick test so whatever
    memset(_tiles, BOARD_DEFAULT_VALUE, bytes());
}

std::ostream& operator<<(std::ostream& stream, const Board& brd)
{
    for (size_t i = 0; i < brd._height; i++)
    {
        for (size_t j = 0; j < brd._width; j++)
        {
            stream << brd._tiles[i * brd._width + j] << " ";
        }
        stream << '\n';
    }
    
    return stream;
}

// COLLAPSE FUNCTION
static size_t random_position(size_t length)
{
    return rand() % length;
}

size_t Board::xytoi(size_t x, size_t y)
{
    return y * _width + x;
}


std::pair<size_t, size_t> Board::itoxy(size_t index)
{
    size_t x = index % _width;
    size_t y = index / _width;
    return std::make_pair(x, y);
}

void Board::recur_prop(BitRange* table, size_t index, BitRange entropy, const Waves& waves, bool* hasPropogated)
{
    if(hasPropogated[index]) return;
    hasPropogated[index] = true;

    if(_tiles[index] != BOARD_DEFAULT_VALUE) return;
    if(table[index] == entropy) return;
    BitRange expandedEntropy = waves.get_expanded_entropy(entropy);

    //std::cout << "Propogated " << table[index] << " & " << entropy;
    table[index] &= expandedEntropy;
    //std::cout << " == " << table[index] << std::endl;

    std::pair<size_t, size_t> coord = itoxy(index);
    //std::cout << coord.first << ", " << coord.second << " Expanded: " << expandedEntropy << std::endl;
    if((int)coord.first - 1 >= 0) // open square left
        recur_prop(table, index - 1, expandedEntropy, waves, hasPropogated);
    if(coord.first + 1 < _width) // open square right
        recur_prop(table, index + 1, expandedEntropy, waves, hasPropogated);
    if((int)coord.second - 1 >= 0) // open square above
        recur_prop(table, index - _width, expandedEntropy, waves, hasPropogated);
    if(coord.second + 1 < _height) // open square below
        recur_prop(table, index + _width, expandedEntropy, waves, hasPropogated);
}

void Board::propogate(BitRange* table, size_t startIndex, BitRange newEntropy, const Waves& waves)
{
    std::unique_ptr<bool[]> hasPropogated = std::unique_ptr<bool[]>(new bool[length()]());
    recur_prop(table, startIndex, newEntropy, waves, hasPropogated.get());
}

static int bits_set(BitRange n)
{
    int count = 0;
    while(n != 0)
    {
        if(n & 0x1) ++count;
        n = n >> 1;
    }

    return count;
}

size_t Board::get_random_minimum_entropy(const BitRange* range, size_t length)
{
    std::vector<size_t> positions;
    int minbitsset = INT_MAX;

    for(size_t i = 0; i < length; i++)
    {
        if(_tiles[i] != BOARD_DEFAULT_VALUE) continue;
        int bits = bits_set(range[i]);
        if(bits < minbitsset)
        {
            minbitsset = bits;
            positions.clear();
            positions.push_back(i);
            continue;
        }

        if(bits == minbitsset)
        {
            positions.push_back(i);
        }
    }

    return positions[rand() % positions.size()];
}

void Board::print_bsboard(const std::unique_ptr<BitRange[]>& table)
{

    std::cout << std::hex << std::uppercase;
    for(int i = 0; i < _height; i++)
    {
        for(int j = 0; j < _width; j++)
        {
            std::cout << table[xytoi(i, j)] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::dec;
    std::cout << "\n\n";
}

void Board::collapse(const Waves& waves)
{
    std::unique_ptr<BitRange[]> table = std::unique_ptr<BitRange[]>(new BitRange[length()]);
    memset(table.get(), 0xFF, sizeof(BitRange) * length());

    size_t uncollapsed = length();
    
    while(uncollapsed > 0)
    {
        size_t position = get_random_minimum_entropy(table.get(), length());
        Tile ctile = waves.get_random_from_adj(table[position]);

        propogate(table.get(), position, waves.get_droplet(ctile), waves);

        //print_bsboard(table);

        _tiles[position] = ctile;
        --uncollapsed;
    }



}