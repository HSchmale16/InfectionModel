/**\file Simulation.h
 * \author Henry Jacob Schmale
 * \date 09/14/2015
 */

#ifndef SIMULATION_H_INC
#define SIMULATION_H_INC

#include <vector>
#include <fstream>
#include "Person.h"

class Simulation : public sf::Drawable{
private:
    std::vector<Person> m_persons;
    std::ofstream m_simfile; //!< File to dump the sim state each tick
    std::ofstream m_reportfile;
    int m_tick;

    virtual void draw(sf::RenderTarget& target,
            sf::RenderStates states)const{
        for(int i = 0; i < m_persons.size(); i++){
            target.draw(m_persons[i]);
        }
    }
    
    /**\param ter The infector
     * \param ee The infectee
     */
    bool performInfection(Person& ter, Person& ee){
        if(ter.getDistance(ee) < 
                iniparser_getdouble(gConfig, "inf:radius", 2)){
            ee.infect();
            return true;
        }
        return false;
    }

    void writeSimParamReport(){
        char tmpfilename[] = "./tmpinidumpfile";
        FILE* ftmp = fopen(tmpfilename, "w");
        iniparser_dump(gConfig, ftmp);
        fclose(ftmp);
        std::ifstream infile(tmpfilename);
        char c;
        while(infile.get(c)){
            m_reportfile << c;
        } 
        infile.close();
        remove(tmpfilename);
    }
protected:
public:
    Simulation(){
        // open files for saving state and making reports
        if(iniparser_getboolean(gConfig, ":log", false) == true){
            m_reportfile.open(iniparser_getstring(gConfig, "world:reportfile",
                        NULL));
            if(!m_reportfile){
                throw 1;
            }
            m_simfile.open(iniparser_getstring(gConfig, "world:simfile", NULL));
            if(!m_simfile){
                throw 1;
            }
            writeSimParamReport();
        }
        // create initial population
        m_tick = 0;
        size_t persons = iniparser_getint(gConfig, "people:num", 100);
        for(size_t i = 0; i < persons; i++){
            Person p;
            m_persons.push_back(p);
        }
    }
    
    ~Simulation(){
        m_reportfile.close();
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
            it->tick();
        }
        // time to quit?
        if(infectedCount != m_persons.size()){
            // only flush out to file if log is true
            // after all why would we want to 
            if((iniparser_getboolean(gConfig, ":log", false) == true) &&
                    m_tick < iniparser_getint(gConfig, "world:ticks", 1000)){
                for(std::vector<Person>::iterator it = m_persons.begin();
                        it != m_persons.end(); ++it){
                    m_simfile << *it;
                }
                m_reportfile << "Infected : " << infectedCount << std::endl
                             << "Healthy  : " << m_persons.size() - infectedCount
                             << std::endl;
            }
            return 0;
        }else{
            return 1; // sim over
        }
    }
};

#endif // SIMULATION_H_INC
