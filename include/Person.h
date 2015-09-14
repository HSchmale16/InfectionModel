#ifndef PERSON_H_INC
#define PERSON_H_INC

/**\file Person.h
 * \author Henry Jacob Schmale
 * \date 09/14/2015
 */

#include <cmath>
#include <cstdio>
#include "global.h"

#define SCRATCH_BUFF_LEN 100

static char P_BUFF[SCRATCH_BUFF_LEN];

#define SEP '\t'
#define P_SEARCH_BUFF(id,val) \
        snprintf(P_BUFF, SCRATCH_BUFF_LEN, "P%d:%s", id, val);

#define MAP_SIZE_2 int(iniparser_getdouble(gConfig, "world:size", 100) / 2)

const double DIR_ARRAY[][2] = {
    {0,0},
    {0, 1}, // north
    {sqrt(2), sqrt(2)}, // north east
    {1, 0}, // east
    {sqrt(2), -sqrt(2)},
    {0,-1}, // south
    {-sqrt(2), -sqrt(2)},
    {-1,0},  // west
    {-sqrt(2), sqrt(2)},
};

const int NUM_DIRS = 9;

class Person {
private:
protected:
    static size_t nextid;
    size_t  m_id;
    double  m_x;
    double  m_y;
    bool    m_infected;
public:
    Person()
        :m_id(nextid++){
            P_SEARCH_BUFF(m_id, "x");
            m_x = iniparser_getdouble(gConfig, P_BUFF, rand() % MAP_SIZE_2);
            P_SEARCH_BUFF(m_id, "y");
            m_y = iniparser_getdouble(gConfig, P_BUFF, rand() % MAP_SIZE_2); 
            P_SEARCH_BUFF(m_id, "v");
            m_infected = iniparser_getboolean(gConfig, P_BUFF, false);
    }

    double getDistance(Person& p) {
        return sqrt(pow(this->m_x - p.m_x, 2) +
                        pow(this->m_y - p.m_y, 2));
    }

    void tick() {
        int dir = rand() % NUM_DIRS;
        m_x += DIR_ARRAY[dir][0];
        m_y += DIR_ARRAY[dir][1];
    }

    bool infected(){
        return m_infected;
    }
    void infect(){
        m_infected = true;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Person& p){
        stream << p.m_id << SEP << (int)p.m_x << SEP << (int)p.m_y  << SEP
               << p.m_infected << std::endl;
    }
};

size_t Person::nextid = 0;
#endif // PERSON_H_INC
