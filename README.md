# Attacking the Substitution Cipher 

### Compiling & Running
```bash
~$ g++ -O3 -fopenmp cipherAttack.cpp -o cipherAttack
~$ ./cipherAttack 1 2 3 4
```
### Parameters
**1** - path/to/file\
**2** - Population size for the Genetic Algorithm [1 ~ MAX\_INT ]\
**3** - Number of generations used in the Genetic Algorithm [1 ~ MAX\_INT ]\
**4** - 0 for Genetic Algorithm and 1 for Simulated Annealing

Modifying the parameters for each instance - plaintext provided - allows for a fine tunning that is better than using the same parameters for
all the inputs, since each text is different - number of unique words, average word length, total characters, etc. The results vary alot and it's hard to reach a consistent similarity rate - how close to the original text the current solution is - in some texts. Remmember that while the substitution cipher is a relatively simple method, it has a solution space of 26!. This is equal to 4.0329146e26 of keys, which would take ≈ 1.2788e11 years to investigate assuming the worst case scenario. This is equivalent to restarting  the universe 9 times, given that it's current estimated age is of 13,7 billion years.

## Problem Description
The substitution cipher, also known as the Caesar's cipher, is a method of transforming a given string of characters into another one, in hopes
of making any attempt to read the string fail if you don't have the original key - supposedly any person who might intercept the message during a transmission from A to B in an insecure channel. This key serves as a template for how the substitutions are made, and represents a mapping of each letter of a given alphabet into another letter of the same alphabet. Even tough this method is outdated, it is still used as part of modern ciphers, which means that understanding it's pros ans cons may provide insights on how to improve/break current encryption methods.

## N-grams
For this project, I used Peter's Norvig fusion tables with [ngram](https://en.wikipedia.org/wiki/N-gram) frequencies up until the 5gram - more than that significantly increased the processing overhead. Google has discontinued fusion tables but luckily I kept backup files of 2gram ~  9gram. These frequency tables were gathered using Google Books corpus by Norvig himself. Due to GitHub's native size limit I can't upload them here, but for all intents and purposes if you need them feel free to contact me.

<p align="center">
<img src="https://raw.githubusercontent.com/lucas-t-reis/cipher-attack/master/assets/subscipher.png" alt="Image taken from Wikipedia's substitution cipher page">
</p>

## Problem Definition
- **A** - set of alphabets
- **K** - set of possible keys
- *m* - message
- *c* - ciphertext
- *k* - key
- *d*<sub>x</sub> and *k*<sub>x</sub> returns the corpus and the solution ngram frequency of the xth alphabet character respectively, for each i,j,k,l or m
- The aforementioned rule is analogous for upper dimensions - a.k.a *d*<sub>ij</sub>, *d*<sub>ijk</sub>... 
- **Encrypt<sub>*k*</sub>**(*m*) = *c*
- **Decrypt<sub>*k*</sub>**(*c*) = *m*

## Cost Function
<p align="center">
<img src="https://raw.githubusercontent.com/lucas-t-reis/cipher-attack/master/assets/cost.svg" alt="Substitution Cipher Cost Function">
</p>
