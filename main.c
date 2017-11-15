#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct node NODE;

struct node{
    char text[100];

    int label;

    NODE *left;

    NODE *right;
};

int read_file(FILE **file){
    *file = fopen("database.dot","r");

    if(*file==NULL){
        return 0;
    }

    return 1;
}

int open_file(FILE **file){
    *file = fopen("database.dot","w+");

    if(*file==NULL){
        printf("PROBLEMA AO CRIAR ARQUIVO!TENTE NOVAMENTE!");
        exit(1);
    }

    return 1;
}

int exisist_database(FILE *file){
    char str[10];

    fscanf(file,"%[^\n]",str);

    if(strcmp(str,"digraph database")==0){
        return 1;
    }

    return 0;
}

void create_database(FILE *file){
    fprintf(file,"digraph database\n{\n");
}

void position_file(FILE *file){
    rewind(file);

    fseek(file,-2*sizeof(char),SEEK_END);

    fprintf(file,"%s","b\n}");
}

NODE* create_object(char value[100],int *label){
    NODE *object;

    object = malloc(sizeof(NODE));

    object->right = NULL;

    object->left = NULL;

    object->label = *label;

    strcpy(object->text,value);

    ++*label;

    return object;
}

void write_database(FILE *file,NODE *root){
    NODE *aux;
    aux = root;
    if(aux!=NULL){
        fprintf(file,"%d%s%s%s",aux->label,"[label=\"",aux->text,"\"];\n");

        if(aux->left!=NULL){
            fprintf(file,"%d%s%d%s",aux->label,"->",aux->left->label,"[label=\"sim\"];\n");
        }

        if(aux->right!=NULL){
            fprintf(file,"%d%s%d%s",aux->label,"->",aux->right->label,"[label=\"nao\"];\n");
        }

        write_database(file,aux->left);

        write_database(file,aux->right);
    }
}

void test(NODE **root,int *label){
    NODE *aux;

    aux = create_object("TEM PELAGEM?",label);

    aux->right = create_object("EH BIPEDE?",label);

    aux->right->left = create_object("HOMEM",label);

    aux->right->right = create_object("PORCO",label);

    aux->left = create_object("RONRONA?",label);

    aux->left->left = create_object("GATO",label);

    aux->left->right = create_object("CAO",label);

    *root = aux;
}

int trata_string(char str[]){
    char aux[100];

    int i,j=0;

    while(str[j]>='0' && str[j]<='9'){
        j++;
    }

    if (str[j]=='['){
        i=j+8;
        while(str[i]!='"'){
            aux[i-(j+8)] = str[i];
            i++;
        }
        aux[i-(j+8)]='\0';

        strcpy(str,aux);

        return 1;
    }

    return 0;
}

NODE* read_database(FILE *file,int *label){
    char str[150];
    char a;

    NODE *aux = NULL;

    fscanf(file,"%[^\n]",str);

    getc(file);

    if(trata_string(str)){
        aux = create_object(str,label);
        int j=1;
        a = getc(file);
        while(a>='0' && a<='9'){
            a = getc(file);
            j++;
        }
        fseek(file,(-1*j)*sizeof(char),SEEK_CUR);
        if(a=='[' || a=='}'){
            return aux;
        }

        fscanf(file,"%[^\n]",str);

        getc(file);

        fscanf(file,"%[^\n]",str);

        getc(file);

        aux->left = read_database(file,label);

        aux->right = read_database(file,label);
    }

    return aux;
}

void menu(FILE *file){
    printf("Seja Bem Vindo ao Mini Akinator!\n\n");

    printf("1) Comecar\n2) Apagar banco de dados\n3) Sair\n");

    int resposta;

    while(1){
        scanf("%d",&resposta);

        switch(resposta){
        case 1:
            return;
        case 2:
            remove("database.dot");
            exit(0);
        case 3:
            exit(0);
        default:
            printf("Resposta Invalida! Tente Novamente!\n");
        }
    }
}

