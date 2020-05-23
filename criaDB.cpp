#include <bits/stdc++.h>
using namespace std;

double bigram[26][26] = {{0.0}};
double trigram[26][26][26] = {{{0.0}}};
double fourgram[26][26][26][26] = {{{{0.0}}}};
double fivegram[26][26][26][26][26] = {{{{{0.0}}}}};

char text[100000000];

int alphaPos(char c) { return ( (int) c % 32 ) - 1; } // Converts both upper and lower case chars (Stack overflow courtesy)

void buildNgramsFrom(const vector<pair<string, long long int> > &txt, long long int total, int ngram) {

    if(ngram==5) {

        for(int i=0; i<txt.size(); i++) {

            int c1 = alphaPos(txt[i].first[0]);
            int c2 = alphaPos(txt[i].first[1]);
            int c3 = alphaPos(txt[i].first[2]);
            int c4 = alphaPos(txt[i].first[3]);
            int c5 = alphaPos(txt[i].first[4]);

            fivegram[c1][c2][c3][c4][c5] = txt[i].second/(total*1.0) * 100.0;

        }

        for(int i=0; i<26; i++) {
            for(int j=0; j<26; j++)
                for(int k=0; k<26; k++)
                    for(int l=0; l<26; l++){
                        for(int m=0; m<26; m++)
                            printf("%lf ", fivegram[i][j][k][l][m]);
                    cout << endl;
                    }
        }
    
    }

    else if(ngram==4){
        for(int i=0; i<txt.size(); i++) {

        int c1 = alphaPos(txt[i].first[0]);
        int c2 = alphaPos(txt[i].first[1]);
        int c3 = alphaPos(txt[i].first[2]);
        int c4 = alphaPos(txt[i].first[3]);
        
        fourgram[c1][c2][c3][c4] = txt[i].second/(total*1.0) * 100.0;

        }

        for(int i=0; i<26; i++) {
            for(int j=0; j<26; j++)
                for(int k=0; k<26; k++){
                    for(int l=0; l<26; l++)
                        printf("%lf ", fourgram[i][j][k][l]);
                cout << endl;
            }
        }
    }
    else if(ngram==3) {
        
        for(int i=0; i<txt.size(); i++) {

        int c1 = alphaPos(txt[i].first[0]);
        int c2 = alphaPos(txt[i].first[1]);
        int c3 = alphaPos(txt[i].first[2]);
        
        trigram[c1][c2][c3] = txt[i].second/(total*1.0) * 100.0;

        }

        for(int i=0; i<26; i++) {
            for(int j=0; j<26; j++){
                for(int k=0; k<26; k++)
                    printf("%lf ", trigram[i][j][k]);
            cout << endl;
            }
        }

    }
    else if(ngram==2) {

        for(int i=0; i<txt.size(); i++) {
        
            int c1 = alphaPos(txt[i].first[0]);
            int c2 = alphaPos(txt[i].first[1]);
            
            bigram[c1][c2] = txt[i].second/(total*1.0) * 100.0;

        }

        for(int i=0; i<26; i++) {
          for(int j=0; j<26; j++)
            printf("%lf ", bigram[i][j]);
          cout << endl;
        }
    }

}

int main(int argc, char **argv) {

    
    
    int ngram = atoi(argv[1]);
     //Reading bigrams and storing its values....outdated since we preprocess stuff now
      string line;
      getline(cin, line);
      vector<pair<string, long long int>> txt;
      long long int total = 0;
      long long int n = 0;
      string s;
      
      while(cin >> s >> n) {
    
        total += n;
        txt.push_back({s, n});
    
    }

    cerr << "Frequencies total " << total << endl;

    
    buildNgramsFrom(txt, total, ngram);

  


}