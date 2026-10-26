/*
 * CCP230 - Desenvolvimento de Algoritmos
 * Projeto: Exchange de Criptomoedas (individual)
 *
 * Arquivos usados pelo programa (o proprio programa cria, nao precisa criar a mao):
 *   usuario_<CPF>.txt  -> dados de cada investidor (criado no cadastro)
 *   extrato_<CPF>.txt  -> historico de operacoes de cada investidor
 *   cotacoes.txt       -> cotacao atual do Bitcoin, Ethereum e Ripple
 *
 * Nos vetores, cada posicao representa uma moeda:
 *   0 = Bitcoin, 1 = Ethereum, 2 = Ripple
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Tabelas fixas das moedas (nao mudam durante o programa) */
char nomes_moedas[3][10] = {"Bitcoin", "Ethereum", "Ripple"};
char siglas[3][4] = {"BTC", "ETH", "XRP"};
double taxa_compra[3] = {0.02, 0.01, 0.01};
double taxa_venda[3] = {0.03, 0.02, 0.01};


/* ===================== FUNCOES DE LEITURA ===================== */

/* Joga fora o que sobrou na linha digitada (inclusive o ENTER) */
void limpar_buffer() {
    int c;
    c = getchar();
    while (c != '\n' && c != EOF) {
        c = getchar();
    }
}

/* Le um numero inteiro. Se o usuario digitar letras, devolve -1 */
int ler_inteiro() {
    int numero;
    int lido = scanf("%d", &numero);
    if (lido == EOF) exit(0);
    limpar_buffer();
    if (lido != 1) return -1;
    return numero;
}

/* Le um numero real. Se o usuario digitar letras, devolve -1 */
double ler_valor() {
    double valor;
    int lido = scanf("%lf", &valor);
    if (lido == EOF) exit(0);
    limpar_buffer();
    if (lido != 1) return -1;
    return valor;
}

/* Le uma palavra (sem espacos) */
void ler_texto(char texto[]) {
    if (scanf("%49s", texto) == EOF) exit(0);
    limpar_buffer();
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}


/* ===================== FUNCOES DE ARQUIVO ===================== */

/* Le os dados do investidor. Devolve 0 se o CPF nao estiver cadastrado */
int carregar_usuario(char cpf[], char nome[], char senha[], double *reais, double cripto[]) {
    char arquivo[50];
    int tam;
    FILE *f;

    sprintf(arquivo, "usuario_%s.txt", cpf);
    f = fopen(arquivo, "r");
    if (f == NULL) return 0;

    /* 1a linha: nome (pode ter espaco, por isso fgets) */
    fgets(nome, 50, f);
    tam = strlen(nome);
    if (tam > 0 && nome[tam - 1] == '\n') nome[tam - 1] = '\0';   /* tira o ENTER do final */

    /* 2a linha: senha | 3a linha: saldos */
    fscanf(f, "%s", senha);
    fscanf(f, "%lf %lf %lf %lf", reais, &cripto[0], &cripto[1], &cripto[2]);

    fclose(f);
    return 1;
}

/* Grava (ou regrava) o arquivo do investidor com os saldos atuais */
void salvar_usuario(char cpf[], char nome[], char senha[], double reais, double cripto[]) {
    char arquivo[50];
    FILE *f;

    sprintf(arquivo, "usuario_%s.txt", cpf);
    f = fopen(arquivo, "w");
    if (f == NULL) {
        printf("Erro ao salvar os dados!\n");
        return;
    }
    fprintf(f, "%s\n", nome);
    fprintf(f, "%s\n", senha);
    fprintf(f, "%.2f %.8f %.8f %.8f\n", reais, cripto[0], cripto[1], cripto[2]);
    fclose(f);
}

void salvar_cotacoes(double cotacao[]) {
    FILE *f = fopen("cotacoes.txt", "w");
    if (f == NULL) {
        printf("Erro ao salvar as cotacoes!\n");
        return;
    }
    fprintf(f, "%.2f %.2f %.2f\n", cotacao[0], cotacao[1], cotacao[2]);
    fclose(f);
}

void carregar_cotacoes(double cotacao[]) {
    FILE *f = fopen("cotacoes.txt", "r");
    if (f == NULL) {
        /* primeira execucao: cotacoes iniciais em reais */
        cotacao[0] = 350000.00;
        cotacao[1] = 16108.00;
        cotacao[2] = 2.57;
        salvar_cotacoes(cotacao);
        return;
    }
    fscanf(f, "%lf %lf %lf", &cotacao[0], &cotacao[1], &cotacao[2]);
    fclose(f);
}

