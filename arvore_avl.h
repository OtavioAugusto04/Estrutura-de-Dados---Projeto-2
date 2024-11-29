#ifndef ARVORE_AVL_H
#define ARVORE_AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definindo a estrutura do nó de uma árvore AVL
typedef struct No
{
    int chave;      // Matrícula - valor inteiro de 4 dígitos
    char nome[40];  // Nome completo do funcionário (até 40 caracteres)
    int idade;      // Idade do funcionário
    char cargo[25]; // Cargo do funcionário (até 25 caracteres)
    float salario;  // Salário do funcionário
    struct No *esquerda;
    struct No *direita;
    int altura;
} No;

// Protótipos das funções
int altura(No *N);
int maximo(int a, int b);
No *novoNo(int chave, char nome[], int idade, char cargo[], float salario);
No *rotacionarDireita(No *y);
No *rotacionarEsquerda(No *x);
int obterBalanceamento(No *N);
No *inserir(No *no, int chave, char nome[], int idade, char cargo[], float salario);
void percursoInOrder(No *raiz);
No *buscar(No *raiz, int chave);
void atualizarNo(No *raiz, int matricula, char nome[], int idade, char cargo[], float salario);
No *deletarNo(No *raiz, int chave);
void encontrarMaisNovoMaisVelho(No *no, No **maisNovo, No **maisVelho);

// avl_tree.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_avl.h"

// Função para obter a altura de um nó
int altura(No *N)
{
    if (N == NULL)
        return 0;
    return N->altura;
}

// Função para obter o valor máximo entre dois inteiros
int maximo(int a, int b)
{
    return (a > b) ? a : b;
}

// Função para alocar um novo nó com uma chave específica e outros dados
No *novoNo(int chave, char nome[], int idade, char cargo[], float salario)
{
    No *no = (No *)malloc(sizeof(No));
    no->chave = chave;
    strcpy(no->nome, nome);
    no->idade = idade;
    strcpy(no->cargo, cargo);
    no->salario = salario;
    no->esquerda = NULL;
    no->direita = NULL;
    no->altura = 1; // Novo nó é inicialmente adicionado como uma folha
    return no;
}

// Função para realizar uma rotação à direita
No *rotacionarDireita(No *y)
{
    No *x = y->esquerda;
    No *T2 = x->direita;

    // Realizando a rotação
    x->direita = y;
    y->esquerda = T2;

    // Atualizando as alturas
    y->altura = maximo(altura(y->esquerda), altura(y->direita)) + 1;
    x->altura = maximo(altura(x->esquerda), altura(x->direita)) + 1;

    // Retornando a nova raiz
    return x;
}

// Função para realizar uma rotação à esquerda
No *rotacionarEsquerda(No *x)
{
    No *y = x->direita;
    No *T2 = y->esquerda;

    // Realizando a rotação
    y->esquerda = x;
    x->direita = T2;

    // Atualizando as alturas
    x->altura = maximo(altura(x->esquerda), altura(x->direita)) + 1;
    y->altura = maximo(altura(y->esquerda), altura(y->direita)) + 1;

    // Retornando a nova raiz
    return y;
}

// Função para obter o fator de balanceamento de um nó
int obterBalanceamento(No *N)
{
    if (N == NULL)
        return 0;
    return altura(N->esquerda) - altura(N->direita);
}

