
struct lelement{
  int data;
  struct lelement*next;
};

struct pile{
  struct lelement* first;
};

struct pile* pile__init();

int pile__is_empty (const struct pile* p);

void pile__add(struct pile* p,int x);

int pile__top(struct pile *p);

struct lelement* pile__pop(struct pile* p);

void pile__free(struct pile* p);
