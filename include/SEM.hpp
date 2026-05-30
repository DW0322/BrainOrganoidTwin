#ifndef SEM_HPP
#define SEM_HPP

#include "structs.hpp"
#include <fstream>

void force(ParticleData &pd, const constants &p);
void updatePosition(ParticleData &pd, const constants &p);
void writeToFile(ofstream &out, const ParticleData &pd, const int step,
                 const constants &p, double U);
double totalEnergy(const ParticleData &pd, const constants &p);
#endif // SEM_H
