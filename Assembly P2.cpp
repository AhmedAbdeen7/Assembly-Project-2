#include <iostream>
#include <set>
#include <map>
#include <math.h>
#include <iomanip>
#include <cstring> // for std::memcpy
using namespace std;
#define DBG 1
#define DRAM_SIZE (64*1024*1024)
#define CACHE_SIZE (64*1024)
#define LINE_SIZE (32)
#define MAX_LINES (CACHE_SIZE/LINE_SIZE)
enum cacheResType {MISS=0, HIT=1};
//set <int> fully_associative_cashe;
map <unsigned int, char[32]> fully_associative_cache;
/* The following implements a random number generator */
unsigned int m_w = 0xABABAB55; /* must not be zero, nor 0x464fffff */
unsigned int m_z = 0x05080902; /* must not be zero, nor 0x9068ffff */
unsigned int rand_()
{
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w; /* 32-bit result */
}
unsigned int memGen1()
{
    static unsigned int addr=0;
    return (addr++)%(DRAM_SIZE);
}
unsigned int memGen2()
{
    static unsigned int addr=0;
    return rand_()%(24*1024);
}
unsigned int memGen3()
{
    return rand_()%(DRAM_SIZE);
}
unsigned int memGen4()
{
    static unsigned int addr=0;
    return (addr++)%(4*1024);
}
unsigned int memGen5()
{
    static unsigned int addr=0;
    return (addr++)%(1024*64);
}
unsigned int memGen6()
{
    static unsigned int addr=0;
    return (addr+=32)%(64*4*1024);
}
// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
// This function accepts the memory address for the memory transaction and
// returns whether it caused a cache miss or a cache hit
// The current implementation assumes there is no cache; so, every transaction is a miss
    return MISS;
}
// Fully Associative Cache Simulator
cacheResType cacheSimFA(unsigned int addr)
{
// This function accepts the memory address for the read and
// returns whether it caused a cache miss or a cache hit
// The current implementation assumes there is no cache; so, every transaction is a miss
    cacheResType status = MISS;  
    int n = log2(LINE_SIZE);   
    unsigned int addr_tag = addr >> n; //shift right to ignore the byte select bits
    map <unsigned int, char[32]>::iterator itr;
    for (itr = fully_associative_cache.begin(); itr != fully_associative_cache.end(); itr++)
     {
        if((itr->first>>1) == addr_tag)   //if tag matches 
        {
            if(itr->first & 0x1)   //if valid bit is 1
            status = HIT;

            if(status==MISS)  //cold start
            {
                char value[16] = {0};
                unsigned int key = (addr_tag<<1) + 0x1;
                std::memcpy(fully_associative_cache[key], value, sizeof(value));

            }
            return status;   //will return miss if its a cold start and hit otherwise
        }
    }

    if(fully_associative_cache.size()>=MAX_LINES)  //deletes a random line when cache is full and no hit
    {
    int random_index = rand_() % fully_associative_cache.size();

    auto it = fully_associative_cache.begin();
    std::advance(it, random_index);

    // Display the element to be deleted
   // std::cout << "Deleting key: " << it->first << std::endl;

    // Erase the element
    fully_associative_cache.erase(it);
    }


    char value[16] = {0};
    unsigned int key = (addr_tag<<1) + 0x1;
    std::memcpy(fully_associative_cache[key], value, sizeof(value));
    return status;
}
char *msg[2] = {"Miss","Hit"};
#define NO_OF_Iterations 100// Change to 1,000,000 (was originally 100)
int main()
{
    //set <int> fully_associative_cashe;
    unsigned int hit = 0;
    cacheResType r;
    unsigned int addr;
    // cout << "Direct Mapped Cache Simulator\n";
    // for(int inst=0;inst<NO_OF_Iterations;inst++)
    // {
    //     addr = memGen2();
    //     r = cacheSimDM(addr);
    //     if(r == HIT) hit++;
    //     cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[r]
    //          <<")\n";
    // }
    // cout << "Hit ratio = " << (100*hit/NO_OF_Iterations)<< endl;
    // cout <<"-------------------------------\n" << endl;
    cout << " Fully Associative Cache Simulator\n";
    for(int inst=0;inst<NO_OF_Iterations;inst++)
    {
        addr = memGen1();
        r = cacheSimFA(addr);
        if(r == HIT) hit++;
        cout <<"0x" << setfill('0') << setw(8) << hex << addr <<" ("<< msg[r]
             <<")\n";
    }
    cout << "Hit ratio = " <<dec<<hit<< endl;

}