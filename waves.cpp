#include "wfc.hpp"

Waves::Waves() : _wave() { }


void Waves::add_droplet(Tile tile)
{
    _wave.push_back(Droplet(1 << (_wave.size() + 1), tile));
}

#include <iostream>
#include <bitset>

BitRange Waves::get_expanded_entropy(BitRange bits) const
{
    //std::cout << "Start: " << std::bitset<8>(bits) << std::endl;
    return get_adjacency(bits);
    BitRange newRange = bits;
    int index = 0;
    while(bits != 0)
    {
        if(bits & 0x1)
        {
            //std::cout << "getting adjacency from index " << index << std::endl;
            BitRange adj = get_adjacency(bits);
            //std::cout << "Oring: " << std::bitset<8>(newRange) << " | " << std::bitset<8>(adj) << " == " <<
            //    std::bitset<8>(newRange | adj) << std::endl;
            newRange |= adj;
        }

        bits >>= 1;
        ++index;
    }
    return newRange;
}

BitRange Waves::get_droplet(Tile tile) const
{
    int idx = tile_index(tile);
    return (idx == -1) ? BitRange() : _wave[idx].first;
}

int Waves::tile_index(Tile tile) const
{
    for (size_t i = 0; i < _wave.size(); i++)
    {
        if(_wave[i].second == tile) return i;
    }
    return -1;
}

BitRange Waves::get_adjacency_tile(Tile tile) const
{
    int idx = tile_index(tile);
    int rv = _wave[idx].first;
    if(idx - 1 >= 0)
        rv |= _wave[idx - 1].first;
    if(idx + 1 < _wave.size())
        rv |= _wave[idx + 1].first;
    
    return rv;
}

BitRange Waves::get_adjacency(BitRange bits) const
{
    const BitRange mask = ~(UINT_MAX << _wave.size());
    return (bits | (bits >> 1) | (bits << 1)) & mask;
}

Tile Waves::get_random_from_adj(BitRange bits) const 
{
    std::vector<size_t> indices;
    
    int position = 0;
    while(bits != 0 && position < _wave.size())
    {
        if(bits & 0x1)
        {
            indices.push_back(position);
        }
        bits >>= 1;
        ++position;
    }

    size_t index = indices[rand() % indices.size()];
    //std::cout << "Choosing " << _wave[index].second << " from " << indices.size() << " choices" << std::endl;
    return _wave[index].second;
}