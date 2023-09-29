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
int passwdcompare(char *str1, char *str2)
{
    int result = strcmp(str1, str2);
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
int newclient(char *str, int *numclients, struct client *client, struct extrato *extrato)
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
            for (int i = 0; i < 12; i++)
            {
                if (*(str + i) != '1' && *(str + i) != '2' && *(str + i) != '3' && *(str + i) != '4' && *(str + i) != '5' && *(str + i) != '6' && *(str + i) != '7' && *(str + i) != '8' && *(str + i) != '9' && i != 11)
                {
                    cpferro = 1;
                    break;
                }
            }
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
            break;
        }
    }
    strcpy(extrato->cpf,client->cpf);
    numclients++;
    return 0;
}

int listclients(int numClients,struct client *clients)
{
    printf("Lista de Clientes:\n");

    for (int i = 0; i < numClients + 1; i++)
    {
        if (clients[i].accounttype == 1 || clients[i].accounttype == 2)
        {
            printf("Cliente %d:\n", i + 1);
            printf("Nome: %s\n", clients[i].name);
            printf("CPF: %s\n", clients[i].cpf);
            printf("Tipo de Conta: ",);
            if(clients[i].accounttype == 1)
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

int deleteclient(char *str,int numClients,struct client *clients, struct extrato *extratos)
{
    input("Digite seu CPF (somente números)\n-> ", str, 1024);
    int id = -1;
    for (int i = 0; i < numClients + 1; i++)
    {
        if(passwdcompare(str,clients[i].cpf))
        {
            id = i;
            break;
        }
    }
    if(id == -1)
    {
        printf("Cliente não encontrado.\n");
        return 1;
    }
    char pass[512];
    input("Digite sua senha:\n-> ", pass, 512);
    encrypt(pass,key);
    if(passwdcompare(pass,clients[id].passwd))
    {
        int c = intinput("Deseja mesmo deletar sua conta?\n1 - Sim\n2 - Não\n-> ");
        if(c == 2)
        {
            return 1;
        }
        for (int i = 0; i < numClients + 1; i++)
        {
            clients[i].accounttype = clients[i + 1].accounttype;
            strcpy(clients[i].cpf,clients[i+1].cpf);
            strcpy(clients[i].name,clients[i+1].name);
            strcpy(clients[i].passwd,clients[i+1].passwd);
            clients[i].money = clients[i + 1].money;
            strcpy(extratos[i].cpf,extratos[i + 1].cpf);
            for(int j = 0; j < 100; j++)
            {
                strcpy(extratos[i].details[j],extratos[i + 1].details[j]);
            }
        }
        
    }

    return 0;
}