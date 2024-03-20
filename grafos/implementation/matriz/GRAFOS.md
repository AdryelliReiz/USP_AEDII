# Anotações sobre as aulas de GRAFOS

## Gafos completos

- Um grafo completo é u grafo não direcionado no qual todos os pares de vértices são adjacentes.
- Possui (|V|² - |V|)/2 = |V|(|V| - 1) / arestas, pois do total de |V|² pares possíveis de vértices devemos subtrair |V| self-loops e dividir por 2 (cada aresta ligando dois vértices é contada duas vezes).
- O número total de grafos diferentes com |V| vértices é 2^|V|(|V| - 1) /2 (Número de maneiras diferentes de escolher um subconjunto a partir de |V|(|V| - 1) / 2 possíveis arestas).

## Para compilar tudo (por enquanto...)

```bash
gcc -c grafo_matrizadj.c

gcc -c testa_grafo.c

gcc -o testa_grafo.exe grafo_matrizadj.o testa_grafo.o
```

## Makefile - EX

all: myprogram.exe
myprogram.exe: part1.o part2.o … partn.o main.o
```bash
gcc -o myprogram.exe part1.o part2.o … partn.o main.o
```

part1.o: part1.c part1.h
```bash
gcc -c part1.c # gera part1.o
```

part2.o: part2.c part2.h
```bash
gcc -c part2.c # gera part2.o
```

partn.o: partn.c partn.h
```bash
gcc -c partn.c # gera partn.o
```

main.o: main.c
```bash
gcc -c main.c # gera main.o
```

## Nosso Makefile

Executando (no linux, via linha de comando):
```bash
./testa_grafo_matriz.exe
```

Ou redirecionando a entrada de um arquivo (com símbolo “<”):
```bash
./testa_grafo_matriz.exe < entrada_teste.txt
```

Ou redirecionando também a saída para um arquivo (com símbolo “>”):
```bash
./testa_grafo_matriz.exe < entrada_teste.txt > saida.txt
```

Ou redirecionando também a saída de erro para um outro arquivo (com símbolo “2>”):
```bash
./testa_grafo_matriz.exe < entrada_teste.txt > saida.txt 2> erro.txt
```

## Matriz de adjacência - Reflexões

Essa representação por matriz adjancência é sempre eficiente?

```
inicializaGrafo: O(V²)
imprimeGrafo: O(V²)
insereAresta: O(1)
existeAresta: O(1)
removeAresta:  O(1)
listaAdjVazia: O(V)
proxListaAdj: O(V)
liberaGrafo: O(1)
```

Acesso instantâneo a uma aresta (tempo constante) - consulta, inserção e remoção.
Para grafos densos OK!!!

Mesmo que o grafo tenha poucas arestas (esparso):
- Utilização da lista de adjacência em O(V)
- Espaço Omega(V²)

## Referências

ZIVIANI, N. Projetos de Algoritmos - com implementações em Pascal e C. 3a ed. revista e ampliada Cengage Learning, 2011. (cap 7)