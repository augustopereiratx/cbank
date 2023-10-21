#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include "vars.h"

// Criptografia básica (obrigado ChatGPT)
// Esse tipo de crypt é inseguro, mas pra esse projeto é ok
void encrypt(char *passwd, const char *key)
{
    int keyLen = strlen(key);
    int passwdLen = strlen(passwd);

    for (int i = 0; i < passwdLen; i++)
    {
        passwd[i] ^= key[i % keyLen];
    }
}

// Wrapper da func de encriptar (faz o mesmo processo para retornar a string)
void decrypt(char *passwd, const char *key)
{
    encrypt(passwd, key);
}

// Função para incicializar acentos em português
int init()
{
    setlocale(LC_ALL, "Portuguese");
    return 0;
}

// Tela de erro para o usuário
void clienterror()
{
    printf("Cliente não encontrado ou senha incorreta.\n");
}

// Entrada de strings
int input(char *text, char *str, int max_len)
{
    strcpy(str, "");
    printf("%s", text);
    printf("");
    if (fgets(str, max_len, stdin) != NULL)
    {
        // Retorna 1 porque eu uso como comparador em ifs
        return 1;
    }
    else
    {
        return 0;
    }
}

// Comparar uma string com a outra (ChatGPT me ajudou com esse return)
int strcompare(char *str1, char *str2)
{
    int result = strcmp(str1, str2);
    if (result == 0)
    {
        // Compara cada char pra ver se é igual mesmo
        for (int i = 0; i < strlen(str2); i++)
        {
            if (str1[i] != str2[i])
            {
                return 0;
            }
        }
    }
    return (result == 0) ? 1 : 0;
}

// Entrada de ints
int intinput(char *text)
{
    char input_buffer[256];
    printf("%s", text);
    if (fgets(input_buffer, sizeof(input_buffer), stdin) != NULL)
    {
        char *endptr;
        // mesma lógica do input de strings, só muda de str pra long
        long number = strtol(input_buffer, &endptr, 10);

        if (endptr != input_buffer && (*endptr == '\n' || *endptr == '\0'))
        {
            // aqui o long vira int
            return (int)number;
        }
    }
    return -1;
}

// Novo Cliente
int newclient(char *str, struct client *clients, int numClients)
{
    int i;
    int check = 0;
    int cpferro;
    int accounttype = 0;
    double valor;
    // Eu uso whiles pra prender o usuáro enquanto ele não termina esse processo
    while (1)
    {
        cpferro = 0;
        input("Digite seu CPF (somente números)\n-> ", str, 1024);
        if (strlen(str) == 12)
        {
            for (int i = 0; i < 11; i++)
            {
                if (!isdigit(str[i]))
                {
                    cpferro = 1;
                }
            }
            if (!cpferro)
            {
                for (int i = 0; i < numClients; i++)
                {
                    cpferro = 1;
                    for (int j = 0; j < 11; j++)
                    {
                        if (str[j] != clients[i].cpf[j])
                        {
                            cpferro = 0;
                            break;
                        }
                    }
                }
                if (cpferro)
                {
                    printf("CPF já em uso.\n");
                }
            }
        }
        else
        {
            cpferro = 1;
        }
        if (!cpferro)
        {
            strcpy(clients[numClients].cpf, str);
            break;
        }
        printf("\n");
    }
    while (1)
    {
        if (input("Digite seu nome:\n-> ", clients[numClients].name, 512))
        {
            for (int i = 0; i < strlen(clients[numClients].name); i++)
            {
                // Vê se tem espaço no nome
                if (clients[numClients].name[i] == ' ')
                {
                    check = 1;
                    continue;
                }

                // Troca um \n no final pra \0. Não sei porque tem \n ali
                if (clients[numClients].name[i] == '\n')
                {
                    clients[numClients].name[i] = '\0';
                }
            }
            if (!check)
            {
                printf("É necessário pelo menos um nome e um sobrenome.\n");
                continue;
            }
            break;
        }
        printf("\n");
    }
    while (1)
    {
        if (input("Digite sua senha: (deve conter ao menos uma letra maiúscula e um número)\n-> ", str, 1024))
        {
            // Verificar se senha tem maiúscula e número
            int upperpass = 0;
            int numberpass = 0;

            for (i = 0; i < strlen(str); i++)
            {
                printf("%c", *(str + i));
                if (isupper(*(str + i)))
                {
                    upperpass = 1;
                    continue;
                }
                if (isdigit(*(str + i)))
                {
                    numberpass = 1;
                    continue;
                }
                if (numberpass && upperpass)
                {
                    break;
                }
            }
            if (numberpass && upperpass)
            {
                // Criptografar senha
                encrypt(str, key);
                strcpy(clients[numClients].passwd, str);
                break;
            }
            else
            {
                printf("Senha fraca. Tente novamente.\n");
            }
        }
        printf("\n");
    }
    while (1)
    {
        accounttype = intinput("Qual o tipo de conta?\n1. Comum\n2. Plus\n-> ");
        if (accounttype == 1 || accounttype == 2)
        {
            clients[numClients].accounttype = accounttype;
            break;
        }
    }
    while (1)
    {
        if (input("Digite o valor inicial de capital da conta:\n-> ", str, 1024))
        {
            // Vírgula pra ponto
            for (i = 0; i < strlen(str); i++)
            {
                if (*(str + i) == ',')
                {
                    str[i] = '.';
                    break;
                }
            }
            // Converter pra double
            valor = strtof(str, NULL);
            clients[numClients].money = valor;

            // "inicialização" do extrato
            clients[numClients].extr.detailcount = 0;
            break;
        }
    }
    return 0;
}