/* Acrescenta uma linha no final do extrato (modo "a") */
void registrar_extrato(char cpf[], char sinal, double valor, char moeda[],
                       double ct, double tx, double reais, double cripto[]) {
    char arquivo[50];
    FILE *f;
    time_t agora;
    struct tm *data;

    sprintf(arquivo, "extrato_%s.txt", cpf);
    f = fopen(arquivo, "a");
    if (f == NULL) {
        printf("Erro ao gravar o extrato!\n");
        return;
    }

    /* pega a data e hora atuais do computador */
    agora = time(NULL);
    data = localtime(&agora);

    fprintf(f, "%02d-%02d-%04d %02d:%02d %c %.2f %s CT: %.2f TX: %.2f ",
            data->tm_mday, data->tm_mon + 1, data->tm_year + 1900,
            data->tm_hour, data->tm_min, sinal, valor, moeda, ct, tx);
    fprintf(f, "REAL: %.2f BTC: %.6f ETH: %.6f XRP: %.6f\n",
            reais, cripto[0], cripto[1], cripto[2]);
    fclose(f);
}


/* ===================== FUNCOES DE TELA ===================== */

void mostrar_saldo(char nome[], char cpf[], double reais, double cripto[]) {
    printf("Nome: %s\n", nome);
    printf("CPF: %s\n", cpf);
    printf("Reais: %.2f\n", reais);
    printf("Bitcoin: %.6f\n", cripto[0]);
    printf("Ethereum: %.6f\n", cripto[1]);
    printf("Ripple: %.6f\n", cripto[2]);
}

/* tipo = 1 mostra a taxa de compra, tipo = 2 mostra a taxa de venda */
void mostrar_cotacoes(double cotacao[], int tipo) {
    int i;
    printf("\nCotacoes atuais:\n");
    for (i = 0; i < 3; i++) {
        printf("%d. %-9s R$ %12.2f   ", i + 1, nomes_moedas[i], cotacao[i]);
        if (tipo == 1)
            printf("(taxa de compra: %.0f%%)\n", taxa_compra[i] * 100);
        else
            printf("(taxa de venda: %.0f%%)\n", taxa_venda[i] * 100);
    }
}

/* Pede a senha de novo. Devolve 1 se estiver certa */
int verificar_senha(char senha[]) {
    char digitada[50];
    printf("Digite sua senha: ");
    ler_texto(digitada);
    if (strcmp(digitada, senha) == 0) return 1;
    printf("Senha incorreta! Operacao cancelada.\n");
    return 0;
}


/* ===================== OPCOES DO MENU ===================== */

void consultar_saldo(char nome[], char cpf[], char senha[], double reais, double cripto[]) {
    printf("\n===== CONSULTAR SALDO =====\n");
    if (verificar_senha(senha) == 0) return;
    printf("\n");
    mostrar_saldo(nome, cpf, reais, cripto);
}

void consultar_extrato(char nome[], char cpf[], char senha[]) {
    char arquivo[50], linha[200];
    FILE *f;

    printf("\n===== CONSULTAR EXTRATO =====\n");
    if (verificar_senha(senha) == 0) return;

    printf("\nNome: %s\n", nome);
    printf("CPF: %s\n", cpf);

    sprintf(arquivo, "extrato_%s.txt", cpf);
    f = fopen(arquivo, "r");
    if (f == NULL) {
        printf("Nenhuma operacao realizada ainda.\n");
        return;
    }
    while (fgets(linha, 200, f) != NULL) {
        printf("%s", linha);
    }
    fclose(f);
}

void depositar(char nome[], char cpf[], double *reais, double cripto[]) {
    double valor;

    printf("\n===== DEPOSITAR =====\n");
    printf("Valor do deposito (use ponto, ex: 150.50): R$ ");
    valor = ler_valor();
    if (valor <= 0) {
        printf("Valor invalido!\n");
        return;
    }

    *reais = *reais + valor;
    registrar_extrato(cpf, '+', valor, "REAL", 0, 0, *reais, cripto);

    printf("\nDeposito realizado!\n\n");
    mostrar_saldo(nome, cpf, *reais, cripto);
}