// Função para inserir um nó na árvore AVL
No *inserir(No *no, int chave, char nome[], int idade, char cargo[], float salario)
{
    // 1. Realizar a inserção normal de uma árvore binária de busca
    if (no == NULL)
        return novoNo(chave, nome, idade, cargo, salario);

    if (chave < no->chave)
        no->esquerda = inserir(no->esquerda, chave, nome, idade, cargo, salario);
    else if (chave > no->chave)
        no->direita = inserir(no->direita, chave, nome, idade, cargo, salario);
    else // Chaves iguais não são permitidas na árvore AVL
        return no;

    // 2. Atualizar a altura deste nó ancestral
    no->altura = 1 + maximo(altura(no->esquerda), altura(no->direita));

    // 3. Obter o fator de balanceamento deste nó ancestral para verificar se ele ficou desbalanceado
    int balance = obterBalanceamento(no);

    // Se o nó ficar desbalanceado, existem 4 casos a serem considerados

    // Caso Esquerda-Esquerda
    if (balance > 1 && chave < no->esquerda->chave)
        return rotacionarDireita(no);

    // Caso Direita-Direita
    if (balance < -1 && chave > no->direita->chave)
        return rotacionarEsquerda(no);

    // Caso Esquerda-Direita
    if (balance > 1 && chave > no->esquerda->chave)
    {
        no->esquerda = rotacionarEsquerda(no->esquerda);
        return rotacionarDireita(no);
    }

    // Caso Direita-Esquerda
    if (balance < -1 && chave < no->direita->chave)
    {
        no->direita = rotacionarDireita(no->direita);
        return rotacionarEsquerda(no);
    }

    // Retornar o ponteiro do nó (inalterado)
    return no;
}

// Função para remover um nó da árvore AVL
No *minimoNo(No *no)
{
    No *atual = no;

    // Encontrando o nó mais à esquerda
    while (atual->esquerda != NULL)
        atual = atual->esquerda;

    return atual;
}

No *deletarNo(No *raiz, int chave)
{
    // 1. Realizar a remoção normal de uma árvore binária de busca
    if (raiz == NULL)
        return raiz;

    // Se a chave a ser removida for menor que a chave da raiz, então está no subárvore esquerda
    if (chave < raiz->chave)
        raiz->esquerda = deletarNo(raiz->esquerda, chave);

    // Se a chave a ser removida for maior que a chave da raiz, então está no subárvore direita
    else if (chave > raiz->chave)
        raiz->direita = deletarNo(raiz->direita, chave);

    // Se a chave for igual à chave da raiz, então este é o nó a ser removido
    else
    {
        // Nó com apenas um filho ou nenhum
        if ((raiz->esquerda == NULL) || (raiz->direita == NULL))
        {
            No *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;

            // Nenhum filho
            if (temp == NULL)
            {
                temp = raiz;
                raiz = NULL;
            }
            else               // Um filho
                *raiz = *temp; // Cópia do conteúdo do filho não vazio

            free(temp);
        }
        else
        {
            // Nó com dois filhos: obter o sucessor em ordem (menor na subárvore direita)
            No *temp = minimoNo(raiz->direita);

            // Copiar os dados do sucessor para este nó
            raiz->chave = temp->chave;
            strcpy(raiz->nome, temp->nome);
            raiz->idade = temp->idade;
            strcpy(raiz->cargo, temp->cargo);
            raiz->salario = temp->salario;

            // Remover o sucessor
            raiz->direita = deletarNo(raiz->direita, temp->chave);
        }
    }

    // Se a árvore tinha apenas um nó, então retorne
    if (raiz == NULL)
        return raiz;

    // 2. Atualizar a altura do nó atual
    raiz->altura = 1 + maximo(altura(raiz->esquerda), altura(raiz->direita));

    // 3. Obter o fator de balanceamento deste nó ancestral para verificar se ele ficou desbalanceado
    int balance = obterBalanceamento(raiz);

    // Se o nó ficar desbalanceado, existem 4 casos a serem considerados

    // Caso Esquerda-Esquerda
    if (balance > 1 && obterBalanceamento(raiz->esquerda) >= 0)
        return rotacionarDireita(raiz);

    // Caso Esquerda-Direita
    if (balance > 1 && obterBalanceamento(raiz->esquerda) < 0)
    {
        raiz->esquerda = rotacionarEsquerda(raiz->esquerda);
        return rotacionarDireita(raiz);
    }

    // Caso Direita-Direita
    if (balance < -1 && obterBalanceamento(raiz->direita) <= 0)
        return rotacionarEsquerda(raiz);

    // Caso Direita-Esquerda
    if (balance < -1 && obterBalanceamento(raiz->direita) > 0)
    {
        raiz->direita = rotacionarDireita(raiz->direita);
        return rotacionarEsquerda(raiz);
    }

    return raiz;
}

