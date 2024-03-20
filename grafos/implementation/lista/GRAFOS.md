# Anotações sobre as aulas de GRAFOS

## Vamos definir um tipo Apontador

Apontador é algo (um tipo coerente com a implementação por matriz ou lista de adjacência) que pode ser usado para identidicar um vértice adjacente:
- Matriz de adjacência: int
- Lista de adjacência: Aresta*

## Makefile

testa_lista: grafo_listaadj.o testa_hrago.o
```bash
gcc -o testa_grafo_lista.exe grafo_listaadj.o testa_grafo.o
```

grafo_listaadj.o: grafo_listaadj.c grafo_listaadj.h
```bash
gcc -c grafo_listaadj.c
```

testa_grafo.o: testa_grafo.c grafo_matrizadj.h grafo_listaadj.h
```bash
gcc -c testa_grafo.c
```