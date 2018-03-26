// Shell.
#include "types.h"
#include "user.h"
#include "fcntl.h"

// Parsed command representation
#define NULL  0
#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5

#define MAXARGS 10
#define MAX_HISTORY 16
#define NULL 0
/*******************************functions added by Noy - Linkedlist for history**************************************/

typedef struct node{
	int cmd_num;
	char* cmd_desc;
	struct node* next;
}node;

typedef struct linkedlist{
	struct node* first;
	struct node* last;
}linkedlist;

linkedlist* list_append(linkedlist* mylist, node* addnode){

    linkedlist* new_list = mylist;
    if(new_list == NULL){
	new_list = malloc(sizeof(linkedlist));
	new_list->first = addnode;
	new_list->last = addnode;

    }
     else{
         node* ptr = new_list->first;
	 if(ptr == NULL){
            ptr = addnode;
	}
        else {
	 while(ptr->next != NULL)
               ptr = ptr->next;
               if(ptr->next == NULL){
                  ptr->next = addnode;
		  new_list->last = addnode;
                }
            }
	}
            return new_list;
    }

char* print_last_k(linkedlist* mylist , int size){
        node *curr = mylist->first;
        int _size = size;
        char* result =NULL;
        while((curr != NULL)&&(_size > 1 )){            
		   _size--;
                   curr = curr->next;
				}
        if(curr !=NULL && _size == 1){
            result = curr->cmd_desc;
        }
        return result;
}		
			
void print_all_history(linkedlist* mylist , int size){
        node *curr = mylist->first;
        int _size = size;
        while((curr != NULL)&&(_size > 0 )){            
                printf(2, "%d.%s \n", curr->cmd_num ,curr->cmd_desc);	   
                _size--;
                curr = curr->next;
                }      
           
        }

void node_free(node* node_){
	if(node_!=NULL){

        free(node_->cmd_desc);
        free(node_);
	}
	return;
}
linkedlist* delete_first(linkedlist* history_list){

    if(history_list == NULL)
        return history_list;
    else{
        node* tmp = history_list->first->next;
        node* to_delete = history_list->first;
        history_list->first = tmp;
        
        node_free(to_delete);
        return history_list;
    }
}
void list_free(linkedlist *mylist){
	 if(mylist != NULL){
	   node *curr_node = mylist->first;
	   node *tmp;
     while(curr_node!=NULL){
                tmp = curr_node;
                curr_node = curr_node->next;
                node_free(tmp);

            }

        }

	free(mylist);
        return;
}

/*******************************functions added by Noy - Linkedlist for history**************************************/
struct cmd {
  int type;
};

struct execcmd {
  int type;
  char *argv[MAXARGS];
  char *eargv[MAXARGS];
};

struct redircmd {
  int type;
  struct cmd *cmd;
  char *file;
  char *efile;
  int mode;
  int fd;
};

struct pipecmd {
  int type;
  struct cmd *left;
  struct cmd *right;
};

struct listcmd {
  int type;
  struct cmd *left;
  struct cmd *right;
};

struct backcmd {
  int type;
  struct cmd *cmd;
};

int fork1(void);  // Fork but panics on failure.
void panic(char*);
struct cmd *parsecmd(char*);

// Execute cmd.  Never returns.
void
runcmd(struct cmd *cmd)
{
  int p[2];
  struct backcmd *bcmd;
  struct execcmd *ecmd;
  struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if(cmd == 0)
    exit();

  switch(cmd->type){
  default:
    panic("runcmd");

  case EXEC:
    ecmd = (struct execcmd*)cmd;
    if(ecmd->argv[0] == 0)
      exit();
    exec(ecmd->argv[0], ecmd->argv);
    printf(2, "exec %s failed\n", ecmd->argv[0]);
    break;

  case REDIR:
    rcmd = (struct redircmd*)cmd;
    close(rcmd->fd);
    if(open(rcmd->file, rcmd->mode) < 0){
      printf(2, "open %s failed\n", rcmd->file);
      exit();
    }
    runcmd(rcmd->cmd);
    break;

  case LIST:
    lcmd = (struct listcmd*)cmd;
    if(fork1() == 0)
      runcmd(lcmd->left);
    wait();
    runcmd(lcmd->right);
    break;

  case PIPE:
    pcmd = (struct pipecmd*)cmd;
    if(pipe(p) < 0)
      panic("pipe");
    if(fork1() == 0){
      close(1);
      dup(p[1]);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->left);
    }
    if(fork1() == 0){
      close(0);
      dup(p[0]);
      close(p[0]);
      close(p[1]);
      runcmd(pcmd->right);
    }
    close(p[0]);
    close(p[1]);
    wait();
    wait();
    break;

  case BACK:
    bcmd = (struct backcmd*)cmd;
    if(fork1() == 0)
      runcmd(bcmd->cmd);
    break;
  }
  exit();
}

