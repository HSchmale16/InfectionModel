#ifndef PERSON_H_INC
#define PERSON_H_INC

/**\file Person.h
 * \author Henry Jacob Schmale
 * \date 09/14/2015
 */

#include <cmath>

class Person{
    private:
    protected:
        float   m_x;
        float   m_y;
        bool    m_infected;
    public:
        Person()
            :m_infected(false){

            }

        ~Person();

        float getDistance(Person& p){
            return sqrt(pow(this->m_x - p.m_x, 2) +
                    pow(this->m_y - p.m_y, 2));
        }

        void tick(){

        }
};
#endif // PERSON_H_INC
