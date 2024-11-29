// OBS: IMPRIMIR TODOS DO CARGO SELECIONADO IN ORDER!!
// LEMBRAR DE ALTERAR O VALOR DO NUMERO NO COMEÇO DO DADOS!!!!!!!

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_avl.h"

void limparTela()
{
    system("clear");
}

void esperarUsuario()
{
    printf("Pressione qualquer tecla para continuar...");
    getchar();
}

void removerEspacos(char *str)
{
    int len = strlen(str);
    while (len > 0 && str[len - 1] == ' ')
    {
        str[--len] = '\0';
    }
}

void adicionarEspacos(char *str, int tamanho)
{
    int len = strlen(str);
    while (len < tamanho - 1)
    {
        str[len++] = ' ';
    }
    str[len] = '\0';
}

void atualizarArquivoDadosComEspacos(No *raiz, FILE *arquivo)
{
    if (raiz != NULL)
    {
        atualizarArquivoDadosComEspacos(raiz->esquerda, arquivo);
        char nome[40], cargo[25];
        strcpy(nome, raiz->nome);
        strcpy(cargo, raiz->cargo);
        adicionarEspacos(nome, 40);
        adicionarEspacos(cargo, 25);
        fprintf(arquivo, "%4d %-40s %2d %-25s %8.2f\n", raiz->chave, nome, raiz->idade, cargo, raiz->salario);
        atualizarArquivoDadosComEspacos(raiz->direita, arquivo);
    }
}

void print2DUtil(struct No *raiz, int espaco)
{
    if (raiz == NULL)
        return;

    espaco += 10;

    print2DUtil(raiz->direita, espaco);

    printf("\n");
    for (int i = 10; i < espaco; i++)
        printf(" ");
    printf("%d\n", raiz->chave);

    print2DUtil(raiz->esquerda, espaco);
}

void print2D(struct No *raiz)
{
    print2DUtil(raiz, 0);
}

void criarFuncionario(No *raiz)
{
    int matricula, idade;
    char nome[40];
    char cargo[25];
    float salario;

    limparTela();
    printf("\tNOVO FUNCIONÁRIO:");
    printf("\nInsira o código da matrícula: ");
    scanf("%d", &matricula);
    getchar();

    printf("Insira o nome (Limitado em 40 caracteres): ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Qual é a idade de %s? ", nome);
    scanf("%d", &idade);
    getchar();

    printf("Insira o cargo (Limitado em 25 caracteres): ");
    fgets(cargo, sizeof(cargo), stdin);
    cargo[strcspn(cargo, "\n")] = '\0';

    printf("Qual é o salário de %s? ", nome);
    scanf("%f", &salario);

    raiz = inserir(raiz, matricula, nome, idade, cargo, salario);
}

