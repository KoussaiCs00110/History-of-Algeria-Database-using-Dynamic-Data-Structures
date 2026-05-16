/* gui.c - Raylib GUI for History of Algeria Database */
#include "raylib.h"
#include "../include/data_structures.h"
#include "../include/linked_list.h"
#include "../include/stack.h"
#include "../include/bst.h"
#include "../include/recursion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/* ── colours (clean white & black theme) ── */
#define C_BG     CLITERAL(Color){245,245,245,255}
#define C_PANEL  CLITERAL(Color){230,230,230,255}
#define C_ACCENT CLITERAL(Color){30,30,30,255}
#define C_OUTPUT CLITERAL(Color){255,255,255,255}
#define C_WHITE  CLITERAL(Color){20,20,20,255}
#define C_GRAY   CLITERAL(Color){100,100,100,255}
#define C_DARK   CLITERAL(Color){240,240,240,255}

/* ── layout ── */
#define W 1280
#define H 720
#define TOP_H 50
#define LEFT_W 220
#define BOT_H 80
#define BTN_H 32
#define BTN_PAD 4
#define OUTBUF 65536
#define MAXLINES 2048

/* ── sections ── */
enum { SEC_LL=0, SEC_STK, SEC_BST, SEC_REC, SEC_COUNT };
static const char *secNames[]={"Linked List","Stack","BST","Recursion"};

/* ── global data ── */
static TList *listS=NULL, *listA=NULL;
static TStack *stack=NULL;
static TStack *eventStack=NULL;
static TTree *tree=NULL;
static char outBuf[OUTBUF];
static int outScroll=0;
static char inName[100]="", inDef[256]="", inDoB[20]="", inDoD[20]="";
static int activeField=-1;
static int curSection=0;
static int selOp=-1;
static Font appFont;

/* ── safe printf capture using pipe ── */
static int saved_stdout=-1;
static int pipefd[2]={-1,-1};
static void outAppend(const char *s);  /* forward declaration */

static void captureStart(void){
    fflush(stdout);
    if(pipe(pipefd)==-1) return;
    fcntl(pipefd[0], F_SETFL, O_NONBLOCK);
    saved_stdout=dup(STDOUT_FILENO);
    dup2(pipefd[1],STDOUT_FILENO);
    close(pipefd[1]); pipefd[1]=-1;
}
static void captureEnd(void){
    if(saved_stdout<0) return;
    fflush(stdout);
    fsync(STDOUT_FILENO);
    dup2(saved_stdout,STDOUT_FILENO);
    close(saved_stdout); saved_stdout=-1;
    /* read captured data */
    char tmp[4096];
    ssize_t n;
    while((n=read(pipefd[0],tmp,sizeof(tmp)-1))>0){
        tmp[n]=0; outAppend(tmp);
    }
    close(pipefd[0]); pipefd[0]=-1;
}

static void outClear(void){ outBuf[0]=0; outScroll=0; }
static void outAppend(const char *s){
    size_t c=strlen(outBuf), l=strlen(s), a=OUTBUF-c-1;
    if(l<a){ memcpy(outBuf+c,s,l); outBuf[c+l]=0; }
}

