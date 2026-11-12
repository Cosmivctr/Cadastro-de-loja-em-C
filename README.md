# 🛒 Cadastro de Loja em C

Sistema de cadastro de produtos para uma loja, feito em **linguagem C**, que reúne **manipulação de arquivos**, **estruturas de dados**, **ponteiros** e **algoritmos de ordenação** em um único programa de terminal.

Projeto desenvolvido como **Atividade Avaliativa 04**.

---

## 📋 Sumário

- [Funcionalidades](#-funcionalidades)
- [Conceitos aplicados](#-conceitos-aplicados)
- [Estrutura de dados](#-estrutura-de-dados)
- [Formato dos arquivos](#-formato-dos-arquivos)
- [Como compilar e executar](#-como-compilar-e-executar)
- [Menu do programa](#-menu-do-programa)
- [Exemplo de execução](#-exemplo-de-execução)
- [Decisões técnicas](#-decisões-técnicas)
- [Limitações](#-limitações)
- [Autor](#-autor)

---

## ✅ Funcionalidades

| Função | Descrição |
|---|---|
| **Criar** | Cria o arquivo `produtos.txt` com 4 produtos iniciais |
| **Listar** | Exibe todos os produtos em formato de tabela |
| **Inserir** | Cadastra um novo produto com **ID automático** e verificação de **nome duplicado** |
| **Alterar** | Altera os dados de um produto pelo ID |
| **Excluir** | Remove um produto pelo ID |
| **Buscar por nome** | Busca por trecho do nome, sem diferenciar maiúsculas/minúsculas |
| **Buscar por categoria** | Busca por trecho da categoria, sem diferenciar maiúsculas/minúsculas |
| **Ordenar** | Nome (A–Z), nome (Z–A), preço (menor → maior) e preço (maior → menor) |
| **Exportar CSV** | Gera o arquivo `produtos.csv` separado por vírgula |

### Regras de negócio

- 🔢 **ID automático e incremental:** o usuário não digita o ID; o programa usa o maior ID existente + 1.
- 🚫 **Sem duplicatas:** não é possível cadastrar (nem alterar para) um nome que já exista.
- 🔍 **Verificação de ID duplicado** na inserção.
- 🔤 **Comparações e buscas *case-insensitive***, inclusive com letras acentuadas (ex.: `Óptico` = `óptico`).
- 📄 A ordenação e a exibição em tabela **não alteram o arquivo**.

---

## 🧠 Conceitos aplicados

- **Manipulação de arquivos** — `fopen`, `fscanf`, `fprintf`, `fclose`, `remove`, `rename`
- **Struct** — `typedef struct` para representar o produto
- **Ponteiros** — passagem de structs, vetores e strings por ponteiro nas funções
- **Vetor de estruturas** — `Produto lista[MAX]`
- **Algoritmo de ordenação** — **Bubble Sort**, com critério (nome/preço) e direção (crescente/decrescente) definidos por parâmetros
- **Estrutura `switch`** — menu principal e submenu de ordenação
- **Modularização** — cada operação em sua própria função

---

## 🧱 Estrutura de dados

```c
typedef struct {
    int id;                 // identificador (gerado automaticamente)
    char produto[50];       // nome do produto
    char categoria[50];     // categoria do produto
    float preco;            // preço em reais
    int quantidade;         // quantidade em estoque
    char fornecedor[50];    // nome do fornecedor
} Produto;
```

---

## 🗂️ Formato dos arquivos

### `produtos.txt` (dados do sistema)

Um produto por linha, com os campos separados por `;`:

```text
1;Camiseta Básica;Vestuário;49.90;30;Moda Brasil
2;Mouse Óptico USB;Informática;59.99;15;TecnoShop
3;Shampoo 300ml;Higiene Pessoal;22.50;40;Beleza Pura
4;Café 500g;Alimentos;18.00;25;Grãos do Sul
```

### `produtos.csv` (exportação)

Gerado pela opção **Exportar CSV**, separado por vírgula:

```csv
ID,Produto,Categoria,Preco,Quantidade,Fornecedor
1,Camiseta Básica,Vestuário,49.90,30,Moda Brasil
2,Mouse Óptico USB,Informática,59.99,15,TecnoShop
3,Shampoo 300ml,Higiene Pessoal,22.50,40,Beleza Pura
4,Café 500g,Alimentos,18.00,25,Grãos do Sul
```

---

## ⚙️ Como compilar e executar

**Pré-requisito:** um compilador C (por exemplo, o [GCC](https://gcc.gnu.org/)).

```bash
# Clonar o repositório
git clone https://github.com/Cosmivctr/NOME-DO-REPOSITORIO.git
cd NOME-DO-REPOSITORIO

# Compilar
gcc atividade_04.c -o atividade_04

# Executar (Linux/macOS)
./atividade_04

# Executar (Windows)
atividade_04.exe
```

> 💡 Na primeira execução, use a opção **1 - Criar** para gerar o arquivo `produtos.txt` com os dados iniciais.

---

## 🖥️ Menu do programa

```text
 --- Escolha qual acao voce deseja realizar ---

 1 - Criar
 2 - Listar
 3 - Inserir
 4 - Alterar
 5 - Excluir
 6 - Buscar por nome
 7 - Buscar por categoria
 8 - Ordenar produtos
 9 - Exportar CSV
 0 - Sair
```

Submenu **Ordenar produtos**:

```text
--- Ordenar ---
1 - Nome (A-Z)
2 - Nome (Z-A)
3 - Preco (menor -> maior)
4 - Preco (maior -> menor)
0 - Voltar
```

---

## 📺 Exemplo de execução

Listagem (opção `2`):

```text
---------------------------------------------------------------------------------------
| ID  | Produto            | Categoria        | Preco (R$) | Qtd  | Fornecedor     |
---------------------------------------------------------------------------------------
| 1   | Camiseta Básica    | Vestuário        |      49.90 | 30   | Moda Brasil    |
| 2   | Mouse Óptico USB   | Informática      |      59.99 | 15   | TecnoShop      |
| 3   | Shampoo 300ml      | Higiene Pessoal  |      22.50 | 40   | Beleza Pura    |
| 4   | Café 500g          | Alimentos        |      18.00 | 25   | Grãos do Sul   |
---------------------------------------------------------------------------------------
```

Tentativa de cadastrar um nome repetido (opção `3`):

```text
Produto: mouse óptico usb
Ja existe um produto com esse nome!
```

---

## 🔧 Decisões técnicas

- **Arquivo temporário em alterar/excluir:** o programa lê o arquivo original, grava o resultado em `temp.txt`, apaga o original e renomeia o temporário. É uma técnica comum para evitar corrupção de dados caso algo falhe durante a escrita.
- **Bubble Sort didático:** a mesma função ordena por nome ou por preço, em ordem crescente ou decrescente, de acordo com os parâmetros `campo` e `crescente`.
- **Ordenação em memória:** os dados são carregados para um vetor, ordenados e exibidos; o arquivo `produtos.txt` permanece intacto.
- **Busca por trecho:** buscar `info` encontra `Informática`, e buscar `mouse` encontra `Mouse Óptico USB`.
- **Case-insensitive com UTF-8:** a função `paraMinusculo` converte também letras acentuadas, para que a comparação funcione com dados em português.

---

## ⚠️ Limitações

- Capacidade máxima de **100 produtos** (`MAX`) e campos de texto com até **49 caracteres** — aumente os valores no código se precisar de mais.
- O preço deve ser digitado com **ponto** (ex.: `89.90`).
- Os textos não devem conter `;`, pois ele é o separador do arquivo de dados.
- Em terminais com acentos, as colunas da tabela podem ficar levemente desalinhadas.

---

## 👤 Autor

**João Victor Ferreira Cosmi**

[![GitHub](https://img.shields.io/badge/GitHub-Cosmivctr-181717?style=flat&logo=github)](https://github.com/Cosmivctr)
