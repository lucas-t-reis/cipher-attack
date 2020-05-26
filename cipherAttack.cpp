#include "core.h"

double greedy(char key[][27], int k, int n) {
    
    double bestCost = std::numeric_limits<double>::max();
    char msg[n] = "";
    
    for(int i=0; i<n; i++) msg[i] = originalCipher[i];

    // Swap each char staying with the swap that minimizes the cost
    for(int i=0; i<26; i++){
        for(int j=0; j<26; j++){

            
            if(i==j) continue;
            
            int a = i;
            int b = j;

            std::swap(key[k][a], key[k][b]);
            decrypt(k, key, msg, n);
            double currCost = cost(msg, n);
            
            if(currCost<bestCost)
                bestCost = currCost;
            else 
                std::swap(key[k][a], key[k][b]);

        }    
    }

    return bestCost;
    
}
// OX, Order crossover operator
void crossover( char parents[][27], int p1, int p2, double mutation, 
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
    if(p>q) std::swap(p,q);

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
    
    if(mutation <= 0.05) greedy(offspring, 0, n);
    
    // To evaluate the cost of a solution, first you must decipher the message with the found key
    // and then rank the text frequencies against the known ngrams
    decrypt(0, offspring, pMsg, n);
    offspringFit = cost(pMsg,n);
    
}

void geneticAlgorithm(char bestKey[][27] , double &bestFitness, int n, int generations, int size){

 for(int i=0; i<generations; i++) {

        std::cerr << "Generation " << i << ":\n";
        char population[size][26+1];

        #pragma omp parallel for
        for(int i=0; i<size; i++){
            
            char c = 'a';
            
            #pragma unroll
            for(int j=0; j<26; j++) population[i][j] = c++;

            std::random_shuffle(population[i], population[i]+(26*sizeof(char)));
        }
        
        int ct = 0;
        while(ct<size) {
            
            ct++;
            char key[1][26+1] = {"--------------------------"};
            int worstParent = -1;
            double offspringFit = std::numeric_limits<double>::max();
            
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

                decrypt(0, bestKey, message, n);
                std::cout << cost(message,n) << "\n";
                
            }
            
            // Replacing worst parent
            #pragma unroll
            for(int k=0; k<26; k++) 
                population[worstParent][k] = key[0][k];

        }
    }
}

void nextNeighbor(char s[][27], char t[][27]){

    #pragma unroll
    for(int i=0;i<26;i++) t[0][i] = s[0][i];
    
    int a = rand()%26;
    int b = rand()%26;
    
    while(a==b) b = rand()%26;
    std::swap(t[0][a], t[0][b]);

}

void simulatedAnnealing(char key[][27] , double &bestFitness, int n) {

    // Setting initial parameters. 26+1 accounts for the \0
    char s[1][26+1] = {"abcdefghijklmnopqrstuvwxyz"}; 
    char t[1][26+1] = {"abcdefghijklmnopqrstuvwxyz"}; 
    char pMsg[n];
    
    std::random_shuffle( s[0], s[0]+(26*sizeof(char)) );
    greedy(s, 0, n);
    decrypt(0, s, pMsg, n);
    double currBest = cost(pMsg,n);
    
    double T = 100;
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

            } 
            else {
                
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
        } // Trials loop

        // Cooling schedule
        equilibrium = (changes*1.0)/(trials*1.0);
        double k = ((equilibrium>1)?equilibrium/100.0:equilibrium)*0.25;
        T *= 0.60 + k; 
    
    }

    #pragma unroll
    for(int i=0;i<26;i++) key[0][i] = best[0][i];
    decrypt(0, key, pMsg, n);
    bestFitness = cost(pMsg,n);
}


int main(int argc, char **argv) {
    
    srand (time(NULL));

    int n = 0;                          // Number of chars in text
    int size = atoi(argv[2]);           // GA population size
    int generations = atoi(argv[3]); 
    int method = atoi(argv[4]);
    
    getDataset(argv[1], n);
    
    // Generate random original key
    std::random_shuffle( originalKey[0], originalKey[0]+(26*sizeof(char)) );
   
    // Initializing solution variables     
    std::cerr << "Normal text score: " << cost(text, n) << "\n";
    encrypt(originalKey, originalCipher, n);
    std::cerr << "Encrypted score: " << cost(originalCipher, n) << "\n\n";

    // Initializing random population
    double bestFitness = std::numeric_limits<double>::max();
    char bestKey[1][26+1] = {"xxxxxxxxxxxxxxxxxxxxxxxxxx"};
    
    if(method==1) simulatedAnnealing(bestKey, bestFitness, n);
    else if(method==0) geneticAlgorithm(bestKey, bestFitness, n, generations, size);
    else {

        std::cout << "Wrong method provided (" << argv[4] << "). Aborting\n";
        exit(EXIT_FAILURE);

    }

    decrypt(0, bestKey, message, n);
    std::cout << "\n" << argv[1] << "\n";
    stats(message, bestKey, originalKey, n);

}
