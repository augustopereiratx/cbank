void encrypt(char *, const char *);
void decrypt(char *, const char *);
int init();
int input(char *, char *, int);
int intinput(char *);
int passwdcompare(char *,char *);
int newclient(char *str, int *, struct client *);
int listclients(int,struct client *);