int
getcmd(char *buf, int nbuf)
{
  printf(2, "$ ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

int
main(void)
{
  static char buf[100];
  int fd;  
  int commands_counter = 0;
  struct linkedlist *history_list = NULL;

  // Ensure that three file descriptors are open.
  while((fd = open("console", O_RDWR)) >= 0){
    if(fd >= 3){
      close(fd);
      break;
    }
  }

  // Read and run input commands.
  while(getcmd(buf, sizeof(buf)) >= 0) {
    //noy added for hisotry function
    commands_counter = commands_counter + 1;
    struct node *set_link = NULL;
    set_link = (struct node*) malloc(sizeof(struct node));
    set_link->cmd_num = commands_counter;
    set_link->cmd_desc = malloc(strlen(buf)+1);
    strcpy(set_link->cmd_desc, buf);
    set_link->next = NULL;

    if(commands_counter < MAX_HISTORY){
       history_list = list_append(history_list, set_link);
    }
    else{
        delete_first(history_list);
        history_list = list_append(history_list, set_link);

        }
     if(strcmp("history\n", buf) == 0){
        print_all_history(history_list , 16);
        continue;
    }

 

    // Check if user is setting a variable.
    int needle = (int) strchr(buf, '=');
    if (needle != NULL) {
      int name_length = (needle - ((int) buf));
      // User is setting a variable.

      // Set room for the value.
      char *variable_name = (char *) malloc((sizeof(char) * name_length) + 1);
      char *variable_value = (char *)malloc(sizeof(char) * (strlen(buf) - name_length - 1));
      memmove(variable_name, buf, name_length);
      memmove(variable_value, buf+name_length+1, (strlen(buf) - name_length - 1));
      gsetvariable(variable_name, variable_value);
      continue;
    }

      // Check for $ signs, to replace variables.
      int isVariableName = -1;
      for (int index = 0; index < strlen(buf); index++) {
        if (isVariableName > -1) {
          // Check if variable name has ended.
          if
              (!(
                    (buf[index] >= 65 && buf[index] <= 90)
                ||  (buf[index] >= 97 && buf[index] <= 122)
              )) {
                // Variable name ended.
                char * var_name =  (char *) malloc(sizeof(char) * 128);
                char * var_value = (char *) malloc(sizeof(char) * 128);
                memmove(var_name, buf + isVariableName, (index - isVariableName));
                ggetvariable(var_name, var_value);
                // Rebuild buf.
                int newbuf_length = strlen(buf) + strlen(var_value) - (strlen(var_name) - 1);
                char * new_buf = (char *) malloc(sizeof(char) * newbuf_length);
                if (isVariableName - 1 > 0) {
                  // Copy buffer until the $ character.
                  memmove(new_buf, buf, isVariableName);
                }
                // Copy value of variable_value where $var_name is present (except for last char == '\0')..
                memmove(new_buf+(isVariableName-1), var_value, strlen(var_value) - 1);
                // Copy the rest of the buffer.
                memmove(new_buf+(isVariableName-1) + strlen(var_value)-1, buf+(isVariableName)+strlen(var_name), strlen(buf+(isVariableName)+strlen(var_name)));
                for (int indexB = 0; indexB < strlen(new_buf); indexB ++) {
                  if (indexB != strlen(new_buf) - 1 && *(new_buf + indexB) == '\0') {
                    buf[indexB] = ' ';
                    continue;
                  }
                  buf[indexB] = *(new_buf + indexB);
                }
                buf[strlen(new_buf)] = '\0';
                // We changed the current buf, go back to the start.
                index = -1;
                isVariableName = -1;
              }
        }
        if (buf[index] == '$') {
          // check until a non-letter character appears.
          // (could be 0, but in every use we add 1 anyway)
          isVariableName = index + 1;
          gprintvariables();
        }
      }
      
    if(buf[0] == 'h' && buf[1] == 'i' && buf[2] == 's' && buf[3] == 't' && buf[4] == 'o' && buf[5] == 'r' && buf[6] == 'y' && buf[7] == ' '&& buf[8] == '-' && buf[9] == 'l'){
        int size = atoi(buf+11);        
        buf = print_last_k(history_list , size);
   
        
  }


    if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' '){
      // Chdir must be called by the parent, not the child.
      buf[strlen(buf)-1] = 0;  // chop \n
      if(chdir(buf+3) < 0){
        printf(2, "cannot cd %s\n", buf+3);

    }
      continue;
    }
    if(fork1() == 0)
      runcmd(parsecmd(buf));
    wait();
  }
  list_free(history_list);
  exit();
}

void
panic(char *s)
{
  printf(2, "%s\n", s);
  exit();
}

int
fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

//PAGEBREAK!
// Constructors

struct cmd*
execcmd(void)
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd*)cmd;
}

struct cmd*
redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
{
  struct redircmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = REDIR;
  cmd->cmd = subcmd;
  cmd->file = file;
  cmd->efile = efile;
  cmd->mode = mode;
  cmd->fd = fd;
  return (struct cmd*)cmd;
}

struct cmd*
pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = PIPE;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

