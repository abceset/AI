#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>


#define SETUP_FILE "setup.txt"
#define MAX_BUFFER 255 /*fgets reads the first define+1 characters or until it finds the first "\n" character.
                                    So, I set this to the highest number to read an entiry line*/

#define MAX_EVALUATION 7

typedef struct node
{

    struct node *previous;
    int indexInFile;
    float distance;
    int evaluation;
    struct node *next;

} node;

typedef struct descriptor
{

    node *first;
    int quant;
    node *last;

}descriptor;

typedef struct point /*It's a list of coordinates to construct the point, because I don't now the dimension of point. strtok solve the problem, but it doesn't
                     work on Windows (It's a POSIX function)*/
{

    float coordinate;
    struct point *next;

}point;

typedef struct point_descriptor
{
    point *first;
    int quant;
    point *last;

}point_descriptor;


node *next;
descriptor *head;
descriptor *descriptor_node;
int erro;
int total_lines_training,total_lines_test;


char *substring(char *str, int start, int quant){
  char *ret = str;
  int i = 0;

  // Initial position less than 0 or
  // greater than length?
  if(( start < 0) || (start > strlen(str)))
    start = 0;

  //
  if(quant > start + strlen(str))
    quant = strlen(str) - start;

  // Get the chars
  for(i = 0; i <= quant - 1; i++){
    ret[i] = str[start + i];
  }

  // End of string
  ret[i] = '\0';

  return ret;
}


void createInitialConditions()
{

    descriptor_node = (descriptor *)malloc(sizeof(descriptor));

    if (descriptor_node == NULL) {
            printf ("EXITING THE PROGRAM NOW! MALLOC RETURNED NULL: FATAL ERROR - NO MEMORY AVAILABLE!\n");
            exit (EXIT_FAILURE);
    }

    descriptor_node->first=NULL;
    descriptor_node->quant=0;
    descriptor_node->last=NULL;

    head = (descriptor *)malloc(sizeof(descriptor));

    if (head == NULL) {
            printf ("EXITING THE PROGRAM NOW! MALLOC RETURNED NULL: FATAL ERROR - NO MEMORY AVAILABLE!\n");
            exit (EXIT_FAILURE);
    }

    head = descriptor_node;


}

void createNewNode(int indexInFile,float distance, int evaluation,int k)
{

    node *p;
    p = (node *)malloc(sizeof(node));
    if (p == NULL) {
      printf ("EXITING THE PROGRAM NOW! MALLOC RETURNED NULL: FATAL ERROR - NO MEMORY AVAILABLE!\n");
      exit (EXIT_FAILURE);
    }

    /*printf("\nDistância Euclidiana: %f\n",distance);
    printf("\nDiagnóstico: %d\n",evaluation);
    printf("\nIndex: %d\n",indexInFile);
    printf("\nK: %d\n",k);
    printf("\nTAMANHO DA LISTA: %d\n",head->quant);
    system("pause");*/


    p->previous=NULL;
    p->distance=distance;
    p->evaluation=evaluation;
    p->indexInFile=indexInFile;
    p->next = NULL;

    if (head->quant == 0){

        /*First node of the list*/

        head->first = p;
        head->last = p;
        head->quant = 1;

    }else {

        if ((distance <= head->last->distance) && (head->quant == k)) {

            if (distance < head->first->distance){

                /*Found the new nearest point*/

                p->next=head->first;

                p->previous=NULL;

                head->first->previous = p;

                head->first = p;


            } else {

                /*It's on the middle of the list*/

                node *q;

                q = head->first->next;

                while(q != NULL){

                    if (distance <= (q->distance)){

                        //printf("É menor");

                        p->next=q;
                        p->previous=q->previous;
                        q->previous->next=p;
                        q->previous=p;

                        break;

                    }

                    q=q->next;
                }

                q = NULL;
                free(q);

            }

            node *t;

            t = head->last;

            head->last = t->previous;

            head->last->next = NULL;

            free(t);




        }else if((distance > head->last->distance) && (head->quant < k)){

            p->previous = head->last;
            p->next = NULL;
            head->last->next = p;
            head->last = p;
            head->quant += 1;


        } else if (head->quant < k) {

            if (distance < head->first->distance){

                /*Found the new nearest point*/

                p->next=head->first;

                p->previous=NULL;

                head->first->previous = p;

                head->first = p;

                head->quant += 1;


            } else {

                /*It's on the middle of the list*/

                node *q;

                q = head->first->next;

                while(q != NULL){

                    if (distance <= (q->distance)){

                        //printf("É menor");

                        p->next=q;
                        p->previous=q->previous;
                        q->previous->next=p;
                        q->previous=p;

                        head->quant+=1;

                        break;

                    }

                    q=q->next;
                }

                q = NULL;
                free(q);

            }

        }

    }




}

