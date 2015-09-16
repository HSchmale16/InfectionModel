#ifndef GLOBAL_H_INC
#define GLOBAL_H_INC

#include <array>
#include <chrono>
#include <random>
extern "C"{
#include <iniparser.h>
}

#define SCRATCH_BUFF_LEN 100

static char P_BUFF[SCRATCH_BUFF_LEN];

// define seperator character
#define SEP '\t'
// Screen Pixel Coordinate Calculation
#define SCR_X(x) (((x) / MAP_SIZE) * SCREEN_MIN)
#define SCR_Y(y) (((y) / MAP_SIZE) * SCREEN_MIN)

/* This is a magic macro that generates the section for ini config values
 * that have an id attached to it's section to it, and places in the global
 * variable, `P_BUFF`.
 * \param id value id
 * \param val the config key name
 * \param key ini file section 
 */
#define INI_SEC_ID_GEN(id,val,key) \
        snprintf(P_BUFF, SCRATCH_BUFF_LEN, #key"%d:%s", id, val);

#define INI_KEY_ID_GEN(id,val,key) \
        snprintf(P_BUFF, SCRATCH_BUFF_LEN, "%s:%s%d", key, val, id);
// Make getting the map size a simple macro
#define MAP_SIZE iniparser_getint(gConfig, "world:size", 100) 
#define MAP_SIZE_2 (MAP_SIZE / 2)

const double DIR_ARRAY[][2] = {
    {0, 1}, // north
    {sqrt(2), sqrt(2)}, // north east
    {1, 0}, // east
    {sqrt(2), -sqrt(2)},
    {0,-1}, // south
    {-sqrt(2), -sqrt(2)},
    {-1, 0},  // west
    {-sqrt(2), sqrt(2)},
};

const int NUM_DIRS = 9;

// other globals
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator(seed);
std::array<int, NUM_DIRS> dir_probs;
std::discrete_distribution<int>* dir_distrib;

// the config file
dictionary* gConfig = NULL;

// magic globals that need to manipulated in main, but should be treated as
// constants everywhere else
int SCREEN_WIDTH = 760;
int SCREEN_HEIGHT = 760;
int SCREEN_MIN;



#endif // GLOBAL_H_INC
