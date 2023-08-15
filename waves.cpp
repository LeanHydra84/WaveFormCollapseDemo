#include "wfc.hpp"

Waves::Waves() : _wave() { }


BitRange Waves::add_droplet(Tile tile)
{
    _wave.push_back(1 << (_wave.siez() + 1), tile)
}

BitRange Waves::get_expanded_entropy(BitRange bits) const
{
    BitRange newRange = 0;
    int index = bits;
    while(bits != 0)
    {
        if(bits & 0x1)
        {
            newRange |= _wave[index].first;
        }
        ++index;
    }
    return newRange;
}

BitRange Waves::get_droplet(Tile tile) const
{
    int idx = tile_index(tile);
    return (idx == -1) ? default(BitRange) : _wave[idx].first;
}

int Waves::tile_index(Tile tile) const
{
    for (size_t i = 0; i < _wave.size(); i++)
    {
        if(_wave[i].second == tile) return i;
    }
    return -1;
}

BitRange Waves::get_adjacency(Tile tile) const
{
    int idx = tile_index(tile);
    int rv = _wave[idx].first;
    if(idx - 1 >= 0)
        rv |= _wave[idx - 1].first;
    if(idx + 1 < _wave.size())
        rv |= _wave[idx + 1].first;
    
    return rv;
}

// theres definitely a faster way to do this using the index of the rightmost set bit,
// but I don't think I'll ever even call this function so why bother
Tile Waves::get_tile_from_adj(BitRange wave) const 
{
    for (size_t i = 0; i < _wave.size(); i++)
    {
        if(_wave[i].first == wave) return _wave[i].second;
    }
    return default(BitRange);
}