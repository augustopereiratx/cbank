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
            // Conversion successful, and the entire string was used
            return (int)number;
        }
    }
    return -1;
}

// Novo Cliente
int newclient(char *str, int *numclients, struct client *clients)
{
    int nc = numclients;
    int i;
    int check;
    char cpf[12];
    int cpferro = 0;
    int accounttype = 0;
    float valor;
    while (1)
    {
        printf("Digite seu CPF (somente números)\n-> ");
        scanf("%s", cpf);

        // ChatGPT disse pra eu usar e deu certo
        getchar();

        printf("%s",cpf);
        printf("\n");
        printf("%d",strlen(cpf));

        if (strlen(cpf) == 11)
        {
            for (int i = 0; i < 11; i++)
            {
                if (!isdigit(cpf[i]))
                {
                    cpferro = 1;
                    break;
                }
            }
        }else
        {
            cpferro = 1;
        }
        if(!cpferro)
        {
            strcpy((clients + nc)->cpf,cpf);
        }
        printf("\n");
    }
    while (1)
    {
        if (input("Digite seu nome:\n-> ", str, 1024))
        {
            strcpy((clients + nc)->name, str);
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

            for (i = 0; i < 512; i++)
            {
                if (isupper((str + i)))
                {
                    upperpass = 1;
                }
                if (isdigit((str + i)))
                {
                    numberpass = 1;
                }
            }
            if (numberpass == 1 && upperpass == 1)
            {
                // Criptografar senha
                encrypt(str, key);
                strcpy((clients + nc)->passwd, str);
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
            (clients + nc)->accounttype = accounttype;
            break;
        }
    }
    while (1)
    {
        if (input("Digite o valor inicial de capital da conta:\n-> ", str, 1024))
        {
            // Vírgula pra ponto
            for (i = 0; i < 1024; i++)
            {
                if ((str + i) == ',')
                {
                    str[i] = '.';
                    break;
                }
                if ((str + i) == NULL)
                {
                    break;
                }
            }
            // Converter pra float
            valor = strtof(str, NULL);
            (clients + nc)->money = valor;
            break;
        }
    }
    numclients++;
    return 0;
}