/* ── load data ── */
static void loadData(void){
    /* Load full records with all 4 fields */
    FILE *ff=fopen("data/history.txt","r");
    if(ff){
        char line[1024];
        while(fgets(line,sizeof(line),ff)){
            line[strcspn(line,"\n")]=0;
            char *eq=strchr(line,'=');
            if(!eq) continue;
            *eq='\0';
            char *name=line;
            char *rest=eq+1;
            /* rest = "Definition{DoB}{DoD}" */
            char def[500]="", dob[20]="", dod[20]="";
            char *b1=strchr(rest,'{');
            if(b1){
                /* definition is text before first { */
                int dlen=(int)(b1-rest); if(dlen>499) dlen=499;
                strncpy(def,rest,dlen); def[dlen]='\0';
                char *e1=strchr(b1+1,'}');
                if(e1){
                    int dbl=(int)(e1-(b1+1)); if(dbl>19) dbl=19;
                    strncpy(dob,b1+1,dbl); dob[dbl]='\0';
                    char *b2=strchr(e1+1,'{');
                    if(b2){
                        char *e2=strchr(b2+1,'}');
                        if(e2){
                            int ddl=(int)(e2-(b2+1)); if(ddl>19) ddl=19;
                            strncpy(dod,b2+1,ddl); dod[ddl]='\0';
                        }
                    }
                }
            } else {
                strncpy(def,rest,499); def[499]='\0';
            }
            listS=insertAtTail(listS,name,def,dob,dod);
            listA=insertAtTail(listA,name,def,dob,dod);
        }
        fclose(ff);
    }
    /* build stack and tree from full list */
    stack=toStack(listS);
    tree=toTree(stack);
    /* load events into a separate event stack */
    FILE *fe=fopen("data/events.txt","r");
    if(fe){
        char line[512];
        TStack *tmp=NULL;
        while(fgets(line,sizeof(line),fe)){
            line[strcspn(line,"\n")]=0;
            char *eq=strchr(line,'=');
            if(!eq) continue;
            *eq='\0';
            char *rest=eq+1;
            char *ob1=strchr(rest,'{'); char *evdate=NULL;
            if(ob1){ *ob1='\0'; evdate=ob1+1; char *cb=strchr(evdate,'}'); if(cb)*cb='\0'; }
            TStack *nd=(TStack*)malloc(sizeof(TStack));
            strncpy(nd->name,line,99); nd->name[99]='\0';
            strncpy(nd->definition,rest,499); nd->definition[499]='\0';
            nd->DoB[0]='\0'; nd->DoD[0]='\0';
            if(evdate){ strncpy(nd->event_date,evdate,19); nd->event_date[19]='\0'; }
            else nd->event_date[0]='\0';
            nd->next=tmp; tmp=nd;
        }
        fclose(fe);
        eventStack=tmp;
    }
}

