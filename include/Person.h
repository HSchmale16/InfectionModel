#ifndef PERSON_H_INC
#define PERSON_H_INC

/**\file Person.h
 * \author Henry Jacob Schmale
 * \date 09/14/2015
 */

#include <cmath>
#include <cstdio>
#include <SFML/Graphics.hpp>
#include "global.h"

#define SCRATCH_BUFF_LEN 100

static char P_BUFF[SCRATCH_BUFF_LEN];

// define seperator character
#define SEP '\t'
// Screen Pixel Coordinate Calculation
#define SCR_X(x) (((x) / MAP_SIZE) * SCREEN_MIN)
#define SCR_Y(y) (((y) / MAP_SIZE) * SCREEN_MIN)
#define P_SEARCH_BUFF(id,val) \
        snprintf(P_BUFF, SCRATCH_BUFF_LEN, "P%d:%s", id, val);
// Make getting the map size a simple macro
#define MAP_SIZE iniparser_getint(gConfig, "world:size", 100) 
#define MAP_SIZE_2 (MAP_SIZE / 2)

const double DIR_ARRAY[][2] = {
    {0, 0},
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

class Person : public sf::Drawable{
private:
    sf::CircleShape m_s;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states)const{
        target.draw(m_s);
    }
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
            m_x = iniparser_getdouble(gConfig, P_BUFF, rand() % MAP_SIZE);
            P_SEARCH_BUFF(m_id, "y");
            m_y = iniparser_getdouble(gConfig, P_BUFF, rand() % MAP_SIZE); 
            P_SEARCH_BUFF(m_id, "v");
            m_infected = iniparser_getboolean(gConfig, P_BUFF, false);
            m_s.setRadius(3.f);
            m_s.setPosition(SCR_X(m_x), SCR_Y(m_y));
            if(m_infected){
                this->infect();
            }
    }

    double getDistance(Person& p) {
        return sqrt(pow(this->m_x - p.m_x, 2) +
                        pow(this->m_y - p.m_y, 2));
    }

    void tick() {
        int dir = rand() % NUM_DIRS;
        m_x += DIR_ARRAY[dir][0];
        if(m_x >= MAP_SIZE)
            m_x = 0;
        if(m_x <= 0)
            m_x = MAP_SIZE;
        m_y += DIR_ARRAY[dir][1];
        if(m_y >= MAP_SIZE)
            m_y = 0;
        if(m_y <= 0)
            m_y = MAP_SIZE;
        m_s.setPosition(SCR_X(m_x), SCR_Y(m_y));
    }

    bool infected(){
        return m_infected;
    }
    
    void infect(){
        m_infected = true;
        m_s.setFillColor(sf::Color::Red);
    }

    friend std::ostream& operator<<(std::ostream& stream, const Person& p){
        stream << p.m_id << SEP << (int)p.m_x << SEP << (int)p.m_y  << SEP
               << p.m_infected << std::endl;
    }
};

size_t Person::nextid = 0;

#endif // PERSON_H_INC
