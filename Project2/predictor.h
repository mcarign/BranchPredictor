#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#ifndef PREDICTOR_H
#define PREDICTOR_H

class Predictor{
    private:
        // Index made from PC and global branch history register.
        unsigned long int m2Mask;
        unsigned long int m1Mask;
        unsigned long int mnMask;
        unsigned long int kMask;
        // Branch history register, XORed with the PC for indexing.
        unsigned long int bhr;
        unsigned long int N = 0;
        unsigned long int M2 = 0;
        unsigned long int K = 0;
        unsigned long int M1 = 0;
        // 1D array for the BHT.
        std::vector<unsigned long int> bimPredictor;
        std::vector<unsigned long int> gshPredictor;
        std::vector<unsigned long int> chooserTable;
        // Counters for measurements
        int numPredicts;
        int numMispredicts;
        
    public:
        Predictor(unsigned long int M1);
        
        Predictor(unsigned long int M1, unsigned long int N);
        
        Predictor(unsigned long int K, unsigned long int M1, unsigned long int N, unsigned long int M2);

        ~Predictor();
        
        /**
         * Searches BHT for the expected outcome. PC is used to compute the index to search BHT.
         * @param pc is the PC bits that will be used to compute the index.
         * @return the two-bit counter prediction.
         */
        void prediction(unsigned long int pc, char outcome);
        
        char bimPredict(unsigned long int pc, unsigned long int *idx, char outcome);

        char gshPredict(unsigned long int pc, unsigned long int *idx, char outcome);

        void hybPredict(unsigned long int pc, char outcome);

        /**
         * Updates the outcome if there was a misprediciton.
         */
        void updateOutcome(unsigned long int idx, std::vector<unsigned long int>& table, char prediction, char outcome);

        void predictorContents();
};

#endif
