#include <stdio.h>   // printf, scanf, fopen, fprintf, fscanf, fclose, remove, rename
#include <string.h>  // strcmp, strstr
#include <ctype.h>   // tolower

#define MAX 100                 // quantidade maxima de produtos no vetor
#define ARQUIVO "produtos.txt"  // arquivo principal de dados
#define TEMPORARIO "temp.txt"   // arquivo temporario (alterar/excluir)

// Estrutura do produto (campos de tamanho limitado: aumente se precisar)
typedef struct {
    int id;                 // identificador (gerado automaticamente)
    char produto[50];       // nome do produto
    char categoria[50];     // categoria do produto
    float preco;            // preco em reais
    int quantidade;         // quantidade em estoque
    char fornecedor[50];    // nome do fornecedor
} Produto;

// ---------- Funcoes auxiliares ----------

// Le um produto de uma linha do arquivo (retorna 1 se leu os 6 campos)
int lerProduto(FILE *f, Produto *p) {
    return fscanf(f, "%d;%49[^;];%49[^;];%f;%d;%49[^\n]\n",
                  &p->id, p->produto, p->categoria,
                  &p->preco, &p->quantidade, p->fornecedor) == 6;
}

// Grava um produto em uma linha do arquivo
void gravarProduto(FILE *f, Produto *p) {
    fprintf(f, "%d;%s;%s;%.2f;%d;%s\n",
            p->id, p->produto, p->categoria, p->preco, p->quantidade, p->fornecedor);
}

// Carrega todos os produtos do arquivo para o vetor lista; *n recebe a quantidade
void carregar(Produto lista[], int *n) {
    FILE *f = fopen(ARQUIVO, "r");          // abre o arquivo para leitura
    *n = 0;                                 // comeca com zero produtos
    if (f == NULL) return;                  // arquivo nao existe: lista vazia
    while (*n < MAX && lerProduto(f, &lista[*n]))  // le uma linha por vez
        (*n)++;                             // conta mais um produto
    fclose(f);                              // fecha o arquivo
}

// Copia 'origem' para 'destino' em minusculas (inclui letras acentuadas em UTF-8)
void paraMinusculo(char *destino, char *origem) {
    while (*origem) {                       // percorre o texto original
        unsigned char c = *origem;          // caractere atual
        if (c == 0xC3 && origem[1] != '\0') {   // letra acentuada (2 bytes em UTF-8)
            unsigned char d = origem[1];    // segundo byte da letra acentuada
            if (d >= 0x80 && d <= 0x9E && d != 0x97) d += 0x20;  // maiuscula -> minuscula
            *destino++ = c;                 // copia o primeiro byte
            *destino++ = d;                 // copia o segundo byte (ja em minuscula)
            origem += 2;                    // avanca os 2 bytes
        } else {
            *destino++ = tolower(c);        // letra comum: converte para minuscula
            origem++;                       // avanca 1 byte
        }
    }
    *destino = '\0';                        // fim do texto
}

// Compara dois textos sem diferenciar maiusculas de minusculas (0 = iguais)
int compararTexto(char *a, char *b) {
    char x[120], y[120];                    // copias em minuscula
    paraMinusculo(x, a);                    // converte a
    paraMinusculo(y, b);                    // converte b
    return strcmp(x, y);                    // compara as copias
}

// Verifica se 'texto' contem 'trecho' sem diferenciar maiusculas (1 = sim)
int contemTexto(char *texto, char *trecho) {
    char x[120], y[120];                    // copias em minuscula
    paraMinusculo(x, texto);                // converte o texto
    paraMinusculo(y, trecho);               // converte o trecho
    return strstr(x, y) != NULL;            // 1 se encontrou o trecho dentro do texto
}

// Gera o proximo ID automatico (maior ID do arquivo + 1)
int proximoId() {
    Produto lista[MAX];                     // vetor de produtos
    int n, maior = 0;                       // quantidade e maior ID
    carregar(lista, &n);                    // le o arquivo
    for (int i = 0; i < n; i++)             // percorre os produtos
        if (lista[i].id > maior) maior = lista[i].id;  // guarda o maior
    return maior + 1;                       // proximo ID
}

