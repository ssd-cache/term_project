#define MAXCOMMAND 100
#define HASHELEMENTS 10

struct htab {
	struct htab *child;
	struct htab *parent;
	char key[MAXCOMMAND];
	char data[HASHELEMENTS];
};
#ifdef __cplusplus 
extern "C" {
	unsigned int hash(char *);
	struct htab *addhash(char *, char *);
	struct htab *findhash(char *);
	int delhash(char *);
	void hashprofile(int);

	int gettoken(char *);

	void ungetch(int);
	int getch(void);
}
#endif