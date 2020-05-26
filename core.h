#ifndef CORE_H
#define CORE_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <math.h>


#define MAX 100000000


// 1-gram list from Beker and Piper (1982)
double frequency [26] = {   8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966,
                            0.153, 0.772, 4.027, 2.406,  6.749, 7.507, 1.929, 0.095, 5.987, 
                            6.327, 9.056, 2.758, 0.978,  2.360, 0.150, 1.974, 0.074         };

// Pre-processed data
double bigram[26][26];
double trigram[26][26][26];
double fourgram[26][26][26][26];
double fivegram[26][26][26][26][26];

// Solutions data
double msgBigram[26][26];
double msgTrigram[26][26][26];
double msgFourgram[26][26][26][26];
double msgFivegram[26][26][26][26][26];

// Storing original text and original ciphered text
char text[MAX];
char originalCipher[MAX];
char originalKey[1][26+1] = {"abcdefghijklmnopqrstuvwxyz"}; // +1 for \0

// Solution's text and ciphered text
char message[MAX];
char cipherText[MAX];


// Converts both upper and lower case chars and get corresponding index from 0 ~ 25 from each 
int alphaPos(char c) { return ( (int) c % 32 ) - 1; } 

// Build frequency tables (ngram, ngram2, ... , ngram5)
void buildNgrams(char message[], int n) {
    
    // Below, the first for loop calculates the decrypted text frequencies
    // The following for loop(s) calculates the absolute frequency
    // This pattern repeats for each ngram  
    
    // Bigrams
    double totalBigrams = 0.0;
    for(int i=0; i<n-1; i++){
        
        int c1,c2;
        c1 = alphaPos(message[i]);
        c2 = alphaPos(message[i+1]);
        
        msgBigram[c1][c2]++;
        totalBigrams++;
    }
    
    for(int i=0; i<26; i++)
        for(int j=0; j<26; j++) 
            msgBigram[i][j] /= totalBigrams;

    // Trigrams
    double totalTrigrams = 0.0;
    for(int i=0; i<n-2; i++){
        
        int c1,c2,c3;
        c1 = alphaPos(message[i]);
        c2 = alphaPos(message[i+1]);
        c3 = alphaPos(message[i+2]);
        
        msgTrigram[c1][c2][c3]++;
        totalTrigrams++;
    }
    
    for(int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++)
                msgTrigram[i][j][k] /= totalTrigrams;
    
    // Fourgrams 
    double totalFourgrams= 0.0;
    for(int i=0; i<n-3; i++){
        
        int c1,c2,c3,c4;
        c1 = alphaPos(message[i]);
        c2 = alphaPos(message[i+1]);
        c3 = alphaPos(message[i+2]);
        c4 = alphaPos(message[i+3]);
        
        msgFourgram[c1][c2][c3][c4]++;
        totalFourgrams++;
    }
    
    for(int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++)
                for(int l=0; l<26; l++)
                msgFourgram[i][j][k][l] /= totalFourgrams;
    
    // Fivegrams 
    double totalFivegrams= 0.0;
    for(int i=0; i<n-4; i++){
        
        int c1,c2,c3,c4,c5;
        c1 = alphaPos(message[i]);
        c2 = alphaPos(message[i+1]);
        c3 = alphaPos(message[i+2]);
        c4 = alphaPos(message[i+3]);
        c5 = alphaPos(message[i+4]);
        
        msgFivegram[c1][c2][c3][c4][c5]++;
        totalFivegrams++;
    }
   
    for(int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++)
                for(int l=0; l<26; l++)
                    for(int m=0; m<26; m++)
                        msgFivegram[i][j][k][l][m] /= totalFivegrams;
}

// Substitution cipher by mapping
void encrypt(char key[][27], char cipherText[], int n){

    for(int i=0; i<n; i++) 
        cipherText[i] = key[0][alphaPos(text[i])];

}

// Reversing the encryption
void decrypt(int p, char key[][27], char message[], int n) {
    
    char reverseKey[1][26+1] = {"--------------------------"};
    char c = 'a';
    
    // Mapping the correct char to the position corresponding to a given char in the key
    for(int i=0; i<26; i++) reverseKey[0][alphaPos(key[p][i])] = c++; 
    for(int i=0; i<n; i++) message[i] = reverseKey[0][alphaPos(originalCipher[i])];
    
}

