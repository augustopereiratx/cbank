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
int newclient(char *str, int *numclients, struct client *client)
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
            printf("%s", str);
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
    numclients++;
    return 0;
}

int listclients(int *numclients, struct client *clients)
{
    for (int i = 0; i < numclients; i++)
    {
        printf("Cliente %d:\n",i + 1);
        printf("CPF: %s\n",*(clients + i)->cpf);
        printf("Nome: %s\n",*(clients + i)->name);
        printf("Conta: ");
        if((clients + i)->accounttype == 1)
        {
            printf("comum\n");
        }
        else
        {
            printf("plus");
        }
        printf("Valor da conta: %.2f\n",(clients + i)->money);
        printf("Senha (criptografada): %s\n",*(clients + i)->passwd);
    }   
}