/* ── button helper ── */
static bool guiBtn(int x,int y,int w,int h,const char *txt,bool active){
    Rectangle r={(float)x,(float)y,(float)w,(float)h};
    bool hover=CheckCollisionPointRec(GetMousePosition(),r);
    Color bg=active?C_ACCENT:(hover?CLITERAL(Color){200,200,200,255}:C_PANEL);
    DrawRectangleRec(r,bg);
    DrawRectangleLinesEx(r,1.0f,C_ACCENT);
    int tw=(int)MeasureTextEx(appFont,txt,16,1).x;
    DrawTextEx(appFont,txt,(Vector2){(float)(x+(w-tw)/2),(float)(y+(h-16)/2)},16,1,active?CLITERAL(Color){255,255,255,255}:C_WHITE);
    return hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

/* ── text input field ── */
static void guiField(int x,int y,int w,const char *label,char *buf,int bufSz,int fid){
    DrawTextEx(appFont,label,(Vector2){(float)x,(float)(y-15)},13,1,C_GRAY);
    Rectangle r={(float)x,(float)y,(float)w,24.0f};
    bool hover=CheckCollisionPointRec(GetMousePosition(),r);
    if(hover && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) activeField=fid;
    DrawRectangleRec(r,activeField==fid?CLITERAL(Color){255,255,255,255}:C_DARK);
    DrawRectangleLinesEx(r,1.0f,activeField==fid?C_ACCENT:C_GRAY);
    DrawTextEx(appFont,buf,(Vector2){(float)(x+4),(float)(y+4)},15,1,C_WHITE);
    if(activeField==fid){
        int key=GetCharPressed();
        while(key>0){
            int len=(int)strlen(buf);
            if(len<bufSz-1){ buf[len]=(char)key; buf[len+1]='\0'; }
            key=GetCharPressed();
        }
        if(IsKeyPressed(KEY_BACKSPACE)){
            int len=(int)strlen(buf);
            if(len>0) buf[len-1]='\0';
        }
        if(IsKeyPressed(KEY_TAB)){ activeField=(fid+1)%4; }
    }
}

/* ── helpers to print data to outBuf ── */
static const char *safeStr(const char *s){ return (s&&s[0])?s:"-"; }
static void printListToBuf(TList *h,int max){
    int n=0;
    while(h&&n<max){
        char line[800];
        snprintf(line,sizeof(line),"%s\n  Def: %s\n  DoB: %s  DoD: %s\n",
            safeStr(h->name),safeStr(h->definition),safeStr(h->DoB),safeStr(h->DoD));
        outAppend(line); h=h->next; n++;
    }
    if(h) outAppend("... (more entries)\n");
}
static void printStackToBuf(TStack *s,int max){
    int n=0;
    while(s&&n<max){
        char line[800];
        snprintf(line,sizeof(line),"%s\n  Def: %s\n  DoB: %s  DoD: %s\n",
            safeStr(s->name),safeStr(s->definition),safeStr(s->DoB),safeStr(s->DoD));
        outAppend(line); s=s->next; n++;
    }
}
/* print list with dates + age */
static void printListDatesToBuf(TList *h,int max){
    int n=0;
    while(h&&n<max){
        char line[700];
        date *dob=convert(h->DoB);
        date *dod=convert(h->DoD);
        int age=agecalculate(dob,dod);
        snprintf(line,sizeof(line),"%-30s | DoB:%-12s DoD:%-12s Age:%d\n",
            safeStr(h->name),safeStr(h->DoB),safeStr(h->DoD),age);
        if(dob) free(dob);
        if(dod) free(dod);
        outAppend(line); h=h->next; n++;
    }
    if(h) outAppend("... (more entries)\n");
}
/* search list by DoB and write matches to outBuf */
static void searchByDoBToBuf(TList *s, const char *dob){
    int found=0;
    TList *cur=s;
    while(cur){
        if(cur->DoB[0] && strstr(cur->DoB,dob)){
            char line[700];
            snprintf(line,sizeof(line),"Name: %s\nDoB: %s\nDoD: %s\n\n",
                safeStr(cur->name),safeStr(cur->DoB),safeStr(cur->DoD));
            outAppend(line); found++;
        }
        cur=cur->next;
    }
    if(!found) outAppend("No match found.\n");
}
/* search list by DoD and write matches to outBuf */
static void searchByDoDToBuf(TList *s, const char *dod){
    int found=0;
    TList *cur=s;
    while(cur){
        if(cur->DoD[0] && strstr(cur->DoD,dod)){
            char line[700];
            snprintf(line,sizeof(line),"Name: %s\nDoB: %s\nDoD: %s\n\n",
                safeStr(cur->name),safeStr(cur->DoB),safeStr(cur->DoD));
            outAppend(line); found++;
        }
        cur=cur->next;
    }
    if(!found) outAppend("No match found.\n");
}
static void printQueueToBuf(TQueue *q,int max){
    if(!q||!q->front) return;
    NodeQueue *c=q->front; int n=0;
    while(c&&n<max){
        char line[700];
        snprintf(line,sizeof(line),"%-30s | %s %s\n",safeStr(c->name),
            c->definition[0]?c->definition:"",
            c->DoB[0]?c->DoB:"");
        outAppend(line); c=c->next; n++;
    }
}
/* tree print: in-order */
static void treeToBuf(TTree *t,int depth,int max,int *cnt){
    if(!t||*cnt>=max) return;
    treeToBuf(t->left,depth+1,max,cnt);
    if(*cnt<max){
        char line[700]; char pad[200]="";
        for(int i=0;i<depth&&i<50;i++) strcat(pad,"  ");
        snprintf(line,sizeof(line),"%s%s (DoB:%s DoD:%s)\n",pad,t->name,safeStr(t->DoB),safeStr(t->DoD));
        outAppend(line); (*cnt)++;
    }
    treeToBuf(t->right,depth+1,max,cnt);
}
/* tree print: pre-order */
static void treeToBufPre(TTree *t,int depth,int max,int *cnt){
    if(!t||*cnt>=max) return;
    char line[700]; char pad[200]="";
    for(int i=0;i<depth&&i<50;i++) strcat(pad,"  ");
    snprintf(line,sizeof(line),"%s%s (DoB:%s DoD:%s)\n",pad,t->name,safeStr(t->DoB),safeStr(t->DoD));
    outAppend(line); (*cnt)++;
    treeToBufPre(t->left,depth+1,max,cnt);
    treeToBufPre(t->right,depth+1,max,cnt);
}
/* tree print: post-order */
static void treeToBufPost(TTree *t,int depth,int max,int *cnt){
    if(!t||*cnt>=max) return;
    treeToBufPost(t->left,depth+1,max,cnt);
    treeToBufPost(t->right,depth+1,max,cnt);
    char line[700]; char pad[200]="";
    for(int i=0;i<depth&&i<50;i++) strcat(pad,"  ");
    snprintf(line,sizeof(line),"%s%s (DoB:%s DoD:%s)\n",pad,t->name,safeStr(t->DoB),safeStr(t->DoD));
    outAppend(line); (*cnt)++;
}

/* ── operation definitions per section ── */
/* Linked List ops */
static const char *llOps[]={"Show Personalities","Show Dates","Search by DoB",
    "Search by DoD","Sort Alphabetically","Sort by Name Length","Sort by Age",
    "Delete Personality","Similar Personality","Palindrome Names",
    "Merge Nodes","Sort Queue by Words","Sort Queue by Age","Convert to Queue"};
#define LL_COUNT 14

static void execLL(int op){
    outClear();
    char title[128]; snprintf(title,128,"=== %s ===\n",llOps[op]); outAppend(title);
    switch(op){
    case 0: printListToBuf(listS,200); break;
    case 1: printListDatesToBuf(listA,200); break;
    case 2:
        if(!inDoB[0]){outAppend("Enter DoB field first (e.g. 23/09/1808 or 1808)\n");break;}
        searchByDoBToBuf(listA,inDoB); break;
    case 3:
        if(!inDoD[0]){outAppend("Enter DoD field first (e.g. 26/05/1883 or 1883)\n");break;}
        searchByDoDToBuf(listA,inDoD); break;
    case 4: listS=sortWord(listS); outAppend("Sorted alphabetically.\n"); printListToBuf(listS,100); break;
    case 5: listS=sortWord2(listS); outAppend("Sorted by name length.\n"); printListToBuf(listS,100); break;
    case 6: listA=sortPersonality(listA); outAppend("Sorted by age (ascending).\n"); printListDatesToBuf(listA,100); break;
    case 7:
        if(!inName[0]){outAppend("Enter Name first\n");break;}
        { FILE *f=fopen("data/history.txt","r");
          if(f){ listS=deletepersonality(f,listS,listA,inName); outAppend("Deleted.\n"); }
        } break;
    case 8:
        if(!inDoB[0]){outAppend("Enter year in DoB field\n");break;}
        { TList *r=similarPersonality(listA,inDoB);
          if(r) printListToBuf(r,100); else outAppend("None found.\n");
        } break;
    case 9: { TList *p=palindromeName(listS); if(p) printListToBuf(p,100); else outAppend("No palindromes found.\n"); } break;
    case 10:{ TList *m=mergeNodes(listS,listA); if(m) printListToBuf(m,100); else outAppend("Merge returned empty.\n"); } break;
    case 11:{ TQueue *q=sName(listS); if(q){ printQueueToBuf(q,100); } else outAppend("Empty.\n"); } break;
    case 12:{ TQueue *q=ageP(listA); if(q){ printQueueToBuf(q,100); } else outAppend("Empty.\n"); } break;
    case 13:{
        TList *merged=NULL; TList *cs=listS,*ca=listA;
        while(cs&&ca){ merged=insertAtTail(merged,cs->name,cs->definition,ca->DoB,ca->DoD); cs=cs->next;ca=ca->next; }
        TQueue *q=toQueue(merged); if(q) printQueueToBuf(q,100); else outAppend("Empty.\n");
    } break;
    }
}

/* Stack ops */
static const char *stkOps[]={"Show Stack","Search Name","Sort Alpha","Delete Name",
    "Update Entry","Stack to Queue","Stack to List","Add Name","Sort by Definition",
    "Pronunciation Stack","Smallest Def","Continuous Search","Is Killed?","Reverse Stack"};
#define STK_COUNT 14

static void execSTK(int op){
    outClear();
    char title[128]; snprintf(title,128,"=== %s ===\n",stkOps[op]); outAppend(title);
    switch(op){
    case 0: printStackToBuf(stack,200); break;
    case 1:
        if(!inName[0]){outAppend("Enter Name\n");break;}
        { TStack *r=getInfoPersonality(stack,inName);
          if(r){ char b[700]; snprintf(b,700,"Name: %s\nDef: %s\nDoB: %s\nDoD: %s\n",r->name,r->definition,r->DoB,r->DoD); outAppend(b); free(r); }
          else outAppend("Not found.\n");
        } break;
    case 2: stack=sortNameStack(stack); outAppend("Sorted.\n"); printStackToBuf(stack,100); break;
    case 3:
        if(!inName[0]){outAppend("Enter Name\n");break;}
        stack=deleteName(stack,inName); outAppend("Deleted.\n"); break;
    case 4:
        if(!inName[0]){outAppend("Enter Name,Def,DoB,DoD\n");break;}
        stack=updateStack(stack,inName,inDef,inDoB,inDoD); outAppend("Updated.\n"); break;
    case 5: { TQueue *q=stackToQueue(stack); if(q) printQueueToBuf(q,100); else outAppend("Empty.\n"); } break;
    case 6: { TList *l=stackToList(stack); if(l) printListToBuf(l,100); else outAppend("Empty.\n"); } break;
    case 7:
        if(!inName[0]){outAppend("Enter Name,Def,DoB,DoD\n");break;}
        stack=addNameStack(stack,inName,inDef,inDoB,inDoD); outAppend("Added.\n"); break;
    case 8: stack=definitionStack(stack); outAppend("Sorted by def words.\n"); printStackToBuf(stack,100); break;
    case 9: {
        TStack *src=eventStack?eventStack:stack;
        TStack *sh=NULL,*lo=NULL;
        pronunciationStack(src,&sh,&lo);
        outAppend("--- Short definitions ---\n");
        if(sh) printStackToBuf(sh,50); else outAppend("(none)\n");
        outAppend("--- Long definitions ---\n");
        if(lo) printStackToBuf(lo,50); else outAppend("(none)\n");
    } break;
    case 10:{
        TStack *src=eventStack?eventStack:stack;
        char *s=getSmallest(src);
        if(s&&s[0]){outAppend("Smallest event date: ");outAppend(s);outAppend("\n");free(s);}
        else outAppend("No events loaded.\n");
    } break;
    case 11:{
        TStack *src=eventStack?eventStack:stack;
        captureStart(); continuousSearch(src); captureEnd();
        if(!outBuf[0] || strlen(outBuf)<50) outAppend("No overlapping events found.\n");
    } break;
    case 12:
        if(!inName[0]){outAppend("Enter Name\n");break;}
        { TStack *r=getInfoPersonality(stack,inName);
          if(r){ outAppend(isPersonalityKilled(r->definition)?"YES - was killed\n":"NO - not killed\n"); free(r); }
          else outAppend("Not found.\n");
        } break;
    case 13: stack=recRevStack(stack); outAppend("Reversed.\n"); printStackToBuf(stack,100); break;
    }
}

/* BST ops */
static const char *bstOps[]={"In-Order","Pre-Order","Post-Order","Search Name",
    "Add to BST","Delete from BST","Update BST","Height & Size",
    "LCA","Count Range","In-Order Succ","Mirror","Is Balanced?","Merge Trees"};
#define BST_COUNT 14

static void execBST(int op){
    outClear();
    char title[128]; snprintf(title,128,"=== %s ===\n",bstOps[op]); outAppend(title);
    switch(op){
    case 0: { int cnt=0; treeToBuf(tree,0,200,&cnt); } break;
    case 1: { int cnt=0; treeToBufPre(tree,0,200,&cnt); } break;
    case 2: { int cnt=0; treeToBufPost(tree,0,200,&cnt); } break;
    case 3:
        if(!inName[0]){outAppend("Enter Name\n");break;}
        { TTree *r=getInfoNameTree(tree,inName);
          if(r){ char b[700]; snprintf(b,700,"Name: %s\nDef: %s\nDoB: %s DoD: %s\n",r->name,r->definition,r->DoB,r->DoD); outAppend(b); }
          else outAppend("Not found.\n");
        } break;
    case 4:
        if(!inName[0]){outAppend("Enter Name,DoB,DoD\n");break;}
        tree=addNameBST(tree,inName,inDoB,inDoD); outAppend("Added.\n"); break;
    case 5:
        if(!inName[0]){outAppend("Enter Name\n");break;}
        tree=deleteNameBST(tree,inName); outAppend("Deleted.\n"); break;
    case 6:
        if(!inName[0]){outAppend("Enter Name,Def,DoB,DoD\n");break;}
        tree=updateNameBST(tree,inName,inDef,inDoB,inDoD); outAppend("Updated.\n"); break;
    case 7: {
        int h=0,s=0; heightSizeBST(tree,&h,&s);
        char b[128]; snprintf(b,128,"Height: %d\nSize: %d\n",h,s); outAppend(b);
    } break;
    case 8:
        if(!inName[0]||!inDef[0]){outAppend("Enter Name1 in Name, Name2 in Def\n");break;}
        { TTree *r=lowestCommonAncestor(tree,inName,inDef);
          if(r){char b[200];snprintf(b,200,"LCA: %s\n",r->name);outAppend(b);}
          else outAppend("Not found.\n");
        } break;
    case 9:
        if(!inName[0]||!inDef[0]){outAppend("Enter low in Name, high in Def\n");break;}
        { int c=countNodesRange(tree,inName,inDef);
          char b[100]; snprintf(b,100,"Nodes in range: %d\n",c); outAppend(b);
        } break;
    case 10:
        if(!inName[0]){outAppend("Enter Name\n");break;}
        { TTree *r=inOrderSuccessor(tree,inName);
          if(r){char b[200];snprintf(b,200,"Successor: %s\n",r->name);outAppend(b);}
          else outAppend("No successor.\n");
        } break;
    case 11: { TTree *m=BSTMirror(tree); outAppend("Mirror created:\n"); int cnt=0; treeToBuf(m,0,100,&cnt); } break;
    case 12: outAppend(isBalancedBST(tree)?"Tree IS balanced.\n":"Tree is NOT balanced.\n"); break;
    case 13: { TTree *m=BSTMerge(tree,NULL); outAppend("Merge done (self-merge demo).\n"); int cnt=0; treeToBuf(m,0,100,&cnt); } break;
    }
}

/* Recursion ops */
static const char *recOps[]={"Count Occurrence","Name Permutation","Subsequences",
    "Distinct Subseq","Is Palindrome?"};
#define REC_COUNT 5

static void execREC(int op){
    outClear();
    char title[128]; snprintf(title,128,"=== %s ===\n",recOps[op]); outAppend(title);
    switch(op){
    case 0:
        if(!inName[0]){outAppend("Enter Name\n");break;}
        { FILE *f=fopen("data/history.txt","r");
          if(f){ int c=countOccurence(f,inName); fclose(f);
            char b[100]; snprintf(b,100,"Occurrences: %d\n",c); outAppend(b); }
        } break;
    case 1:
        if(!inName[0]){outAppend("Enter Name\n");break;}
        { char tmp[100]; strncpy(tmp,inName,99); tmp[99]=0;
          int len=strlen(tmp);
          if(len>8){ outAppend("Name too long for permutation demo (max 8 chars)\n"); break; }
          captureStart(); namePermutation(tmp,0,len-1); captureEnd();
        } break;
    case 2:
        if(!inName[0]){outAppend("Enter word in Name\n");break;}
        { char tmp[100]; strncpy(tmp,inName,99); tmp[99]=0;
          if(strlen(tmp)>12){ outAppend("Too long (max 12 chars)\n"); break; }
          captureStart(); subseqName(tmp); captureEnd();
        } break;
    case 3:
        if(!inName[0]){outAppend("Enter event in Name\n");break;}
        { int c=distinctSubseqWord(inName);
          char b[100]; snprintf(b,100,"Distinct subsequences: %d\n",c); outAppend(b);
        } break;
    case 4:
        if(!inName[0]){outAppend("Enter word in Name\n");break;}
        { int len=strlen(inName);
          bool p=isPalindromeWord(inName,0,len-1);
          outAppend(p?"YES - is palindrome\n":"NO - not palindrome\n");
        } break;
    }
}

/* ── execute current operation ── */
static void executeOp(void){
    if(selOp<0) return;
    switch(curSection){
    case SEC_LL:  execLL(selOp); break;
    case SEC_STK: execSTK(selOp); break;
    case SEC_BST: execBST(selOp); break;
    case SEC_REC: execREC(selOp); break;
    }
}

/* ── MAIN ── */
int main(void){
    InitWindow(W,H,"History of Algeria Database");
    SetTargetFPS(60);
    /* Load clear TTF font */
    appFont=LoadFontEx("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",20,NULL,0);
    SetTextureFilter(appFont.texture,TEXTURE_FILTER_BILINEAR);
    loadData();
    outAppend("Welcome! Select a section and operation.\n");

    while(!WindowShouldClose()){
        /* scroll */
        float wheel=GetMouseWheelMove();
        if(wheel!=0) outScroll-=(int)(wheel*40);
        if(outScroll<0) outScroll=0;

        /* click outside fields */
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            Vector2 m=GetMousePosition();
            if(m.y<H-BOT_H) activeField=-1;
        }

        BeginDrawing();
        ClearBackground(C_BG);

        /* ── top bar ── */
        DrawRectangle(0,0,W,TOP_H,C_PANEL);
        DrawTextEx(appFont,"History of Algeria Database",(Vector2){20,12},22,1,C_ACCENT);

        /* ── left panel: section buttons ── */
        int ly=TOP_H+10;
        for(int i=0;i<SEC_COUNT;i++){
            if(guiBtn(5,ly,LEFT_W-10,BTN_H,secNames[i],curSection==i)){
                curSection=i; selOp=-1;
            }
            ly+=BTN_H+BTN_PAD;
        }
        ly+=10;
        DrawLine(5,ly,LEFT_W-5,ly,C_ACCENT); ly+=10;

        /* ── operation buttons ── */
        const char **ops=NULL; int opCnt=0;
        switch(curSection){
            case SEC_LL:  ops=llOps;  opCnt=LL_COUNT; break;
            case SEC_STK: ops=stkOps; opCnt=STK_COUNT; break;
            case SEC_BST: ops=bstOps; opCnt=BST_COUNT; break;
            case SEC_REC: ops=recOps; opCnt=REC_COUNT; break;
        }
        for(int i=0;i<opCnt&&ly+BTN_H<H-BOT_H;i++){
            if(guiBtn(5,ly,LEFT_W-10,BTN_H,ops[i],selOp==i)){
                selOp=i; executeOp();
            }
            ly+=BTN_H+BTN_PAD;
        }

        /* ── output area ── */
        int ox=LEFT_W+5, oy=TOP_H+5, ow=W-LEFT_W-10, oh=H-TOP_H-BOT_H-10;
        DrawRectangle(ox,oy,ow,oh,C_OUTPUT);
        DrawRectangleLinesEx((Rectangle){(float)ox,(float)oy,(float)ow,(float)oh},1.0f,C_ACCENT);
        BeginScissorMode(ox+4,oy+4,ow-8,oh-8);
        {
            /* split outBuf into lines and draw */
            int ty=oy+6-outScroll, lineH=16;
            char *p=outBuf, *nl;
            while(*p){
                nl=strchr(p,'\n');
                int len=nl?(int)(nl-p):(int)strlen(p);
                if(len>0&&ty+lineH>oy&&ty<oy+oh){
                    char tmp[512]; if(len>511)len=511;
                    memcpy(tmp,p,len); tmp[len]=0;
                    DrawTextEx(appFont,tmp,(Vector2){(float)(ox+8),(float)ty},15,1,C_WHITE);
                }
                ty+=lineH;
                p=nl?nl+1:p+len;
            }
            int maxScroll=ty-(oy+6-outScroll)-oh;
            if(outScroll>maxScroll&&maxScroll>0) outScroll=maxScroll;
        }
        EndScissorMode();

        /* ── bottom input bar ── */
        int by=H-BOT_H;
        DrawRectangle(0,by,W,BOT_H,C_PANEL);
        int fx=10;
        guiField(fx,   by+30,200,"Name",  inName,100,0); fx+=210;
        guiField(fx,   by+30,280,"Definition",inDef,256,1); fx+=290;
        guiField(fx,   by+30,120,"DoB",   inDoB,20,2); fx+=130;
        guiField(fx,   by+30,120,"DoD",   inDoD,20,3); fx+=130;
        if(guiBtn(fx,by+28,120,28,"Execute",false)){ executeOp(); }

        EndDrawing();
    }
    UnloadFont(appFont);
    CloseWindow();
    return 0;
}
