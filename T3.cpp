#include <bits/stdc++.h>
using namespace std;
#define MAX 100000000

// 1-gram list from Beker and Piper (1982)
double frequency [26] = { 8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966,
                         0.153, 0.772, 4.027, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987, 
                         6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074 };

// Pre-processed data
double bigram[26][26] = {{0.0}};
double trigram[26][26][26] = {{{0.0}}};
double fourgram[26][26][26][26] = {{{{0.0}}}};
double fivegram[26][26][26][26][26] = {{{{{0.0}}}}};

// Solutions data
double msgBigram[26][26] = {{0.0}};
double msgTrigram[26][26][26] = {{{0.0}}};
double msgFourgram[26][26][26][26] = {{{{0.0}}}};
double msgFivegram[26][26][26][26][26] = {{{{{0.0}}}}};

// Storing original text and original ciphered text
char text[MAX];
char originalCipher[MAX];

// Get corresponding index from 0 ~ 25 from each char
// Converts both upper and lower case chars (Stack overflow courtesy)
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


void getFreqs(char message[], double freqs[], int n){

    double total = n*1.0;
    #pragma unroll   
    for(int i=0; i<n; i++)
        freqs[alphaPos(message[i])]++;

    for(int i=0; i<26; i++)
        freqs[i] /= total;
}