void percursoPreOrder(No *raiz)
{
    if (raiz != NULL)
    {
        printf("Matrícula: %d, Nome: %s, Idade: %d, Cargo: %s, Salário: %.2f\n", raiz->chave, raiz->nome, raiz->idade, raiz->cargo, raiz->salario);
        percursoInOrder(raiz->esquerda);
        percursoInOrder(raiz->direita);
    }
}

void percursoInOrder(No *raiz)
{
    if (raiz != NULL)
    {
        percursoInOrder(raiz->esquerda);
        printf("Matrícula: %d, Nome: %s, Idade: %d, Cargo: %s, Salário: %.2f\n", raiz->chave, raiz->nome, raiz->idade, raiz->cargo, raiz->salario);
        percursoInOrder(raiz->direita);
    }
}

void percursoPosOrder(No *raiz)
{
    if (raiz != NULL)
    {
        percursoInOrder(raiz->esquerda);
        percursoInOrder(raiz->direita);
        printf("Matrícula: %d, Nome: %s, Idade: %d, Cargo: %s, Salário: %.2f\n", raiz->chave, raiz->nome, raiz->idade, raiz->cargo, raiz->salario);
    }
}

// Função para buscar um nó na árvore AVL por matrícula
No *buscar(No *raiz, int chave)
{
    if (raiz == NULL || raiz->chave == chave)
        return raiz;
    if (chave < raiz->chave)
        return buscar(raiz->esquerda, chave);
    return buscar(raiz->direita, chave);
}

// Função para atualizar os dados de um nó na árvore AVL
void atualizarNo(No *raiz, int matricula, char nome[], int idade, char cargo[], float salario)
{
    No *no = buscar(raiz, matricula);
    if (no != NULL)
    {
        // Atualizando os dados do nó encontrado
        strcpy(no->nome, nome);
        no->idade = idade;
        strcpy(no->cargo, cargo);
        no->salario = salario;
        printf("Dados atualizados para a matrícula %d\n", matricula);
    }
    else
    {
        printf("Funcionário com matrícula %d não encontrado.\n", matricula);
    }
}

// Função para encontrar o funcionário mais novo e o mais velho na árvore AVL
void encontrarMaisNovoMaisVelho(No *no, No **maisNovo, No **maisVelho)
{
    if (no != NULL)
    {
        encontrarMaisNovoMaisVelho(no->esquerda, maisNovo, maisVelho);
        if (*maisNovo == NULL || no->idade < (*maisNovo)->idade)
        {
            *maisNovo = no;
        }
        if (*maisVelho == NULL || no->idade > (*maisVelho)->idade)
        {
            *maisVelho = no;
        }
        encontrarMaisNovoMaisVelho(no->direita, maisNovo, maisVelho);
    }
}

// Função para liberar a memória da árvore AVL
void liberarArvore(No *raiz)
{
    if (raiz != NULL)
    {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// Função para atualizar o arquivo de dados com os funcionários atuais na árvore AVL
void atualizarArquivoDados(No *raiz, FILE *arquivo)
{
    if (raiz != NULL)
    {
        atualizarArquivoDados(raiz->esquerda, arquivo);
        fprintf(arquivo, "%4d %-40s %2d %-25s %8.2f\n", raiz->chave, raiz->nome, raiz->idade, raiz->cargo, raiz->salario);
        atualizarArquivoDados(raiz->direita, arquivo);
    }
}

// Função para imprimir todos os funcionários de um determinado cargo
void imprimirPorCargo(No *no, char *cargo)
{
    if (no != NULL)
    {
        imprimirPorCargo(no->esquerda, cargo);
        if (strcmp(no->cargo, cargo) == 0)
        {
            printf("Matrícula: %d, Nome: %s, Idade: %d, Cargo: %s, Salário: %.2f\n", no->chave, no->nome, no->idade, no->cargo, no->salario);
        }
        imprimirPorCargo(no->direita, cargo);
    }
}

#endif // AVL_TREE_H