// Listar Clientes
int listclients(int numClients, struct client *clients)
{
    printf("Lista de Clientes:\n");

    for (int i = 0; i < numClients + 1; i++)
    {
        if (clients[i].accounttype == 1 || clients[i].accounttype == 2)
        {
            printf("Cliente %d:\n", i + 1);
            printf("Nome: %s\n", clients[i].name);
            printf("CPF: %s\n", clients[i].cpf);
            printf("Tipo de Conta: ");
            if (clients[i].accounttype == 1)
            {
                printf("Comum\n");
            }
            else
            {
                printf("Plus\n");
            }
            printf("Dinheiro: R$%.2f\n", clients[i].money);
            printf("\n");
        }
    }
    return 0;
}

// Achar id do cliente para usar em outras funcs
int validateclient(char *str, int numClients, struct client *clients, int requirePW)
{
    // Exceção para casos de transferência (destinatário)
    if (requirePW == 2)
    {
        input("Digite o CPF do destinatário (somente números)\n-> ", str, 1024);
        int id = -1;
        for (int i = 0; i < numClients + 1; i++)
        {
            if (strcompare(str, clients[i].cpf))
            {
                id = i;
                break;
            }
        }
        if (id == -1)
        {
            return -1;
        }
        else
        {
            return id;
        }
    }

    input("Digite seu CPF (somente números)\n-> ", str, 1024);
    int id = -1;
    for (int i = 0; i < numClients + 1; i++)
    {
        if (strcompare(str, clients[i].cpf))
        {
            id = i;
            break;
        }
    }
    if (id == -1)
    {
        return -1;
    }
    if (requirePW)
    {
        char pass[512];
        input("Digite sua senha:\n-> ", pass, 512);
        encrypt(pass, key);
        if (strcompare(pass, clients[id].passwd))
        {
            return id;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return id;
    }
    return -1;
}

// Marcar no extrato (fiz essa func pra deixar o código mais modular)
int makeextr(struct client *clients, int id, double val, double taxa, int transaction_type)
{
    char *string = malloc(128 * sizeof(char));
    switch (transaction_type)
    {
    case 0:
        // Débito
        sprintf(string, "Débito - R$%.2f - Taxa - R$%.2f", val, taxa);
        break;
    case 1:
        // Depósito
        sprintf(string, "Depósito - R$%.2f", val);
        break;
    default:
        return 1;
    }

    if (clients[id].extr.detailcount == 100)
    {
        resetextr(clients, id);
        strcpy(clients[id].extr.details[99], string);
    }
    else
    {
        strcpy(clients[id].extr.details[clients[id].extr.detailcount], string);
        clients[id].extr.detailcount++;
    }
    free(string);
    return 0;
}

// Deletar Cliente
int deleteclient(char *str, int numClients, struct client *clients)
{
    int id = validateclient(str, numClients, clients, 1);
    if (id == -1)
    {
        clienterror();
        return 0;
    }
    else
    {
        int c = intinput("Deseja mesmo deletar sua conta?\n1 - Sim\n2 - Não\n-> ");
        if (c == 2)
        {
            return 1;
        }
        for (int i = 0; i < numClients; i++)
        {
            clients[i].accounttype = clients[i + 1].accounttype;
            strcpy(clients[i].cpf, clients[i + 1].cpf);
            strcpy(clients[i].name, clients[i + 1].name);
            strcpy(clients[i].passwd, clients[i + 1].passwd);
            clients[i].money = clients[i + 1].money;
            clients[i].extr.detailcount = clients[i + 1].extr.detailcount;
            for (int j = 0; j < 100; j++)
            {
                strcpy(clients[i].extr.details[j], clients[i + 1].extr.details[j]);
            }
        }
    }

    return 0;
}

// Fazer loop no Extrato (necessário para ver sempre os 100 últimos casos)
int resetextr(struct client *clients, int id)
{
    for (int i = 0; i < 100; i++)
    {
        clients[id].extr.detailcount = clients[id + 1].extr.detailcount;
        strcpy(clients[id].extr.details[i], clients[id + 1].extr.details[i]);
    }
    return 0;
}

int deposito(char *str, struct client *clients, int numClients)
{
    int id = validateclient(str, numClients, clients, 0);
    if (id == -1)
    {
        clienterror();
        return 1;
    }
    else
    {
        input("Digite o valor a ser depositado:\nR$", str, 1024);
        for (int i = 0; i < strlen(str); i++)
        {
            if (str[i] == ',')
            {
                str[i] = '.';
                break;
            }
        }
    }
    char *end;
    double val = strtof(str, &end);
    clients[id].money += val;
    makeextr(clients, id, val, 0.0, 1);
    return 0;
}

int debito(char *str, struct client *clients, int numClients)
{
    int id = validateclient(str, numClients, clients, 1);
    if (id == -1)
    {
        clienterror();
        return 1;
    }
    else
    {

        input("Digite o valor a ser debitado:\nR$", str, 1024);
        for (int i = 0; i < strlen(str); i++)
        {
            if (str[i] == ',')
            {
                str[i] = '.';
                break;
            }
        }
        char *end;
        double val = strtof(str, &end);
        int check = 0;
        double taxa = 0.0;
        if (clients[id].accounttype == 1)
        {
            taxa = (val * 5) / 100;
            val = val + taxa;
            if (clients[id].money - val >= -1000.0)
            {
                check = 1;
            }
        }
        else if (clients[id].accounttype == 2)
        {
            taxa = (val * 3) / 100;
            val = val + taxa;
            if (clients[id].money - val >= -5000.0)
            {
                check = 1;
            }
        }
        if (check)
        {
            clients[id].money -= val;
            makeextr(clients, id, val, taxa, 0);
        }
        else
        {
            printf("Sem saldo suficiente na conta.\n");
            return 1;
        }
    }
    return 0;
}

// Mostrar e Salvar Extrato em um TXT com nome do usuário
int showextr(char *str, struct client *clients, int numClients)
{
    int id = validateclient(str, numClients, clients, 1);
    if (id == -1)
    {
        clienterror();
        return 1;
    }
    else
    {
        char nameformat[128];
        strcpy(nameformat, clients[id].name);
        for (int i = 0; i < strlen(nameformat); i++)
        {
            if (nameformat[i] == ' ')
            {
                nameformat[i] = '_';
            }
        }
        char fname[128];
        sprintf(fname, "%s.txt", nameformat);
        FILE *f = fopen(fname, "w");
        fprintf(f, "Extrato de %s:\n", clients[id].name);
        printf("Extrato:\n");
        for (int i = 0; i < clients[id].extr.detailcount; i++)
        {
            printf("%s\n", clients[id].extr.details[i]);
            fprintf(f, "%s\n", clients[id].extr.details[i]);
        }
        fclose(f);
    }
}

int transfer(char *str, struct client *clients, int numClients)
{
    // Origem
    int id = validateclient(str, numClients, clients, 1);
    if (id == -1)
    {
        clienterror();
        return 1;
    }

    // Destinatário
    int destid = validateclient(str, numClients, clients, 2);
    if (destid == -1)
    {
        clienterror();
        return 2;
    }

    input("Digite o valor a ser transferido:\nR$", str, 1024);
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == ',')
        {
            str[i] = '.';
            break;
        }
    }
    char *end;
    double val = strtof(str, &end);
    double destval = val;
    int check = 0;
    double taxa = 0.0;

    // Aplicar taxa
    if (clients[id].accounttype == 1)
    {
        taxa = (val * 5) / 100;
        val = val + taxa;
        if (clients[id].money - val >= -1000.0)
        {
            check = 1;
        }
    }
    else if (clients[id].accounttype == 2)
    {
        taxa = (val * 3) / 100;

        val = val + taxa;
        if (clients[id].money - val >= -5000.0)
        {
            check = 1;
        }
    }

    if (check)
    {
        // Origem tem taxa + valor removidos
        clients[id].money -= val;
        makeextr(clients, id, val, taxa, 0);

        // Destino tem somente o valor adicionado (destval)
        clients[destid].money += destval;
        makeextr(clients, destid, destval, 0.0, 1);
    }
    else
    {
        printf("Sem saldo suficiente na conta.\n");
        return 1;
    }
    return 0;
}

int savedata(struct clients *clientlist, int sizeclients)
{
    FILE *bin = fopen("save.bin", "wb");
    if (bin == NULL)
    {
        printf("Erro ao abrir arquivo para salvar.\n");
        return 1;
    }
    if (fwrite(clientlist, sizeclients, 1, bin) != 1)
    {
        printf("Erro ao escrever no arquivo. (clientlist)\n");
        fclose(bin);
        return 2;
    }
    fclose(bin);
    return 0;
}

int loaddata(struct clients *clientlist, int sizeclients)
{
    FILE *bin = fopen("save.bin", "rb");
    if (bin == NULL)
    {
        printf("Erro ao abrir arquivo para leitura.\n");
        return 1;
    }
    if (fread(clientlist, sizeclients, 1, bin) != 1)
    {
        printf("Erro ao escrever no arquivo. (clientlist)\n");
        fclose(bin);
        return 2;
    }
    fclose(bin);
    return 0;
}