void sacar(char nome[], char cpf[], double *reais, double cripto[]) {
    double valor;

    printf("\n===== SACAR =====\n");
    printf("Valor do saque (use ponto, ex: 150.50): R$ ");
    valor = ler_valor();
    if (valor <= 0) {
        printf("Valor invalido!\n");
        return;
    }
    if (valor > *reais) {
        printf("Saldo insuficiente! Voce tem R$ %.2f\n", *reais);
        return;
    }

    *reais = *reais - valor;
    registrar_extrato(cpf, '-', valor, "REAL", 0, 0, *reais, cripto);

    printf("\nSaque realizado!\n\n");
    mostrar_saldo(nome, cpf, *reais, cripto);
}

void comprar(char nome[], char cpf[], char senha[], double *reais,
             double cripto[], double cotacao[]) {
    int moeda;
    double valor, taxa, total, quantidade;

    printf("\n===== COMPRAR CRIPTOMOEDAS =====\n");
    if (verificar_senha(senha) == 0) return;

    mostrar_cotacoes(cotacao, 1);
    printf("Qual moeda deseja comprar? (0 para cancelar): ");
    moeda = ler_inteiro();
    if (moeda < 1 || moeda > 3) {
        if (moeda != 0) printf("Opcao invalida!\n");
        return;
    }
    moeda = moeda - 1;   /* o menu vai de 1 a 3, o vetor de 0 a 2 */

    printf("Seu saldo: R$ %.2f\n", *reais);
    printf("Quanto deseja investir em reais? R$ ");
    valor = ler_valor();
    if (valor <= 0) {
        printf("Valor invalido!\n");
        return;
    }

    taxa = valor * taxa_compra[moeda];
    total = valor + taxa;
    quantidade = valor / cotacao[moeda];

    if (total > *reais) {
        printf("Saldo insuficiente! Com a taxa, o total fica R$ %.2f\n", total);
        return;
    }

    *reais = *reais - total;
    cripto[moeda] = cripto[moeda] + quantidade;
    registrar_extrato(cpf, '+', valor, siglas[moeda], cotacao[moeda],
                      taxa_compra[moeda], *reais, cripto);

    printf("\nCompra realizada! Voce recebeu %.6f %s (taxa: R$ %.2f)\n\n",
           quantidade, siglas[moeda], taxa);
    mostrar_saldo(nome, cpf, *reais, cripto);
}

void vender(char nome[], char cpf[], char senha[], double *reais,
            double cripto[], double cotacao[]) {
    int moeda;
    double quantidade, bruto, taxa, liquido;

    printf("\n===== VENDER CRIPTOMOEDAS =====\n");
    if (verificar_senha(senha) == 0) return;

    mostrar_cotacoes(cotacao, 2);
    printf("Qual moeda deseja vender? (0 para cancelar): ");
    moeda = ler_inteiro();
    if (moeda < 1 || moeda > 3) {
        if (moeda != 0) printf("Opcao invalida!\n");
        return;
    }
    moeda = moeda - 1;

    printf("Voce tem %.8f %s\n", cripto[moeda], siglas[moeda]);
    printf("Quantidade que deseja vender: ");
    quantidade = ler_valor();
    if (quantidade <= 0) {
        printf("Quantidade invalida!\n");
        return;
    }
    if (quantidade > cripto[moeda]) {
        printf("Saldo insuficiente!\n");
        return;
    }

    bruto = quantidade * cotacao[moeda];
    taxa = bruto * taxa_venda[moeda];
    liquido = bruto - taxa;

    cripto[moeda] = cripto[moeda] - quantidade;
    *reais = *reais + liquido;
    registrar_extrato(cpf, '-', bruto, siglas[moeda], cotacao[moeda],
                      taxa_venda[moeda], *reais, cripto);

    printf("\nVenda realizada! Voce recebeu R$ %.2f (taxa: R$ %.2f)\n\n", liquido, taxa);
    mostrar_saldo(nome, cpf, *reais, cripto);
}

/* Cada cotacao varia aleatoriamente entre -5% e +5% */
void atualizar_cotacao(double cotacao[]) {
    int i;
    double variacao, anterior;

    printf("\n===== ATUALIZAR COTACAO =====\n");
    for (i = 0; i < 3; i++) {
        /* rand() % 1001 vai de 0 a 1000 -> menos 500 fica de -500 a 500
           -> dividido por 10000 fica de -0.05 a +0.05 */
        variacao = (rand() % 1001 - 500) / 10000.0;
        anterior = cotacao[i];
        cotacao[i] = anterior * (1 + variacao);
        printf("%-9s R$ %.2f -> R$ %.2f (%+.2f%%)\n",
               nomes_moedas[i], anterior, cotacao[i], variacao * 100);
    }
    salvar_cotacoes(cotacao);
}