// Computes the current text's character frequency
void getFreqs(char message[], double freqs[], int n){

    double total = n*1.0;
    #pragma unroll   
    for(int i=0; i<n; i++)
        freqs[alphaPos(message[i])]++;

    for(int i=0; i<26; i++)
        freqs[i] /= total;
}

double cost(char message[], int n) {
    
    double unigramCost  = 0.0;
    double bigramCost   = 0.0;
    double trigramCost  = 0.0;
    double fourgramCost = 0.0;
    double fivegramCost = 0.0;
    
    // Weights assigning more/less importance to each part of the cost function
    double w1 = 1;
    double w2 = 4;
    double w3 = 6;
    double w4 = 8;
    double w5 = 10;

    double msgFreqs[26] = {0.0};
    getFreqs(message, msgFreqs, n);
    buildNgrams(message, n);
    
    #pragma unroll
    for(int i=0; i<26; i++)
        unigramCost += fabs(frequency[i] - msgFreqs[i]);

    for(int i=0; i<26; i++)
        #pragma unroll
        for(int j=0; j<26; j++)
            bigramCost += fabs(bigram[i][j] - msgBigram[i][j]);

    for(int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            #pragma unroll
            for(int k=0; k<26; k++)
                trigramCost += fabs(trigram[i][j][k] - msgTrigram[i][j][k]);

    for(int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++)
                #pragma unroll
                for(int l=0; l<26; l++)
                    fourgramCost += fabs(fourgram[i][j][k][l] - msgFourgram[i][j][k][l]);
    
    for(int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++)
                for(int l=0; l<26; l++)
                    #pragma unroll
                    for(int m=0; m<26; m++)
                        fivegramCost += fabs(fivegram[i][j][k][l][m] - msgFivegram[i][j][k][l][m]);
    

    unigramCost *= w1;
    bigramCost *= w2;
    trigramCost *= w3;
    fourgramCost *= w4;
    fivegramCost *= w5;
    
    return unigramCost + bigramCost + trigramCost + fourgramCost + fivegramCost;
}

// Print information about the solution obtained
void stats(char message[], char key [][27], char originalKey[][27], int n) {
   
    int equal = 0; 
    std::cerr << "Original key: ";
    for(int i=0; i<26; i++) {
        if(originalKey[0][i] == key[0][i]) equal++; 
        std::cerr<< originalKey[0][i];
    }
    std::cerr << "\nKey Found: ";
    for(int i=0; i<26; i++) std::cerr<< key[0][i];
    
    // If you wan't to look at how the deciphered text is like, its here you should print it
    int ct=0;
    for(int i=0; i<n; i++) 
        if(message[i] == text[i]) ct++;
        
    std::cout << "\nCorrect Key elements retrieved: " << equal << " original to decrypted text similarity " << ct/(1.0*n) << "\n";
    
}

// Getting pre-processed ngram data and text data
void getDataset(const char *textFile, int &n){

    std::ifstream file;
    file.open("ngrams/ngram2", std::ifstream::in);
    for (int i=0; i<26; i++)
        for(int j=0; j<26; j++) {
            double freq = 0;
            file >> freq;
            bigram[i][j] = freq;
        }
    file.close();
    
    file.open("ngrams/ngram3", std::ifstream::in);
    for (int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++){
                double freq = 0;
                file >> freq;
                trigram[i][j][k] = freq;
            }
    file.close();
    
    file.open("ngrams/ngram4", std::ifstream::in);
    for (int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++)
                for(int l=0; l<26; l++){
                    double freq = 0;
                    file >> freq;
                    fourgram[i][j][k][l] = freq;
                }
    file.close();
    
    file.open("ngrams/ngram5", std::ifstream::in);
    for (int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++)
                for(int l=0; l<26; l++)
                    for(int m=0; m<26; m++) {
                        double freq = 0;
                        file >> freq;
                        fivegram[i][j][k][l][m] = freq;
                    }
    file.close();

    file.open(textFile, std::ifstream::in);
    char c = char();
    while(file >> c) 
        if(isalpha(c)) text[n++] = tolower(c);
    file.close();

}

#endif
