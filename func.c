#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include "vars.h"

// Criptografia básica (obrigado ChatGPT)
void encrypt(char *passwd, const char *key)
{
    int keyLen = strlen(key);
    int passwdLen = strlen(passwd);

    for (int i = 0; i < passwdLen; i++)
    {
        passwd[i] ^= key[i % keyLen];
    }
}

// Decryption function
void decrypt(char *passwd, const char *key)
{
    int keyLen = strlen(key);
    int passwdLen = strlen(passwd);

    for (int i = 0; i < passwdLen; i++)
    {
        passwd[i] ^= key[i % keyLen];
    }
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

// Comparar com senha já salva (ChatGPT me ajudou com esse return)
int strcompare(char *str1, char *str2)
{
    int result = strcmp(str1, str2);
    if (result == 0)
    {
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
        long number = strtol(input_buffer, &endptr, 10);

        if (endptr != input_buffer && (*endptr == '\n' || *endptr == '\0'))
        {
            return (int)number;
        }
    }
    return -1;
}

// Novo Cliente
int newclient(char *str, struct client *client)
{
    int i;
    int check;
    char cpf[128];
    int cpferro = 0;
    int accounttype = 0;
    float valor;
    while (1)
    {
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
        }
        else
        {
            cpferro = 1;
        }
        if (!cpferro)
        {
            strcpy(client->cpf, str);
            break;
        }
        printf("\n");
    }
    while (1)
    {
        if (input("Digite seu nome:\n-> ", client->name, 512))
        {
            for (int i = 0; i < strlen(client->name); i++)
            {
                if (client->name[i] == '\n')
                {
                    client->name[i] = '\0';
                }
            }

            break;
        }
        printf("\n");
    }
    while (1)
    {
        if (input("Digite sua senha: (deve conter ao menos uma letra maiúscula e um número)\n-> ", str, 1024))
        {
            // Verificar senha
            int upperpass = 0;
            int numberpass = 0;

            for (i = 0; i < strlen(str); i++)
            {
                printf("%c", *(str + i));
                if (isupper(*(str + i)))
                {
                    upperpass = 1;
                }
                if (isdigit(*(str + i)))
                {
                    numberpass = 1;
                }
            }
            if (numberpass == 1 && upperpass == 1)
            {
                // Criptografar senha
                encrypt(str, key);
                strcpy(client->passwd, str);
                break;
            }
            else
            {
                printf("Senha fraca. Tente novamente.");
            }
        }
        printf("\n");
    }
    while (1)
    {
        accounttype = intinput("Qual o tipo de conta?\n1. Comum\n2. Plus\n-> ");
        if (accounttype == 1 || accounttype == 2)
        {
            client->accounttype = accounttype;
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
            // Converter pra float
            valor = strtof(str, NULL);
            client->money = valor;

            // "inicialização" do extrato
            client->extr.detailcount = 0;
            break;
        }
    }
    return 0;
}

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
            printf("Senha (criptografada): %s\n", clients[i].passwd);
            printf("\n");
        }
    }
    return 0;
}

// Achar id do cliente para usar em outras funcs
int validateclient(char *str, int numClients, struct client *clients, int requirePW)
{
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
}

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
    float val = strtof(str, &end);
    if (clients[id].extr.detailcount == 100)
    {
        resetextr(clients, id);
        sprintf(clients[id].extr.details[99], "Depósito - R$%.2f\n", val);
    }
    else
    {
        sprintf(clients[id].extr.details[clients[id].extr.detailcount], "Depósito - R$%.2f\n", val);
        clients[id].extr.detailcount++;
    }
    clients[id].money += val;
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
        float val = strtof(str, &end);
        int check = 0;
        float taxa = 0.0;
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
            if (clients[id].extr.detailcount == 100)
            {
                resetextr(clients, id);
                sprintf(clients[id].extr.details[99], "Débito - R$%.2f - Taxa - R$%.2f\n", val, taxa);
            }
            else
            {
                sprintf(clients[id].extr.details[clients[id].extr.detailcount], "Débito - R$%.2f - Taxa - R$%.2f\n", val, taxa);
                clients[id].extr.detailcount++;
            }
        }
        else
        {
            printf("Sem saldo suficiente na conta.\n");
            return 1;
        }
    }
    return 0;
}

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

int savedata(struct client *clients, int *numClients, int sizeclients, int sizenum)
{
    FILE *bin = fopen("save.bin", "wb");
    if (bin == NULL)
    {
        printf("Erro ao abrir arquivo para salvar.\n");
        return 1;
    }
    if (fwrite(clients, sizeclients, sizeclients / sizeof(struct client), bin) != 1)
    {
        printf("Erro ao escrever no arquivo. (clients)\n");
        fclose(bin);
        return 2;
    }
    if (fwrite(numClients, sizenum, 1, bin) != 1)
    {
        printf("Erro ao escrever no arquivo. (numclients)\n");
        fclose(bin);
        return 2;
    }
    fclose(bin);
    return 0;
}

int loaddata(struct client *clients, int *numClients, int sizeclients, int sizenum)
{
    FILE *bin = fopen("save.bin", "rb");
    if (bin == NULL)
    {
        printf("Erro ao abrir arquivo para leitura.\n");
        return 1;
    }
    if (fread(clients, sizeclients, sizeclients/sizeof(struct client), bin) != 1)
    {
        printf("Erro ao ler o arquivo. (clients)\n");
        fclose(bin);
        return 2;
    }
    if (fread(numClients, sizenum, 1, bin) != 1)
    {
        printf("Erro ao ler o arquivo. (numclients)\n");
        fclose(bin);
        return 2;
    }
    fclose(bin);
    return 0;
}