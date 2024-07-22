#include <iostream>
#include <vector>
#include <math.h>
#include <iomanip>
#include <cstring> // for std::memcpy
using namespace std;
#define DBG 1
#define DRAM_SIZE (64 * 1024 * 1024)
#define CACHE_SIZE (64 * 1024)

int line_size = 64; // Line size used for different cache line sizes
int num_lines = CACHE_SIZE / line_size; // Calculating the number of lines in the cache


enum cacheResType 
{
    MISS = 0,
    HIT = 1
};
struct line // A struct for each line containing the necessary values for a line (valid bit, data and tag) 
{
    bool valid=0;
    unsigned int Tag=0;
    unsigned int data=0;
};

vector<line> fully_associative_cache; // An FA cache
vector<line> Direct_Mapped_cache_16(CACHE_SIZE / 16); // 16 byte DM cache
vector<line> Direct_Mapped_cache_32(CACHE_SIZE / 32); // 32 byte DM cache 
vector<line> Direct_Mapped_cache_64(CACHE_SIZE / 64); // 64 byte DM cache
vector<line> Direct_Mapped_cache_128(CACHE_SIZE / 128); // 128 byte DM cache

// unordered_map<unsigned int, char[16]> fully_associative_cache;
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
    static unsigned int addr = 0;
    return (addr++) % (DRAM_SIZE);
  
}
unsigned int memGen2()
{
    static unsigned int addr = 0;
    return rand_() % (24 * 1024);

}
unsigned int memGen3()
{
    return rand_() % (DRAM_SIZE);
}
unsigned int memGen4()
{
    static unsigned int addr = 0;
    return (addr++) % (4 * 1024);
  
}
unsigned int memGen5()
{
    static unsigned int addr = 0;
    return (addr++) % (1024 * 64);

}
unsigned int memGen6()
{
    static unsigned int addr = 0;
    return (addr += 32) % (64 * 4 * 1024);

}
// Direct Mapped Cache Simulator
cacheResType cacheSimDM(unsigned int addr)
{
    cacheResType status = MISS; // Initializing the access state as Miss
    int offset_bits = log2(line_size); // Caculating the number of offset bits to be used in index and tag calculation

    int index_bits = log2(num_lines); // Calculating the number of index bits to be used in index and tag calculation

    int index = (addr >> offset_bits) & ((1 << index_bits) - 1); // Calculating the index using bit masking

    int tag = (addr >> (offset_bits + index_bits)); // Calcuting the tag using shifting

   // cout << endl << "Tag = " << tag << "  Index = " << index <<" Offset = "<<offset_bits<< " bits" << endl;

    switch (line_size) // Different cases for different line sizes
    {
        case 16:
            // If the tag is found in the index given and the vaild bit is one then we return a hit
            if (Direct_Mapped_cache_16[index].valid == 1 && Direct_Mapped_cache_16[index].Tag == tag) 
            {
                return HIT;
            }
            // Otherwise the tag is copied into the index given and valid bit becomes true    
            else  
            {
                Direct_Mapped_cache_16[index].Tag = tag;
                Direct_Mapped_cache_16[index].valid = true;

                return MISS;
            }
            break;
        case 32:
            // If the tag is found in the index given and the vaild bit is one then we return a hit
            if (Direct_Mapped_cache_32[index].valid == 1 && Direct_Mapped_cache_32[index].Tag == tag)
            {
                return HIT;
            }
                // Otherwise the tag is copied into the index given and valid bit becomes true
            else
            {
                Direct_Mapped_cache_32[index].Tag = tag;
                Direct_Mapped_cache_32[index].valid = true;
                return MISS;
            }
            break;
        case 64:
            // If the tag is found in the index given and the vaild bit is one then we return a hit
            if (Direct_Mapped_cache_64[index].valid == 1 && Direct_Mapped_cache_64[index].Tag == tag)
            {
                return HIT;
            }
                // Otherwise the tag is copied into the index given and valid bit becomes true
            else
            {
                Direct_Mapped_cache_64[index].Tag = tag;
                Direct_Mapped_cache_64[index].valid = true;
                return MISS;
            }
            break;
        case 128:
            // If the tag is found in the index given and the vaild bit is one then we return a hit
            if (Direct_Mapped_cache_128[index].valid == 1 && Direct_Mapped_cache_128[index].Tag == tag)
            {
                return HIT;
            }
                // Otherwise the tag is copied into the index given and valid bit becomes true
            else
            {
                Direct_Mapped_cache_128[index].Tag = tag;
                Direct_Mapped_cache_128[index].valid = true;
                return MISS;
            }
    }
}


