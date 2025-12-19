#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
#include <limits.h>
#include "predictor.h"

using namespace std;

void printBits(unsigned long num);

/**
 * Bimodal Constructor
 */
Predictor::Predictor(unsigned long int M2){
   this->m2Mask = ((1 << M2) - 1) << 2;
   this->M2 = M2;
   this->N = 0;
   this->bimPredictor.resize(pow(2, M2));
   for(unsigned long int i = 0; i < bimPredictor.size(); i++){
       this->bimPredictor[i] = 2;
   }
}

/**
 * GShare constructor
 */
Predictor::Predictor(unsigned long int M1, unsigned long int N){ 
   this->m1Mask = ((1 << M1) - 1) << 2;
   this->mnMask = ((1 << (M1 - N)) - 1);
   this->bhr = 0;
   this->M2 = 0;
   this->M1 = M1;
   this->N = N;
   this->gshPredictor.resize(pow(2, M1));
   for(unsigned long int i = 0; i < gshPredictor.size(); i++){
       this->gshPredictor[i] = 2;
   }
}

/**
 * Hybrid constructor
 */
Predictor::Predictor(unsigned long int K, unsigned long int M1, unsigned long int N, unsigned long int M2){
   this->kMask = ((1 << K) - 1) << 2;
   this->m1Mask = ((1 << M1) - 1) << 2;
   this->mnMask = ((1 << (M1 - N)) - 1);
   this->m2Mask = ((1 << M2) - 1) << 2;
   this->bhr = 0;
   this->K = K;
   this->M1 = M1;
   this->N = N;
   this->M2 = M2;
   this->chooserTable.resize(pow(2, K));
   this->gshPredictor.resize(pow(2, M1));
   this->bimPredictor.resize(pow(2, M2));
   for(unsigned long int i = 0; i < this->chooserTable.size(); i++){
       this->chooserTable[i] = 1;
   }
   for(unsigned long int i = 0; i < gshPredictor.size(); i++){
       this->gshPredictor[i] = 2;
   }
   for(unsigned long int i = 0; i < bimPredictor.size(); i++){
       this->bimPredictor[i] = 2;
   }
}

Predictor::~Predictor(){
    
}

void Predictor::prediction(unsigned long int pc, char outcome){   
    this->numPredicts++;
    //check masks
    //   if(this->numPredicts < 3){
    //       printf("%lx      ",pc);
    //       printBits(pc);
    //       printf("mMask:");
    //       printBits(this->mMask);
    //       printf("idx:        ");
    //       printBits(idx);
    //   }

    if(this->M1 == 0 && this->K == 0){
//        printf("bimodal\n");
        unsigned long int idx;
        char prediction = bimPredict(pc, &idx, outcome);
        updateOutcome(idx, this->bimPredictor, prediction, outcome);
    }else if(this->M2 == 0 && this->K == 0){
//        printf("gshare\n");
        unsigned long int idx;
        char prediction = gshPredict(pc, &idx, outcome);
        updateOutcome(idx, this->gshPredictor, prediction, outcome);
    }else{
//        printf("hybrid\n");
        hybPredict(pc, outcome);
    } 
}

char Predictor::bimPredict(unsigned long int pc, unsigned long int *idx, char outcome){
    char prediction;

    *idx = (pc & this->m2Mask) >> 2;
    prediction = (this->bimPredictor[*idx] >= 2) ? 't' : 'n';

    return prediction;
}

char Predictor::gshPredict(unsigned long int pc, unsigned long int *idx, char outcome){
    unsigned long int mn;
    char prediction;

    *idx = (pc & this->m1Mask) >> 2;
    if(this->N !=0){
        mn = *idx & this->mnMask;
        // adjusting the index based on the BHR XORed on the PC bits.
        *idx = (*idx >> (this->M1 - this->N) ^ bhr) << (this->M1 - this->N) ^ mn;
        
        if(outcome == 't'){
            bhr = (bhr >> 1) + (1 << (this->N - 1));
        }else if(outcome == 'n'){
            bhr = (bhr >> 1);
        }
    }

    prediction = (this->gshPredictor[*idx] >= 2) ? 't' : 'n';

    return prediction;
}

void Predictor::hybPredict(unsigned long int pc, char outcome){
    unsigned long int gshIdx;
    unsigned long int bimIdx;
    unsigned long int idx = (pc & this->kMask) >> 2;
    char gshPrediction = gshPredict(pc, &gshIdx, outcome);
    char bimPrediction = bimPredict(pc, &bimIdx, outcome);

    if(this->chooserTable[idx] >= 2){
        updateOutcome(gshIdx, this->gshPredictor, gshPrediction, outcome);
    }else{
        updateOutcome(bimIdx, this->bimPredictor, bimPrediction, outcome);
    }
    if(gshPrediction != bimPrediction){
        if(gshPrediction == outcome){
            updateOutcome(idx, this->chooserTable, 't', 't');
        }else{
            updateOutcome(idx, this->chooserTable, 'n', 'n');
        }
    }
}

void Predictor::updateOutcome(unsigned long int idx, vector<unsigned long int>& table, char prediction, char outcome){
    if(outcome != prediction){ this->numMispredicts++; }

    if(outcome == 't'){
        if(table[idx] < 3){ table[idx]++; }
    }else if(outcome == 'n'){
        if(table[idx] > 0){ table[idx]--; }
    }
}

void Predictor::predictorContents(){
    float mispredictRate = float(this->numMispredicts) / float(this->numPredicts) * 100.0f;
    printf("OUTPUT\n");
    printf(" number of predictions:    %d\n", this->numPredicts);
    printf(" number of mispredictions: %d\n", this->numMispredicts);
    printf(" misprediction rate:       %.2f%%\n", mispredictRate);
    if(this->K != 0){
        printf("FINAL CHOOSER CONTENTS\n");
        for(unsigned long int i = 0; i < this->chooserTable.size(); i++){
            printf("%lu  %lu\n", i, this->chooserTable[i]);
        }
    }
    if(this->M2 == 0 || this->K != 0){
        printf("FINAL GSHARE CONTENTS\n");
        for(unsigned long int i = 0; i < this->gshPredictor.size(); i++){
            printf("%lu  %lu\n", i, this->gshPredictor[i]);
        }
    }
    if(this->M1 == 0 || this-> K != 0){
        printf("FINAL BIMODAL CONTENTS\n");
        for(unsigned long int i = 0; i < this->bimPredictor.size(); i++){
            printf("%lu  %lu\n", i, this->bimPredictor[i]);
        }
    }
}

void printBits(unsigned long num) {
    // Determine the number of bits in an unsigned long
    int num_bits = sizeof(unsigned long) * CHAR_BIT;

    // Iterate from the most significant bit to the least significant bit
    for (int i = num_bits - 1; i >= 0; i--) {
        // Check if the i-th bit is set (1) or not (0)
        // Use a bit mask to isolate the i-th bit
        if ((num >> i) & 1) {
            printf("1");
        } else {
            printf("0");
        }
        // Optional: Add spaces for readability, e.g., every 8 bits
        if (i > 0 && i % 8 == 0) {
            printf(" ");
        }
    }
    printf("\n");
}
