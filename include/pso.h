/* PSO implementation is based on the paper available at http://www.cs.tufts.edu/comp/150GA/homeworks/hw3/_reading6%201995%20particle%20swarming.pdf
AIWPSO implementation is based on the paper available at http://ceit.aut.ac.ir/~ebadzadeh/PSO.htm */

#ifndef PSO_H
#define PSO_H

#include "opt.h"

/* PSO-related functions */
void UpdateParticleVelocity(SearchSpace *s, int i); /* It updates the velocity of an agent (particle) */
void UpdateParticlePosition(SearchSpace *s, int i); /* It updates the position of an agent (particle) */
void runPSO(SearchSpace *s, prtFun Evaluate, ...); /* It executes the Particle Swarm Optimization for function minimization */
/*************************/

/* AIWPSO-related functions */
void runAIWPSO(SearchSpace *s, prtFun Evaluate, ...); /* It executes the Particle Swarm Optimization with Adpative Inertia Weight for function minimization */
double ComputeSuccess(SearchSpace *s); /* It computes the pecentage of success concerning the whole search space */
/****************************/

#endif