cacheResType cacheSimFA(unsigned int addr)
{

    cacheResType status = MISS; // Initializing the access state as Miss
    int n = log2(line_size); 
    unsigned int addr_tag = addr >> n; //shift right to ignore the byte select bits

    for (int i=0;i<fully_associative_cache.size();i++)
    {
        if((fully_associative_cache[i].Tag) == addr_tag)   //if tag matches 
        {
            if(fully_associative_cache[i].valid)   //if valid bit is 1
                status = HIT;

            if(status==MISS)  //cold start
            {
                line x;
                x.data=0;
                x.Tag=addr_tag;
                x.valid=1;
                fully_associative_cache.push_back(x);


            }
            return status;   //will return miss if its a cold start and hit otherwise
        }
    }

    if(fully_associative_cache.size()>=num_lines)  //deletes a random line when cache is full and no hit
    {
                line x;
                x.data=0;
                x.Tag=addr_tag;
                x.valid=1;
                int y = (rand_()%num_lines);
             //   cout << endl << "Replacing Memory Line starting at " << fully_associative_cache[y].Tag << n << endl;
                fully_associative_cache[y] = x;
                return status;


    }


     line x;
     x.data=0;
     x.Tag=addr_tag;
     x.valid=1;
     fully_associative_cache.push_back(x);
     return status;

}
char *msg[2] = {"Miss", "Hit"};
int NO_OF_Iterations =1000000 ;// Change to 1,000,000 (was originally 100)
int main(int argc, char* argv[])
{
    int cachesel = std::atoi(argv[1]);
    line_size= std::atoi(argv[2]);
    int memsel = std::atoi(argv[3]);
    NO_OF_Iterations = std::atoi(argv[4]);

    num_lines = CACHE_SIZE / line_size;



    unsigned int hit = 0;
    unsigned int hit2=0;
    cacheResType r;
    unsigned int addr;

    // choose the cache to simulate
    if (cachesel == 1)
    {
        // choose the DM cache to simulate
       // unsigned int arr[6] = {0x40,0x100040,0x41,0x100041,0x100042,0x42};
        cout << "Direct Mapped Cache Simulator\n";
        for (int inst = 0; inst < NO_OF_Iterations; inst++)
        {
            // choose the memory generating function
            switch (memsel)
            {
            case 1:
                addr = memGen1();
                break;

            case 2:
                addr = memGen2();
                break;

            case 3:
                addr = memGen3();
                break;

            case 4:
                addr = memGen4();
                break;

            case 5:
                addr = memGen5();
                break;
            case 6:
                addr = memGen6();
                break;
            default:
                addr = memGen1();

            }
           // addr = arr[inst];
            r = cacheSimDM(addr);
            if (r == HIT) hit++;
            cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r]
                << ")\n";
        }
        cout << "Hit ratio = " << dec << fixed << setprecision(6) << (100.0 * hit / NO_OF_Iterations) << endl;


    }

    // Choosing FA cache
    else
    {

        cout << "-------------------------------\n" << endl;
        cout << " Fully Associative Cache Simulator\n";
        for (int inst = 0; inst < NO_OF_Iterations; inst++)
        {
            // choose the memory generating function
            switch (memsel)
            {
            case 1:
                addr = memGen1();
                break;

            case 2:
                addr = memGen2();
                break;

            case 3:
                addr = memGen3();
                break;

            case 4:
                addr = memGen4();
                break;

            case 5:
                addr = memGen5();
                break;
            case 6:
                addr = memGen6();
                break;
            default:
                addr = memGen1();

            }
            r = cacheSimFA(addr);
            if (r == HIT)
                hit2++;
            cout << "0x" << setfill('0') << setw(8) << hex << addr << " (" << msg[r]
                << ")\n";
        }
        cout << "Hit ratio = " << dec << fixed << setprecision(6) << (100.0 * hit2 / NO_OF_Iterations) << endl;

    }

}
