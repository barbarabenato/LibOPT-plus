#include "cs.h"

/* It computes the number of nests that will be replaced, taking into account a probability [0,1] */
int NestLossParameter(int size, float probability){
    int value, loss;
    value = size*probability;

    if(value + 0.5 > (int)value + 1)
        loss = (int)value;
    else
        loss = (int)value + 1;

    return loss;
}

/* It executes the Cuckoo Search for function minimization
Parameters:
s: search space
Evaluate: pointer to the function used to evaluate nests
arg: list of additional arguments */
void runCS(SearchSpace *s, prtFun Evaluate, ...){
    va_list arg, argtmp;
    int t, i, j, nest_i, nest_j, loss;
    double rand, *L = NULL, fitValue;
    Agent *tmp = NULL;
    		
    va_start(arg, Evaluate);
    va_copy(argtmp, arg);
    
    if(!s){
        fprintf(stderr,"\nSearch space not allocated @runCS.\n");
        exit(-1);
    }
        
    EvaluateSearchSpace(s, _CS_, Evaluate, arg); /* Initial evaluation of the search space */
    qsort(s->a, s->m, sizeof(Agent**), SortAgent); /* Sorts all nests according to their fitness. First position gets the best nest. */
    
    for(t = 1; t <= s->iterations; t++){
        fprintf(stderr,"\nRunning iteration %d/%d ... ", t, s->iterations);
            
    	va_copy(arg, argtmp);
        tmp = GenerateNewAgent(s, _CS_);
	
        /* Equation 1 */
        L = GenerateLevyDistribution(s->n, s->beta);
        for(j = 0; j < s->n; j++)
            tmp->x[j] = tmp->x[j]+s->alpha*L[j];
        free(L);
        /**************/

        CheckAgentLimits(s, tmp);
        do{
        	nest_i = GenerateUniformRandomNumber(0, s->m-1);
        }while(nest_i != 0);

        fitValue = Evaluate(tmp, arg); /* It executes the fitness function for agent i */
        if(fitValue < s->a[nest_i]->fit){ /* We accept the new solution */
            DestroyAgent(&(s->a[nest_i]), _CS_);
            s->a[nest_i] = CopyAgent(tmp, _CS_);
            s->a[nest_i]->fit = fitValue;
        }

        DestroyAgent(&tmp, _CS_);
        
        loss = NestLossParameter(s->m, s->p);
        
        for(i = s->m-1; i > loss; i--){
            tmp = GenerateNewAgent(s, _CS_);
            /* Random walk */
            rand = GenerateUniformRandomNumber(0,1);
            do{
            	nest_i = GenerateUniformRandomNumber(0, s->m-1);
            	nest_j = GenerateUniformRandomNumber(0, s->m-1);
            }while(nest_i && nest_j != 0);
            for(j = 0; j < s->n; j++)
                tmp->x[j] = tmp->x[j]+rand*(s->a[nest_i]->x[j]-s->a[nest_j]->x[j]);
            /**************/
			
			CheckAgentLimits(s, tmp);
			
            fitValue = Evaluate(tmp, arg); /* It executes the fitness function for agent i */
            if(fitValue < s->a[i]->fit){ /* We accept the new solution */
                DestroyAgent(&(s->a[i]), _CS_);
                s->a[i] = CopyAgent(tmp, _CS_);
                s->a[i]->fit = fitValue;
            }
            DestroyAgent(&tmp, _CS_);
        }

        qsort(s->a, s->m, sizeof(Agent**), SortAgent); /* Sorts all nests according to their fitness. First position gets the best nest. */
	
        va_copy(arg, argtmp);
        fprintf(stderr, "OK (minimum fitness value %lf)", s->a[0]->fit);
    }

    va_end(arg);
}
