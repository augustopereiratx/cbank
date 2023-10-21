#define MAXCLIENTS 1000

// Descriptografar senhas (não sei como fazer bem)
extern const char key[];

struct extrato
{
    int detailcount;
    char details[100][64];
};

struct client
{
    int accounttype;
    double money;
    char name[128];
    char cpf[64];
    char passwd[128];
    struct extrato extr;
};

struct clients 
{
    struct client clients[MAXCLIENTS];
    int numclients;
};