int createList()
{
    /*This function gets an specific exam in test file, generates the euclidean distance and
      creates a list with the distance of this point to each other point in the training file.

    */

    FILE *setupFile;
    FILE *trainingFile;
    FILE *testFile;
    FILE *auxFile;

    char information[4][255];

    char line[MAX_BUFFER];
    char line_backup[MAX_BUFFER];

    setupFile = fopen(SETUP_FILE, "r");
    if (setupFile == 0){
        return 1;
    }

    /*fgets(line, LENGTH_FILES_IN_SETUP, setupFile);

    Strip trailing '\n' if it exists

    int len = strlen(line)-1;
    if(line[len] == '\n')
        line[len] = 0;
    printf("\n%s é o conteúdo da primeira linha\n", line);*/


    /*for (ch = fgetc(setupFile); ch != EOF; ch = fgetc(setupFile)){
        fscanf(setupFile, "%[^\n]", line);
        printf("%s\n", line);

    fscanf is not a good idea. It can leave your file pointer in an unknown location on failure. Even that,
    if you use it, try to discover why cuts off the first letter of the first line.
    }*/

    int i = 0;

    while (fgets (line, MAX_BUFFER, setupFile)) {

        strcpy(information[i],line);

        i++;

    }

    fclose(setupFile);

    //Remove \n
    char* p;

    for (i=0;i<=4;i++){
        p = strchr(information[i],'\n');
        if (p)
            *p = '\0';
    }

    trainingFile = fopen(information[0], "r");
    testFile = fopen(information[1], "r");

    if ((trainingFile == NULL) || (testFile == NULL)){
        perror("Error: Can't Open File \"trainingFile\"");
        perror("Error: Can't Open File \"testFile\"");
        exit(1);
    }

    point *point_test;
    point *point_training;
    point_descriptor *descriptor_of_point_test;
    point_descriptor *descriptor_of_point_training;
    point *ptr_aux;

    descriptor_of_point_test = (point_descriptor *)malloc(sizeof(point_descriptor));

    if (descriptor_of_point_test == NULL) {
            printf ("EXITING THE PROGRAM NOW! MALLOC RETURNED NULL: FATAL ERROR - NO MEMORY AVAILABLE!\n");
            exit (EXIT_FAILURE);
    }

    descriptor_of_point_training = (point_descriptor *)malloc(sizeof(point_descriptor));

    if (descriptor_of_point_training == NULL) {
            printf ("EXITING THE PROGRAM NOW! MALLOC RETURNED NULL: FATAL ERROR - NO MEMORY AVAILABLE!\n");
            exit (EXIT_FAILURE);
    }

    char *ptr;

    int j=0;

    auxFile = fopen("tmp.txt", "w+a");
    if(auxFile==NULL) {
        perror("Error: Can't Create File \"Tmp\"");
        exit(1);
    }

    while (fgets (line, MAX_BUFFER, testFile)){

        strcpy(line_backup,line);

        descriptor_of_point_test->first=NULL;
        descriptor_of_point_test->quant=0;
        descriptor_of_point_test->last=NULL;

        ptr = strtok(line, " ");

        i=0;
        while(ptr != NULL){
            ptr_aux = (point *)malloc(sizeof(point));

            if (ptr_aux == NULL) {
                printf ("EXITING THE PROGRAM NOW! MALLOC RETURNED NULL: FATAL ERROR - NO MEMORY AVAILABLE!\n");
                exit (EXIT_FAILURE);
            }
            ptr_aux->coordinate = floorf((float)atof(ptr)*100)/100;
            ptr_aux->next = NULL;
            if (descriptor_of_point_test->quant == 0){
                descriptor_of_point_test->first=ptr_aux;
                descriptor_of_point_test->last=ptr_aux;
            } else {
                descriptor_of_point_test->last->next=ptr_aux;
                descriptor_of_point_test->last=ptr_aux;
            }
            descriptor_of_point_test->quant+=1;
            ptr = strtok(NULL, " ");
            i++;
         }

         /*point_test = descriptor_of_point_test->first;
         while (point_test != NULL){

                printf("O ponto obtido foi: %.2f\n",point_test->coordinate);
                point_test = point_test->next;
                system("pause");

         }*/

        i=0;

        while(fgets(line, MAX_BUFFER, trainingFile)){

            descriptor_of_point_training->first=NULL;
            descriptor_of_point_training->quant=0;
            descriptor_of_point_training->last=NULL;

            ptr = strtok(line, " ");

             while(ptr != NULL){
                ptr_aux = (point *)malloc(sizeof(point));

                if (ptr_aux == NULL) {
                    printf ("EXITING THE PROGRAM NOW! MALLOC RETURNED NULL: FATAL ERROR - NO MEMORY AVAILABLE!\n");
                    exit (EXIT_FAILURE);
                }
                ptr_aux->coordinate = floorf((float)atof(ptr)*100)/100;
                ptr_aux->next = NULL;
                if (descriptor_of_point_training->quant == 0){
                    descriptor_of_point_training->first=ptr_aux;
                    descriptor_of_point_training->last=ptr_aux;
                } else {
                    descriptor_of_point_training->last->next=ptr_aux;
                    descriptor_of_point_training->last=ptr_aux;
                }
                descriptor_of_point_training->quant+=1;
                ptr = strtok(NULL, " ");
             }


            /*Calculate the Euclidean Distance*/

            float sum=0;

            point_test = descriptor_of_point_test->first;
            point_training = descriptor_of_point_training->first;

            while (point_test->next != NULL){

                sum+=powf(point_test->coordinate-point_training->coordinate,2);
                point_test = point_test->next;
                point_training = point_training->next;

            }

            createNewNode(i,sqrt(sum),descriptor_of_point_training->last->coordinate,atoi(information[2]));

            i++;

        }
        node *p;

        p = head->first;

        /*printf("\n\n\n");
        printf("------------------------\n");
        printf("Primeiro: %.2f\n",head->first->distance);
        printf("Ultimo: %.2f\n",head->last->distance);
        printf("------------------------\n\n");*/



        int evaluation[MAX_EVALUATION];

        int o;
        for (o=0;o<=7;o++){

         evaluation[o]=0;
        }

        while(p != NULL){

            printf("Distance: %f ---> Evaluation: %d\n",p->distance,p->evaluation);

            evaluation[p->evaluation]+=1;

            p = p->next;

        }

        int greater=0;

        for (o=1;o<=6;o++){

            if(evaluation[o] > evaluation[o-1]){

                greater = o;

            }

        }
        j++;

        /*Always Remember: The substring function receives a pointer to the string to be parsed. So, be careful: It changes

        the original string!
        */

        char to[MAX_BUFFER]; // It's to manipulate the original file line and insert the new point evaluation by KNN

        char line_original[MAX_BUFFER]; //It's to get the original evaluation of point

        strcpy(to, line_backup);

        strcpy(line_original,line_backup);

        //printf("lineTo: %s",to);

        int ch = ' ';

        /*printf("\n\nto: %s\n\n",to);
        system("pause");*/

        char *lastIndexofCh = strrchr(to,ch);

        int hasDecimalPoint;

        if (strrchr(lastIndexofCh,'.') != NULL){

                hasDecimalPoint = 1;

        }

        /*printf("%s\n",lastIndexofCh);

        system("pause");*/

        //printf("%s",strrchr(lastIndexofCh,'.'));


        int tam = strlen(to) - strlen(lastIndexofCh);

        char *original_evaluation = substring(line_original,tam+1,strlen(line_backup));

        char *res = substring(to, 0, tam+1);

        char buffer[0];

        strcat(res,itoa(greater,buffer,10));

        if (hasDecimalPoint == 1){

            strcat(res,".0");

        }

        strcat(res,"\n");

        /*Assert if the evaluation provided by KNN is the same that given by the original file*/

        res = res + tam; /*Put the pointer at the beginnig of KNN evaluation*/

        if (atof(original_evaluation) != atof(res)){

                erro+=1;

        }

        res = res - tam; /*Back the pointer to the original position*/

        /*This line creates a file named tmp with the evaluation given by KNN Method. This file is just
        for future comparision. If you want, you can remove this line or even the tmp file.
        */

        fprintf(auxFile,res);


        printf("\n\n\n");
        rewind(trainingFile);
        createInitialConditions();




    }
    fclose(auxFile);
    fclose(testFile);
    fclose(trainingFile);
    total_lines_training = i;
    total_lines_test = j;

    return 0;

}

void showStatisticResults(){

    printf("\n\nTotal de pontos no arquivo de treinamento: %d\n\n",total_lines_training);
    printf("Total de pontos no arquivo de teste: %d\n\n",total_lines_test);
    printf("Total de Erros: %d\n\n",erro);
    float a = (float)erro/(float)total_lines_test;
    float percent = (1-a)*100;
    printf("Percentual de Acerto (%%): %.2f\n\n\n\n",percent);


}

int main()
{
    erro = 0;
    createInitialConditions();
    createList();
    showStatisticResults();
    return 0;
}