NODE* percorre(NODE *anterior,int *label){
    if(islower(anterior->text[0])){
        printf("O objeto pensado eh: %s?\n",anterior->text);
        printf("Resposta(S/N): ");
        char resposta;
        scanf(" %c",&resposta);
        resposta = tolower(resposta);
        while(resposta!='s' && resposta!='n'){
            printf("RESPOSTA INAVALIDA TENTE NOVAMENTE:\n");
            printf("Resposta(S/N): ");
            scanf(" %c",&resposta);
            resposta = tolower(resposta);
        }

        if(resposta=='s'){
            printf("VENCI VOCE HAHA! EBAA\n");

            return NULL;
        }

        else{
            printf("Entao qual eh o objeto?\n");

            char objeto[100];

            scanf(" %[^\n]",objeto);

            objeto[0] = tolower(objeto[0]);

            printf("Digite uma pergunta em que \'sim\' significa %s e \'nao\' significa %s: ",objeto,anterior->text);

            char pergunta[100];

            scanf(" %[^\n]",pergunta);

            pergunta[0] = toupper(pergunta[0]);

            int tam = strlen(pergunta);

            if(pergunta[tam-1]!='?'){
                pergunta[tam] = '?';
                pergunta[tam+1] = '\0';
            }

            NODE *new_node = create_object(pergunta,label);

            NODE *new_node2 = create_object(objeto,label);

            new_node->right = anterior;

            new_node->left = new_node2;

            printf("Perdi!!! :(\n");

            return new_node;
        }
    }

    else{
        printf("%s\n",anterior->text);
        printf("Resposta(S/N): ");
        char resposta;
        scanf(" %c",&resposta);
        resposta = tolower(resposta);
        while(resposta!='s' && resposta!='n'){
            printf("RESPOSTA INAVALIDA TENTE NOVAMENTE:\n");
            printf("Resposta(S/N): ");
            scanf(" %c",&resposta);
            resposta = tolower(resposta);
        }
        if(resposta=='s'){
            NODE* test = percorre(anterior->left,label);
            if(test!=NULL){
                anterior->left = test;
            }

            return NULL;
        }

        else{
            NODE* test = percorre(anterior->right,label);
            if(test!=NULL){
                anterior->right = test;
            }

            return NULL;
        }
    }
}

int main(){
    FILE *file;

    NODE *root = NULL;

    int label = 0;

    menu(file);

    if(read_file(&file)){
        if(!exisist_database(file)){
            fclose(file);

            open_file(&file);

            create_database(file);
        }

        else{

            //PULA O '{'
            fgetc(file);

            fgetc(file);

            fgetc(file);

            //END
            root = read_database(file,&label);

            fclose(file);

            open_file(&file);

            create_database(file);
        }
    }

    else{
      open_file(&file);

      create_database(file);
    }

    if(root==NULL){
        char objeto[100];

        printf("NADA FOI ENCONTRADO NO BANCO DE DADOS\n");

        printf("INFORME UM OBJETO: ");

        scanf(" %[^\n]",objeto);

        objeto[0] = tolower(objeto[0]);

        root = create_object(objeto,&label);
    }

    else if(islower(root->text[0])){
        printf("O objeto pensado eh: %s?\n",root->text);
        printf("Resposta(S/N): ");
        char resposta;
        scanf(" %c",&resposta);
        resposta = tolower(resposta);
        while(resposta!='s' && resposta!='n'){
            printf("RESPOSTA INAVALIDA TENTE NOVAMENTE:\n");
            printf("Resposta(S/N): ");
            scanf(" %c",&resposta);
            resposta = tolower(resposta);
        }
        if(resposta=='s'){
            printf("VENCI VOCE HAHA! EBAA\n");
        }

        else{
            printf("Entao qual eh o objeto?\n");

            char objeto[100];

            scanf(" %[^\n]",objeto);

            objeto[0] = tolower(objeto[0]);

            printf("Digite uma pergunta em que \'sim\' significa %s e \'nao\' significa %s: ",objeto,root->text);

            char pergunta[100];

            scanf(" %[^\n]",pergunta);

            pergunta[0] = toupper(pergunta[0]);

            int tam = strlen(pergunta);

            if(pergunta[tam-1]!='?'){
                pergunta[tam] = '?';
                pergunta[tam+1] = '\0';
            }

            NODE *new_node = create_object(pergunta,&label);

            NODE *new_node2 = create_object(objeto,&label);

            new_node->right = root;

            new_node->left = new_node2;

            root = new_node;

            printf("Perdi!!! :(\n");
        }
    }

    else{
        percorre(root,&label);
    }

    write_database(file,root);

    fprintf(file,"}");

    fclose(file);

    return 0;
}