void atualizarFuncionario(No *raiz)
{
    limparTela();
    printf("\tATUALIZAR USUÁRIO: ");

    int matriculaAlterar;

    // Solicitar a matrícula do usuário a ser alterado
    printf("\nInsira o número da matrícula do usuário a ser alterado: ");
    scanf("%d", &matriculaAlterar);
    getchar(); // Limpar o buffer do teclado

    // Buscar o nó na árvore
    No *usuario = buscar(raiz, matriculaAlterar);
    if (usuario == NULL)
    {
        printf("\nNão foi possível encontrar nenhum usuário com a matrícula %d. \nO processo foi cancelado.", matriculaAlterar);
        esperarUsuario();
        return;
    }

    limparTela();
    printf("\tATUALIZAR USUÁRIO %d:\n", matriculaAlterar);

    // Solicitar qual dado o usuário deseja alterar
    printf("\nQual é o dado que quer alterar? ");
    printf("\n1. Nome (%s)\n2. Idade (%d)\n3. Cargo (%s)\n4. Salário (%.2f)\n\nInsira a opção: ", usuario->nome, usuario->idade, usuario->cargo, usuario->salario);

    int escolha;
    scanf("%d", &escolha);
    getchar(); // Limpar o buffer do teclado

    char nome[40];
    char cargo[25];
    int idade;
    float salario;

    switch (escolha)
    {
    case 1:
        limparTela();
        printf("\tATUALIZAR USUÁRIO %d - TROCAR NOME\n", matriculaAlterar);
        printf("\n\nInsira o novo nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = '\0'; // Remover a nova linha
        atualizarNo(raiz, matriculaAlterar, nome, usuario->idade, usuario->cargo, usuario->salario);
        break;
    case 2:
        limparTela();
        printf("\tATUALIZAR USUÁRIO %d - TROCAR IDADE\n", matriculaAlterar);
        printf("\n\nInsira a nova idade: ");
        scanf("%d", &idade);
        getchar(); // Limpar o buffer do teclado
        atualizarNo(raiz, matriculaAlterar, usuario->nome, idade, usuario->cargo, usuario->salario);
        break;
    case 3:
        limparTela();
        printf("\tATUALIZAR USUÁRIO %d - TROCAR CARGO\n", matriculaAlterar);
        printf("\n\nInsira o novo cargo: ");
        fgets(cargo, sizeof(cargo), stdin);
        cargo[strcspn(cargo, "\n")] = '\0'; // Remover a nova linha
        atualizarNo(raiz, matriculaAlterar, usuario->nome, usuario->idade, cargo, usuario->salario);
        break;
    case 4:
        limparTela();
        printf("\tATUALIZAR USUÁRIO %d - TROCAR SALÁRIO\n", matriculaAlterar);
        printf("\n\nInsira o novo salário: ");
        scanf("%f", &salario);
        getchar(); // Limpar o buffer do teclado
        atualizarNo(raiz, matriculaAlterar, usuario->nome, usuario->idade, usuario->cargo, salario);
        break;
    default:
        limparTela();
        printf("\tATUALIZAR USUÁRIO %d - OPÇÃO INVÁLIDA\n", matriculaAlterar);
        printf("\n\nOperação desconhecida.");
        break;
    }

    esperarUsuario();
}

void removerFuncionario(No *raiz)
{
    limparTela();
    printf("\tREMOVER USUÁRIO");
    printf("\nInsira a matrícula do usuário a ser removido: ");

    int matricula;
    scanf("%d", &matricula);
    getchar();

    deletarNo(raiz, matricula);
    printf("Usuário removido com sucesso.");
    esperarUsuario();

    return;
}

void buscarFuncionario(No *raiz)
{
    limparTela();
    printf("\tBUSCAR USUÁRIO");
    printf("\nInsira o número da matrícula do usuário que deseja buscar: ");

    int matricula;
    scanf("%d", &matricula);
    getchar();

    limparTela();

    No *encontrado = buscar(raiz, matricula);
    if (encontrado != NULL)
    {
        printf("\tUSUÁRIO ENCONTRADO\nMatrícula: %d\nNome: %s\nIdade: %d\nCargo: %s\nSalário: %.2f\n", encontrado->chave, encontrado->nome, encontrado->idade, encontrado->cargo, encontrado->salario);
        esperarUsuario();
    }
    else
    {
        printf("\nFuncionário com matrícula %d não encontrado.\n", matricula);
        esperarUsuario();
    }

    return;
}

void imprimirNovoVelho(No *raiz)
{
    limparTela();

    printf("\tIMPRIMIR FUNCIONÁRIO MAIS NOVO E MAIS VELHO");
    No *maisNovo = NULL;
    No *maisVelho = NULL;
    encontrarMaisNovoMaisVelho(raiz, &maisNovo, &maisVelho);

    if (maisNovo != NULL)
    {
        printf("\nFuncionário mais novo:\n");
        printf("Matrícula: %d\nNome: %s\nIdade: %d\nCargo: %s\nSalário: %.2f\n", maisNovo->chave, maisNovo->nome, maisNovo->idade, maisNovo->cargo, maisNovo->salario);
    }
    else
    {
        printf("\nFuncionário mais novo não encontrado.\n");
    }
    if (maisVelho != NULL)
    {
        printf("\nFuncionário mais velho:\n");
        printf("\nMatrícula: %d\nNome: %s\nIdade: %d\nCargo: %s\nSalário: %.2f\n", maisVelho->chave, maisVelho->nome, maisVelho->idade, maisVelho->cargo, maisVelho->salario);
    }
    else
    {
        printf("\nFuncionário mais velho não encontrado.");
    }

    getchar();
    esperarUsuario();
    return;
}

void imprimirPorCargoFunc(No *raiz)
{
    limparTela();
    getchar();
    printf("\tBUSCA POR CARGO");

    char cargoBusca[25];
    printf("\nDigite o cargo para buscar dentre todos os funcionários: ");
    fgets(cargoBusca, sizeof(cargoBusca), stdin);
    cargoBusca[strcspn(cargoBusca, "\n")] = '\0';

    printf("\nFuncionários com o cargo '%s':\n", cargoBusca);
    imprimirPorCargo(raiz, cargoBusca);

    printf("\n");

    esperarUsuario();
}

void imprimirTodos(No *raiz)
{
    limparTela();
    printf("\tIMPRESSÃO DE TODOS OS USUÁRIOS");
    printf("\nInsira a ordem que deseja imprimir:\n1. Pré-Order\n2. In-Order\n3. Pós-Order\n\nSelecione a opção: ");

    int escolha;
    scanf("%d", &escolha);
    getchar();

    switch (escolha)
    {
    case 1:
        limparTela();
        printf("\tIMPRESSÃO DE TODOS OS USUÁRIOS - PRÉ ORDER\n\n");
        percursoPreOrder(raiz);
        break;
    case 2:
        limparTela();
        printf("\tIMPRESSÃO DE TODOS OS USUÁRIOS - IN ORDER\n\n");
        percursoInOrder(raiz);
        break;
    case 3:
        limparTela();
        printf("\tIMPRESSÃO DE TODOS OS USUÁRIOS - PÓS ORDER\n\n");
        percursoPosOrder(raiz);
        break;
    default:
        break;
    }

    esperarUsuario();
    return;
}

void imprimirArvore(No *raiz)
{
    limparTela();
    printf("\nIMPRESSÃO DA ÁRVORE");

    print2D(raiz);
    getchar();
    esperarUsuario();
    return;
}

void encerrar(No *raiz, int contadorUsuarios)
{
    limparTela();
    FILE *arquivo;

    arquivo = fopen("Dados.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo Dados.txt para escrita\n");
        liberarArvore(raiz);
        return;
    }

    fprintf(arquivo, "%d\n", contadorUsuarios);
    printf("\nArquivo salvo com sucesso.");

    atualizarArquivoDados(raiz, arquivo);
    printf("\nArquivo atualizado com sucesso.");
    fclose(arquivo);
    printf("\nArquivo fechado com sucesso.");

    liberarArvore(raiz);
    printf("\nÁrvore liberada com sucesso.");

    printf("\n\nSistema finalizado e dados atualizados com sucesso.\nTodas as operações obtiveram êxito.\n");
    exit(0);
}

