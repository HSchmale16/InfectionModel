/**\file Simulation.h
 * \author Henry Jacob Schmale
 * \date 09/14/2015
 */

#ifndef SIMULATION_H_INC
#define SIMULATION_H_INC

#include <vector>
#include "Person.h"

class Simulation{
private:
    std::vector<Person> m_persons;
protected:
public:
    Simulation();
    ~Simulation();
};
#endif // SIMULATION_H_INC
