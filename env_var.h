
typedef struct env_var env_var;

struct env_var {
  char * name;
  char * value;
  env_var * next;
};

void set_var(char * name, char * value);
void rename_var(char * name, char * new_name);
void del_var(char * name);
void print_vars();
void delete_vars();
void _del_var(env_var * var);