struct cmd*
listcmd(struct cmd *left, struct cmd *right)
{
  struct listcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = LIST;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

struct cmd*
backcmd(struct cmd *subcmd)
{
  struct backcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  memset(cmd, 0, sizeof(*cmd));
  cmd->type = BACK;
  cmd->cmd = subcmd;
  return (struct cmd*)cmd;
}
//PAGEBREAK!
// Parsing

char whitespace[] = " \t\r\n\v";
char symbols[] = "<|>&;()";

int
gettoken(char **ps, char *es, char **q, char **eq)
{
  char *s;
  int ret;

  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  if(q)
    *q = s;
  ret = *s;
  switch(*s){
  case 0:
    break;
  case '|':
  case '(':
  case ')':
  case ';':
  case '&':
  case '<':
    s++;
    break;
  case '>':
    s++;
    if(*s == '>'){
      ret = '+';
      s++;
    }
    break;
  default:
    ret = 'a';
    while(s < es && !strchr(whitespace, *s) && !strchr(symbols, *s))
      s++;
    break;
  }
  if(eq)
    *eq = s;

  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return ret;
}

int
peek(char **ps, char *es, char *toks)
{
  char *s;

  s = *ps;
  while(s < es && strchr(whitespace, *s))
    s++;
  *ps = s;
  return *s && strchr(toks, *s);
}

struct cmd *parseline(char**, char*);
struct cmd *parsepipe(char**, char*);
struct cmd *parseexec(char**, char*);
struct cmd *nulterminate(struct cmd*);

struct cmd*
parsecmd(char *s)
{
  char *es;
  struct cmd *cmd;

  es = s + strlen(s);
  cmd = parseline(&s, es);
  peek(&s, es, "");
  if(s != es){
    printf(2, "leftovers: %s\n", s);
    panic("syntax");
  }
  nulterminate(cmd);
  return cmd;
}

struct cmd*
parseline(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parsepipe(ps, es);
  while(peek(ps, es, "&")){
    gettoken(ps, es, 0, 0);
    cmd = backcmd(cmd);
  }
  if(peek(ps, es, ";")){
    gettoken(ps, es, 0, 0);
    cmd = listcmd(cmd, parseline(ps, es));
  }
  return cmd;
}

struct cmd*
parsepipe(char **ps, char *es)
{
  struct cmd *cmd;

  cmd = parseexec(ps, es);
  if(peek(ps, es, "|")){
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es));
  }
  return cmd;
}

struct cmd*
parseredirs(struct cmd *cmd, char **ps, char *es)
{
  int tok;
  char *q, *eq;

  while(peek(ps, es, "<>")){
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a')
      panic("missing file for redirection");
    switch(tok){
    case '<':
      cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
      break;
    case '>':
      cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE, 1);
      break;
    case '+':  // >>
      cmd = redircmd(cmd, q, eq, O_WRONLY|O_CREATE, 1);
      break;
    }
  }
  return cmd;
}

struct cmd*
parseblock(char **ps, char *es)
{
  struct cmd *cmd;

  if(!peek(ps, es, "("))
    panic("parseblock");
  gettoken(ps, es, 0, 0);
  cmd = parseline(ps, es);
  if(!peek(ps, es, ")"))
    panic("syntax - missing )");
  gettoken(ps, es, 0, 0);
  cmd = parseredirs(cmd, ps, es);
  return cmd;
}

struct cmd*
parseexec(char **ps, char *es)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;

  if(peek(ps, es, "("))
    return parseblock(ps, es);

  ret = execcmd();
  cmd = (struct execcmd*)ret;

  argc = 0;
  ret = parseredirs(ret, ps, es);
  while(!peek(ps, es, "|)&;")){
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    if(tok != 'a')
      panic("syntax");
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    if(argc >= MAXARGS)
      panic("too many args");
    ret = parseredirs(ret, ps, es);
  }
  cmd->argv[argc] = 0;
  cmd->eargv[argc] = 0;
  return ret;
}

// NUL-terminate all the counted strings.
struct cmd*
nulterminate(struct cmd *cmd)
{
  int i;
  struct backcmd *bcmd;
  struct execcmd *ecmd;
  struct listcmd *lcmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if(cmd == 0)
    return 0;

  switch(cmd->type){
  case EXEC:
    ecmd = (struct execcmd*)cmd;
    for(i=0; ecmd->argv[i]; i++)
      *ecmd->eargv[i] = 0;
    break;

  case REDIR:
    rcmd = (struct redircmd*)cmd;
    nulterminate(rcmd->cmd);
    *rcmd->efile = 0;
    break;

  case PIPE:
    pcmd = (struct pipecmd*)cmd;
    nulterminate(pcmd->left);
    nulterminate(pcmd->right);
    break;

  case LIST:
    lcmd = (struct listcmd*)cmd;
    nulterminate(lcmd->left);
    nulterminate(lcmd->right);
    break;

  case BACK:
    bcmd = (struct backcmd*)cmd;
    nulterminate(bcmd->cmd);
    break;
  }
  return cmd;
}
