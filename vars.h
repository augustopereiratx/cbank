// Descriptografar senhas (não sei como fazer bem)
extern const char key[];

struct client
{
    int accounttype;
    float money;
    char name[512];
    char cpf[512];
    char passwd[512];
    int clientid;
};
struct extrato
{
    int clientid;
    char details[100][1024];
};