void mostrar_menu(char nome[]) {
    printf("\n===== EXCHANGE DE CRIPTOMOEDAS =====\n");
    printf("Ola, %s!\n\n", nome);
    printf("1. Consultar saldo\n");
    printf("2. Consultar extrato\n");
    printf("3. Depositar\n");
    printf("4. Sacar\n");
    printf("5. Comprar criptomoedas\n");
    printf("6. Vender criptomoedas\n");
    printf("7. Atualizar cotacao\n");
    printf("8. Sair\n");
    printf("Escolha uma opcao: ");
}


/* ===================== CADASTRO E LOGIN ===================== */

/* Cadastra um novo investidor, criando o arquivo usuario_<CPF>.txt */
void cadastrar() {
    char nome[50], cpf[50], senha[50];
    char nome_lixo[50], senha_lixo[50];
    double reais_lixo, cripto[3] = {0, 0, 0};
    int tam;

    printf("\n===== CADASTRO =====\n");
    printf("Nome completo: ");
    fgets(nome, 50, stdin);
    tam = strlen(nome);
    if (tam > 0 && nome[tam - 1] == '\n') nome[tam - 1] = '\0';   /* tira o ENTER do final */

    printf("CPF (so numeros, sem pontos e sem traco): ");
    ler_texto(cpf);

    /* se ja existe arquivo com esse CPF, a pessoa ja esta cadastrada */
    if (carregar_usuario(cpf, nome_lixo, senha_lixo, &reais_lixo, cripto) == 1) {
        printf("Este CPF ja esta cadastrado!\n");
        return;
    }

    printf("Crie uma senha de 6 numeros: ");
    ler_texto(senha);

    /* novo investidor comeca com todos os saldos zerados */
    salvar_usuario(cpf, nome, senha, 0, cripto);
    printf("\nCadastro realizado! Agora e so fazer o login.\n");
}

/* Pede CPF e senha. Devolve 1 se o login deu certo e preenche os dados */
int login(char cpf[], char nome[], char senha[], double *reais, double cripto[]) {
    char senha_digitada[50];

    printf("\n===== LOGIN =====\n");
    printf("CPF (so numeros): ");
    ler_texto(cpf);
    printf("Senha (6 digitos): ");
    ler_texto(senha_digitada);

    if (carregar_usuario(cpf, nome, senha, reais, cripto) == 0 ||
        strcmp(senha, senha_digitada) != 0) {
        printf("CPF ou senha incorretos!\n");
        return 0;
    }
    return 1;
}


/* ===================== PROGRAMA PRINCIPAL ===================== */

int main() {
    char cpf[50], nome[50], senha[50];
    double reais, cripto[3], cotacao[3];
    int opcao_inicial, opcao;

    srand(time(NULL));
    carregar_cotacoes(cotacao);

    do {
        printf("\n====================================\n");
        printf("     EXCHANGE DE CRIPTOMOEDAS\n");
        printf("====================================\n");
        printf("1. Login\n");
        printf("2. Cadastrar\n");
        printf("0. Encerrar programa\n");
        printf("Escolha uma opcao: ");
        opcao_inicial = ler_inteiro();

        if (opcao_inicial == 2) {
            cadastrar();
        } else if (opcao_inicial == 1) {
            if (login(cpf, nome, senha, &reais, cripto) == 1) {
                /* login certo: mostra o menu ate o usuario escolher Sair */
                do {
                    mostrar_menu(nome);
                    opcao = ler_inteiro();

                    switch (opcao) {
                        case 1: consultar_saldo(nome, cpf, senha, reais, cripto); break;
                        case 2: consultar_extrato(nome, cpf, senha); break;
                        case 3: depositar(nome, cpf, &reais, cripto); break;
                        case 4: sacar(nome, cpf, &reais, cripto); break;
                        case 5: comprar(nome, cpf, senha, &reais, cripto, cotacao); break;
                        case 6: vender(nome, cpf, senha, &reais, cripto, cotacao); break;
                        case 7: atualizar_cotacao(cotacao); break;
                        case 8: printf("Saindo da conta...\n"); break;
                        default: printf("Opcao invalida!\n");
                    }

                    /* salva depois de toda operacao, para nada se perder */
                    salvar_usuario(cpf, nome, senha, reais, cripto);

                    if (opcao != 8) pausar();
                } while (opcao != 8);
            }
        } else if (opcao_inicial != 0) {
            printf("Opcao invalida!\n");
        }
    } while (opcao_inicial != 0);

    printf("Programa encerrado. Ate logo!\n");
    return 0;
}