# Documentation in progress
Come back in a few days :P

Parâmetros
./a.out 1 2 3 4
1 - arquivo.txt
2 - tamanho da população para o GA
3 - nº de gerações para o GA
4 - 0 para GA e 1 para SA

A saída é no formato:
-Score do texto original antes de ser encriptado.
-Score do texto depois de ser encriptado com uma chave aleatória.
-arquivo.txt #elementos iguais da chave# similaridade

Dentro do código existe uma função @stats que imprime a solução encontrada
de forma verbosa. Se quiser conferir é só descomentar ela.

Modificar os parâmetros da população pra cada tipo de isntância muda bem o
resultado. Tem instância que fica boa com população grande e gerações pequenas, tem instância que fica boa com temperatura alta...
Recomendo rodar umas 5 ~ 10 vezes por instâncias para aparecer um numero bom de similaridade. Tem caso que é horroroso, mas na maior parte deles consegue achar um resultado satisfatório!