// Verifica se ja existe um produto com esse ID (1 = sim)
int existeId(int id) {
    Produto lista[MAX];                     // vetor de produtos
    int n;                                  // quantidade de produtos
    carregar(lista, &n);                    // le o arquivo
    for (int i = 0; i < n; i++)             // percorre os produtos
        if (lista[i].id == id) return 1;    // achou o ID
    return 0;                               // nao achou
}

// Verifica se ja existe produto com esse nome, ignorando o ID 'ignorar' (1 = sim)
int existeNome(char *nome, int ignorar) {
    Produto lista[MAX];                     // vetor de produtos
    int n;                                  // quantidade de produtos
    carregar(lista, &n);                    // le o arquivo
    for (int i = 0; i < n; i++)             // percorre os produtos
        if (lista[i].id != ignorar && compararTexto(lista[i].produto, nome) == 0)
            return 1;                       // nome repetido
    return 0;                               // nome livre
}

// Mostra o vetor em formato de tabela (nao altera o arquivo)
void imprimirTabela(Produto lista[], int n) {
    printf("\n---------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-18s | %-16s | %10s | %-4s | %-14s |\n",
           "ID", "Produto", "Categoria", "Preco (R$)", "Qtd", "Fornecedor");
    printf("---------------------------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++)             // uma linha para cada produto
        printf("| %-3d | %-18s | %-16s | %10.2f | %-4d | %-14s |\n",
               lista[i].id, lista[i].produto, lista[i].categoria,
               lista[i].preco, lista[i].quantidade, lista[i].fornecedor);
    printf("---------------------------------------------------------------------------------------\n");
}

// ---------- Funcoes do menu ----------

void criar(){
    FILE *f = fopen(ARQUIVO, "w");          // cria (ou recria) o arquivo
    if (f == NULL) {                        // deu erro ao abrir?
        printf("Erro ao criar o arquivo!\n");
        return;
    }

    // grava os 4 produtos iniciais da tabela do enunciado
    fprintf(f, "%d;%s;%s;%.2f;%d;%s\n", 1, "Camiseta Básica", "Vestuário", 49.90, 30, "Moda Brasil");
    fprintf(f, "%d;%s;%s;%.2f;%d;%s\n", 2, "Mouse Óptico USB", "Informática", 59.99, 15, "TecnoShop");
    fprintf(f, "%d;%s;%s;%.2f;%d;%s\n", 3, "Shampoo 300ml", "Higiene Pessoal", 22.50, 40, "Beleza Pura");
    fprintf(f, "%d;%s;%s;%.2f;%d;%s\n", 4, "Café 500g", "Alimentos", 18.00, 25, "Grãos do Sul");

    fclose(f);                              // fecha o arquivo
    printf("Arquivo criado com 4 produtos!\n");
}

void listar(){
    Produto lista[MAX];                     // vetor de produtos
    int n;                                  // quantidade de produtos
    carregar(lista, &n);                    // le o arquivo para o vetor
    if (n == 0) {                           // nada para mostrar?
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    imprimirTabela(lista, n);               // mostra em formato de tabela
}

void inserir(){
    Produto p;                              // 1. variavel que guarda o produto novo

    printf("Produto: ");                    // 2. leitura dos campos
    scanf(" %49[^\n]", p.produto);

    if (existeNome(p.produto, 0)) {         // 3. verifica nome duplicado
        printf("Ja existe um produto com esse nome!\n");
        return;
    }

    printf("Categoria: ");
    scanf(" %49[^\n]", p.categoria);

    printf("Preco: ");
    scanf("%f", &p.preco);

    printf("Quantidade: ");
    scanf("%d", &p.quantidade);

    printf("Fornecedor: ");
    scanf(" %49[^\n]", p.fornecedor);

    p.id = proximoId();                     // 4. ID automatico (incremental)
    while (existeId(p.id))                  // 5. garante que o ID nao se repete
        p.id++;

    FILE *f = fopen(ARQUIVO, "a");          // 6. abre o arquivo (adiciona no final)
    if (f == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    gravarProduto(f, &p);                   // 7. grava o produto

    fclose(f);                              // 8. fecha
    printf("Produto inserido com ID %d!\n", p.id);
}

void alterar(){
    Produto novo, p;                        // novo = dados novos, p = produto lido do arquivo
    int id;                                 // ID do produto a alterar

    printf("ID do produto a alterar: ");
    scanf("%d", &id);
    if (!existeId(id)) {                    // ID nao existe?
        printf("ID nao encontrado!\n");
        return;
    }

    novo.id = id;                           // o ID continua o mesmo
    printf("Novo nome do produto: ");       // leitura dos novos dados
    scanf(" %49[^\n]", novo.produto);
    if (existeNome(novo.produto, id)) {     // nome igual ao de outro produto?
        printf("Ja existe um produto com esse nome!\n");
        return;
    }
    printf("Nova categoria: ");
    scanf(" %49[^\n]", novo.categoria);
    printf("Novo preco: ");
    scanf("%f", &novo.preco);
    printf("Nova quantidade: ");
    scanf("%d", &novo.quantidade);
    printf("Novo fornecedor: ");
    scanf(" %49[^\n]", novo.fornecedor);

    FILE *f = fopen(ARQUIVO, "r");          // abre o arquivo original para leitura
    FILE *tmp = fopen(TEMPORARIO, "w");     // cria o arquivo temporario
    if (f == NULL || tmp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    while (lerProduto(f, &p)) {             // le o original linha por linha
        if (p.id == id) p = novo;           // e o produto alterado: troca pelos dados novos
        gravarProduto(tmp, &p);             // grava no temporario
    }

    fclose(f);                              // fecha o original
    fclose(tmp);                            // fecha o temporario
    remove(ARQUIVO);                        // apaga o original
    rename(TEMPORARIO, ARQUIVO);            // o temporario vira o original
    printf("Produto alterado!\n");
}

void excluir(){
    Produto p;                              // produto lido do arquivo
    int id;                                 // ID do produto a excluir

    printf("ID do produto a excluir: ");
    scanf("%d", &id);
    if (!existeId(id)) {                    // ID nao existe?
        printf("ID nao encontrado!\n");
        return;
    }

    FILE *f = fopen(ARQUIVO, "r");          // abre o arquivo original para leitura
    FILE *tmp = fopen(TEMPORARIO, "w");     // cria o arquivo temporario
    if (f == NULL || tmp == NULL) {
        printf("Erro ao abrir os arquivos!\n");
        return;
    }

    while (lerProduto(f, &p))               // le o original linha por linha
        if (p.id != id)                     // so copia quem NAO e o excluido
            gravarProduto(tmp, &p);

    fclose(f);                              // fecha o original
    fclose(tmp);                            // fecha o temporario
    remove(ARQUIVO);                        // apaga o original
    rename(TEMPORARIO, ARQUIVO);            // o temporario vira o original
    printf("Produto excluido!\n");
}

// Busca por nome (porCategoria = 0) ou por categoria (porCategoria = 1)
void buscar(int porCategoria){
    Produto lista[MAX], achados[MAX];       // todos os produtos e os resultados
    int n, qtd = 0;                         // quantidade total e de resultados
    char termo[50];                         // texto procurado

    carregar(lista, &n);                    // le o arquivo
    printf(porCategoria ? "Categoria: " : "Nome: ");
    scanf(" %49[^\n]", termo);              // le o termo da busca

    for (int i = 0; i < n; i++) {           // percorre os produtos
        char *campo = porCategoria ? lista[i].categoria : lista[i].produto;
        if (contemTexto(campo, termo))      // o campo contem o termo?
            achados[qtd++] = lista[i];      // guarda o resultado
    }

    if (qtd == 0) printf("Nenhum produto encontrado.\n");
    else imprimirTabela(achados, qtd);      // mostra os resultados
}

void buscar_por_nome(){
    buscar(0);                              // busca pelo nome
}

void categoria(){
    buscar(1);                              // busca pela categoria
}

// Bubble Sort: campo 1 = nome, 2 = preco | crescente 1 = sim, 0 = decrescente
void bubbleSort(Produto *v, int n, int campo, int crescente) {
    for (int i = 0; i < n - 1; i++) {       // varias passagens pelo vetor
        for (int j = 0; j < n - i - 1; j++) {   // compara pares vizinhos
            int r;                          // resultado da comparacao
            if (campo == 1)                 // ordenar por nome
                r = compararTexto(v[j].produto, v[j + 1].produto);
            else                            // ordenar por preco
                r = (v[j].preco > v[j + 1].preco) - (v[j].preco < v[j + 1].preco);
            if (!crescente) r = -r;         // decrescente: inverte o criterio
            if (r > 0) {                    // esta na ordem errada?
                Produto aux = v[j];         // guarda o atual
                v[j] = v[j + 1];            // move o proximo para tras
                v[j + 1] = aux;             // coloca o atual na frente
            }
        }
    }
}

// Carrega, ordena o vetor em memoria e mostra (o arquivo nao e alterado)
void ordenarEExibir(int campo, int crescente) {
    Produto lista[MAX];                     // vetor de produtos
    int n;                                  // quantidade de produtos
    carregar(lista, &n);                    // le o arquivo
    if (n == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }
    bubbleSort(lista, n, campo, crescente); // ordena o vetor
    imprimirTabela(lista, n);               // mostra a tabela
}

void ordenar(){
    int op;
    do {
        printf("\n--- Ordenar ---\n");
        printf("1 - Nome (A-Z)\n");
        printf("2 - Nome (Z-A)\n");
        printf("3 - Preco (menor -> maior)\n");
        printf("4 - Preco (maior -> menor)\n");
        printf("0 - Voltar\n");
        scanf("%d", &op);
        switch (op) {
            case 1: ordenarEExibir(1, 1); break;   // nome A-Z
            case 2: ordenarEExibir(1, 0); break;   // nome Z-A
            case 3: ordenarEExibir(2, 1); break;   // preco crescente
            case 4: ordenarEExibir(2, 0); break;   // preco decrescente
            case 0: break;
            default: printf("Opcao invalida!\n");
        }
    } while (op != 0);
}

void csv(){
    Produto lista[MAX];                     // vetor de produtos
    int n;                                  // quantidade de produtos
    carregar(lista, &n);                    // le o arquivo

    FILE *f = fopen("produtos.csv", "w");   // cria o arquivo CSV
    if (f == NULL) {
        printf("Erro ao criar o CSV!\n");
        return;
    }

    fprintf(f, "ID,Produto,Categoria,Preco,Quantidade,Fornecedor\n");  // cabecalho
    for (int i = 0; i < n; i++)             // uma linha por produto, separada por virgula
        fprintf(f, "%d,%s,%s,%.2f,%d,%s\n",
                lista[i].id, lista[i].produto, lista[i].categoria,
                lista[i].preco, lista[i].quantidade, lista[i].fornecedor);

    fclose(f);                              // fecha o CSV
    printf("Exportado para produtos.csv!\n");
}


int main(){
    int op;
    do {
        printf("                                          \n");
        printf(" --- Escolha qual acao voce deseja realizar --- \n");
        printf("                                          \n");
        printf(" 1 - Criar                                \n");
        printf(" 2 - Listar                               \n");
        printf(" 3 - Inserir                              \n");
        printf(" 4 - Alterar                              \n");
        printf(" 5 - Excluir                              \n");
        printf(" 6 - Buscar por nome                      \n");
        printf(" 7 - Buscar por categoria                 \n");
        printf(" 8 - Ordenar produtos                     \n");
        printf(" 9 - Exportar CSV                         \n");
        printf(" 0 - Sair\n");
        printf("\n");
        printf("Opcao: ");
        scanf("%d", &op);
        switch (op) {
            case 1: criar(); break;
            case 2: listar();  break;
            case 3: inserir(); break;
            case 4: alterar(); break;
            case 5: excluir(); break;
            case 6: buscar_por_nome(); break;
            case 7: categoria(); break;
            case 8: ordenar(); break;
            case 9: csv(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (op != 0);

    return 0;

}