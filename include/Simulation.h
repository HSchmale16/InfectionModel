/**\file Simulation.h
 * \author Henry Jacob Schmale
 * \date 09/14/2015
 */

#ifndef SIMULATION_H_INC
#define SIMULATION_H_INC

#include <vector>
#include <fstream>
#include "Person.h"

class Simulation{
private:
    std::vector<Person> m_persons;
    std::ofstream m_simfile;   //!< File to dump the simulation state each tick
    std::ofstream m_reportfile;
    int m_tick;
protected:
public:
    Simulation(){
        m_tick = 0;
        size_t persons = iniparser_getint(gConfig, "people:num", 100);
        for(size_t i = 0; i < persons; i++){
            m_persons.push_back(Person());
        }
        m_reportfile.open(iniparser_getstring(gConfig, "world:reportfile", NULL));
        if(!m_reportfile){
            throw 1;
        }
        m_simfile.open(iniparser_getstring(gConfig, "world:simfile", NULL));
        if(!m_simfile){
            throw 1;
        }
    }
    
    ~Simulation(){
        m_reportfile.close();
    }

    /**\param ter The infector
     * \param ee The infectee
     */
    bool performInfection(Person& ter, Person& ee){
        if(ter.getDistance(ee) < iniparser_getdouble(gConfig, "inf:radius", 2)){
            ee.infect();
            return true;
        }
        return false;
    }

    int tick(){
        m_tick++;
        m_reportfile << "TURN " << m_tick << std::endl;
        // it0 is the infector
        // it1 is the infectee
        for(std::vector<Person>::iterator it0 = m_persons.begin();
                it0 != m_persons.end(); ++it0){
            if(it0->infected() == false)
                continue; // skip this iteration
            for(std::vector<Person>::iterator it1 = m_persons.begin();
                    it1 != m_persons.end(); ++it1){
                if(it1 == it0)
                    continue; // skip over self
                if(it0->infected() == true){
                    this->performInfection(*it0, *it1);
                }
            }
        }
        int infectedCount = 0;
        for(std::vector<Person>::iterator it = m_persons.begin();
                it != m_persons.end(); ++it){
            if(it->infected() == true){
                infectedCount++;
            }
            m_simfile << *it;
            it->tick();
        }
        m_reportfile << "Total Infected: " << infectedCount << std::endl;
        // time to quit?
        if(infectedCount != m_persons.size())
            return 0;
        else
            return 1; // sim over
    }
};
#endif // SIMULATION_H_INC
