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
    int64_t m_infected   :  1;
    int64_t m_doctor     :  1;
    int64_t m_infectlast : 16; // if infected how many ticks does it last
    int64_t m_cough      :  1; // can only infect if coughing this turn
    int64_t m_dir        :  4; // direction to travel
public:
    Person()
        :m_id(nextid++){
            // init the simulation values from the sim file
            INI_SEC_ID_GEN(m_id, "x", P);
            m_x = iniparser_getdouble(gConfig, P_BUFF, rand() % MAP_SIZE);
            INI_SEC_ID_GEN(m_id, "y", P);
            m_y = iniparser_getdouble(gConfig, P_BUFF, rand() % MAP_SIZE); 
            INI_SEC_ID_GEN(m_id, "v", P);
            m_infected = iniparser_getboolean(gConfig, P_BUFF, false);
            INI_SEC_ID_GEN(m_id, "d", P);
            m_doctor = iniparser_getboolean(gConfig, P_BUFF, false);
            
            // init the shape
            m_s.setRadius(3.f);
            m_s.setPosition(SCR_X(m_x), SCR_Y(m_y));
            if(m_infected){
                this->infect();
            }
            pickNewDir();
    }

    double getDistance(Person& p) {
        return sqrt(pow(this->m_x - p.m_x, 2) +
                        pow(this->m_y - p.m_y, 2));
    }

    void pickNewDir(){
        int old_dir = m_dir;
        do{
            m_dir = (*dir_distrib)(generator);
        }while(old_dir == m_dir);
    }

    void tick() {
        // bounds check
        if(((m_x >= MAP_SIZE) || (m_x <= 0)) || 
            ((m_y >= MAP_SIZE) || (m_y <= 0))) this->pickNewDir();
            
        m_x += DIR_ARRAY[m_dir][0];
        m_y += DIR_ARRAY[m_dir][1];
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