double cost(char message[], int n) {
    
    double unigramCost = 0.0;
    double bigramCost = 0.0;
    double trigramCost = 0.0;
    double fourgramCost = 0.0;
    double fivegramCost = 0.0;
    
    // Weights to assign different importance to each part of the function
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

double greedy(char key[][27], int k, int n) {
    
    double bestCost = numeric_limits<double>::max();
    char msg[n] = "";
    
    for(int i=0; i<n; i++) msg[i] = originalCipher[i];

    // Swap each char staying with the swap that minimizes the cost
    for(int i=0; i<26; i++){
        for(int j=0; j<26; j++){

            
            if(i==j) continue;
            
            int a = i;
            int b = j;

            swap(key[k][a], key[k][b]);
            decrypt(k, key, msg, n);
            double currCost = cost(msg, n);
            
            // If the current move improved the solution, leave it that way
            if(currCost<bestCost)
                bestCost = currCost;
            else 
                swap(key[k][a], key[k][b]);
        }    
    }

    return bestCost;
    
}
// OX, Order crossover operator
void crossover(char parents[][27], int p1, int p2, double mutation, 
                char offspring[][27], int n, int &worstParent, double &offspringFit){
    
    // Select an interval in the solution to exchange genes
    int p,q;
    p = rand()%26;
    q = rand()%26;
    
    char pMsg[n];
    double p1Fitness, p2Fitness;
    
    
    decrypt(p1, parents, pMsg, n);
    p1Fitness = cost(pMsg,n);
    
    decrypt(p2, parents, pMsg, n);
    p2Fitness = cost(pMsg,n);

    worstParent = (p1Fitness<p2Fitness)?p2:p1;

    // The interval must have a length greater than 1 and force p < q
    while(abs(p-q) < 1) q = rand()%26; 
    if(p>q) swap(p,q);

    // Two point crossover
    bool hasGene[26] = {false};
    for(int i=p; i<q; i++) {
        char gene = parents[p1][i];
        offspring[0][i] = gene;
        hasGene[alphaPos(gene)] = true;
    }
    
    bool looped = false;
    int pos = q;
    for(int i=q; i<26; i++) {
        
        char gene = parents[p2][i];
        if(!hasGene[alphaPos(gene)]){
            offspring[0][pos++] = gene; 
            hasGene[alphaPos(gene)] = true;
        }
        // If finished going through a solution array, loop back to its beginning
        if(i==25 && !looped) {
            i = -1;
            looped = true;
        }
        // Circularity while filling the key is independent of i circularity
        if(pos==26) pos = 0;
    }
    
    if(mutation <= 0.05) {

        greedy(offspring, 0, n);

    }
    
    // To evaluate the cost of a solution, first you must decipher the message with the found key
    // and then rank the text frequencies against the known ngrams
    decrypt(0, offspring, pMsg, n);
    offspringFit = cost(pMsg,n);
    
}
void nextNeighbor(char s[][27], char t[][27]){

    #pragma unroll
    for(int i=0;i<26;i++) t[0][i] = s[0][i];
    
    int a = rand()%26;
    int b = rand()%26;
    
    while(a==b) b = rand()%26;
    swap(t[0][a], t[0][b]);

}
void simulatedAnnealing(char key[][27] , double &bestFitness,int n) {

    // Setting initial parameters
    char s[1][26+1] = {"abcdefghijklmnopqrstuvwxyz"}; // +1 for \0
    char t[1][26+1] = {"abcdefghijklmnopqrstuvwxyz"}; // +1 for \0
    char pMsg[n];
    
    random_shuffle( s[0], s[0]+(26*sizeof(char)) );
	greedy(s, 0, n);
    decrypt(0, s, pMsg, n);
    double currBest = cost(pMsg,n);
    
    double T = 100; // Dummy val
	double Tmin = 0.001;
	int maxTrials = 100;

    double bestCost = std::numeric_limits<double>::max();
    char best[1][26+1] = {"xxxxxxxxxxxxxxxxxxxxxxxxxx"};
	
    while(T>Tmin){

	
		double trials = 0.0;
		double changes = 0.0;
		double equilibrium = 0.0;
		
		while(trials < maxTrials) {
			trials++;
			nextNeighbor(s,t);

            decrypt(0, t, pMsg, n);
			double currSol = cost(pMsg,n);
			double delta = currSol - currBest;
		
            // Updates global best if found a better key
		    if(currSol < bestCost) {
                bestCost = currSol;
                #pragma unroll
                for(int i=0;i<26;i++) best[0][i] = t[0][i];
		    }

		    if(delta <= 0) {
                changes++;
                currBest = currSol;
                #pragma unroll
                for(int i=0;i<26;i++) s[0][i] = t[0][i];
		    } else {
                
                double e = 1.0/exp(delta/T);
                double R = (rand()%100 + 1)/100.0;
                
                // Accepts a bad move with a given probability
                if(e>R)	{
                    changes++;
                    currBest = currSol;
                    #pragma unroll
                    for(int i=0;i<26;i++) s[0][i] = t[0][i];
                }
            }		
		} // Internal Loop

        // Cooling schedule
		equilibrium = (changes*1.0)/(trials*1.0);
		double k = ((equilibrium>1)?equilibrium/100.0:equilibrium)*0.25;
		T *= 0.60 + k; 
	
    } // Temperature loop

    #pragma unroll
    for(int i=0;i<26;i++) key[0][i] = best[0][i];
    decrypt(0, key, pMsg, n);
    bestFitness = cost(pMsg,n);
}
void stats(char message[], char key [][27], char originalKey[][27], int n) {
   
    int equal = 0; 
    //cerr << "Key original :";
    for(int i=0; i<26; i++) {
        if(originalKey[0][i] == key[0][i]) equal++; 
        //cerr<< originalKey[0][i];
    }
    //cerr << "\nKey Found :";
    //for(int i=0; i<26; i++) cerr<< key[0][i];
    
    //cerr << "\nOriginal Text:\n";
    //for(int i=0; i<n; i++) 
        //cerr << text[i];
    
    //cerr << "\nTexto recuperado :\n";
    int ct=0;
    for(int i=0; i<n; i++) {
        if(message[i] == text[i]) ct++;
        //cerr << message[i];
    }
    
    cout << equal << " " << ct/(1.0*n) << endl;
    //cout << "\nSimilaridade " << ct/(1.0*n) << endl;
    //cout << "\nKey equal elements " << equal << endl;
}

int main(int argc, char **argv) {
    
    
    srand (time(NULL));
    char c = char();
    int n = 0; // Number of chars in text
    int size = atoi(argv[2]); // Population size
    int generations = atoi(argv[3]); 
    int method = atoi(argv[4]);
    
    /* -----------  Getting pre processed data and text data ------------ */
    ifstream file;
    file.open("ngrams/ngram2", ifstream::in);
    for (int i=0; i<26; i++)
        for(int j=0; j<26; j++) {
            double freq = 0;
            file >> freq;
            bigram[i][j] = freq;
        }
    file.close();
    
    file.open("ngrams/ngram3", ifstream::in);
    for (int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++){
                double freq = 0;
                file >> freq;
                trigram[i][j][k] = freq;
            }
    file.close();
    
    file.open("ngrams/ngram4", ifstream::in);
    for (int i=0; i<26; i++)
        for(int j=0; j<26; j++)
            for(int k=0; k<26; k++)
                for(int l=0; l<26; l++){
                    double freq = 0;
                    file >> freq;
                    fourgram[i][j][k][l] = freq;
                }
    file.close();
    
    file.open("ngrams/ngram5", ifstream::in);
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

    file.open(argv[1], ifstream::in);
    while(file >> c) 
        if(isalpha(c)) text[n++] = tolower(c);
    file.close();
    
    /* ------------ CORE ------------- */


    // Generate random original key
    char originalKey[1][26+1] = {"abcdefghijklmnopqrstuvwxyz"}; // +1 for \0
    random_shuffle( originalKey[0], originalKey[0]+(26*sizeof(char)) );
   
    // Initialize solution variables 
    char message[n];
    char cipherText[n];
    
    cerr << "Normal text score: " << cost(text, n) << endl;
    encrypt(originalKey, originalCipher, n);
    cerr << "Encrypted score: " << cost(originalCipher, n) << endl;

    // Initializing random popularion (Needs to fix last index not being shuffled)
    double bestFitness = numeric_limits<double>::max();
    char bestKey[1][26+1] = {"xxxxxxxxxxxxxxxxxxxxxxxxxx"};
    
    if(method==1) {
        
        simulatedAnnealing(bestKey, bestFitness, n);
        
        decrypt(0, bestKey, message, n);
        cout << argv[1] << " ";
        stats(message, bestKey, originalKey, n);
    }

    else {
        
        for(int i=0; i<generations; i++) {
            int ct = 0;
            //cerr << "Generation : " << i << endl;
            char population[size][26+1];
            #pragma omp parallel for
            for(int i=0; i<size; i++){
                
                char c = 'a';
                
                #pragma unroll
                for(int j=0; j<26; j++) 
                    population[i][j] = c++;

                random_shuffle(population[i], population[i]+(26*sizeof(char)));
            }



            while(ct<size) {
                
                ct++;
                char key[1][26+1] = {"--------------------------"};
                int worstParent = -1;
                double offspringFit = numeric_limits<double>::max();
                
                // While offspring isn't better than parents
                int maxTrials = size*size;
                
                int p1 = rand()%size;
                int p2 = rand()%size;
                while(p1==p2) p2 = rand()%size;

                int temp = rand()%1000;
                double mutation = (temp*1.0)/100;
                crossover(population, p1, p2, mutation, key, n, worstParent, offspringFit);
                
                if(offspringFit < bestFitness) {
                    bestFitness = offspringFit;
                    
                    #pragma unroll
                    for(int j=0; j<26; j++) bestKey[0][j] = key[0][j];
                    
                }
                
                // Replacing worst parent
                #pragma unroll
                for(int k=0; k<26; k++) 
                    population[worstParent][k] = key[0][k];
            }
        }

        decrypt(0, bestKey, message, n);
        cout << argv[1] << " ";
        stats(message, bestKey, originalKey, n);
    }
}