void mostrarMenu(No *raiz, int contadorUsuarios)
{

    while (1)
    {
        limparTela();
        printf("\n\tMENU PRINCIPAL");
        printf("\n1. Inserir novo funcionário");
        printf("\n2. Atualizar / Alterar dados de um funcionário");
        printf("\n3. Remover um funcionário");
        printf("\n4. Buscar funcionário");
        printf("\n5. Imprimir dados do funcionário mais novo e mais velho");
        printf("\n6. Imprimir todos usuários por cargo");
        printf("\n7. Imprimir todos os usuários");
        printf("\n8. Imprimir estrutura da árvore balanceada");
        printf("\n0. Salvar e encerrar");
        printf("\n\nEscolha uma operação: ");

        int escolha;
        scanf("%d", &escolha);

        switch (escolha)
        {
        case 1:
            criarFuncionario(raiz);
            break;
        case 2:
            atualizarFuncionario(raiz);
            break;
        case 3:
            removerFuncionario(raiz);
            break;
        case 4:
            buscarFuncionario(raiz);
            break;
        case 5:
            imprimirNovoVelho(raiz);
            break;
        case 6:
            imprimirPorCargoFunc(raiz);
            break;
        case 7:
            imprimirTodos(raiz);
            break;
        case 8:
            imprimirArvore(raiz);
            break;
        case 0:
            encerrar(raiz, contadorUsuarios);
            break;
        default:
            break;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Portuguese_Brasil");

    No *raiz = NULL;
    FILE *arquivo;
    char linha[150];
    char nome[40], cargo[25];
    int matricula, idade;
    float salario;

    arquivo = fopen("Dados.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo Dados.txt\n");
        return 1;
    }

    printf("\n\tImportando os usuários do arquivo selecionado...\n");

    fgets(linha, sizeof(linha), arquivo);

    int contadorUsuarios = 0;
    while (fgets(linha, sizeof(linha), arquivo))
    {
        if (sscanf(linha, "%d %39[^\t] %d %24[^\t] %f", &matricula, nome, &idade, cargo, &salario) == 5)
        {
            removerEspacos(nome);
            removerEspacos(cargo);
            raiz = inserir(raiz, matricula, nome, idade, cargo, salario);
            contadorUsuarios++;
        }
    }

    fclose(arquivo);

    percursoInOrder(raiz);
    printf("\n-> Usuários importados com sucesso.\n\n");

    esperarUsuario();
    mostrarMenu(raiz, contadorUsuarios);

    percursoInOrder(raiz);

    arquivo = fopen("Dados.txt", "w");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo Dados.txt para escrita\n");
        liberarArvore(raiz);
        return 1;
    }
    fprintf(arquivo, "%d\n", contadorUsuarios);
    atualizarArquivoDados(raiz, arquivo);
    fclose(arquivo);

    // Liberar a memória da árvore
    liberarArvore(raiz);

    printf("\nSistema finalizado e dados atualizados com sucesso.\n");

    return 0;
}