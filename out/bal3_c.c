/**
 * \name bal3_c.c
 * \author Nathan SALEZ
 * \date 20/11/2019
 * \brief ouvre la boîte aux lettres créée par bal3_s, qui écrit 10 fois
 * des messages saisis par l’utilisateur. Ces messages sont de type 1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <assert.h>
#include <time.h>
#include <string.h>

/**
 * \def CHECK
 * \brief Macro vérifiant le bon fonctionnement de l'appel système, en obtenant sa valeur de retour en 1ere argument.
 */
#define CHECK(sts,msg,code_err) if ((sts) == (code_err)) {perror(msg);exit(-1);}

#define BUFFER_MESSAGE_LENGTH 50

#define ID_IPC 17104952

#define MESSAGE_QUEUE_ID 1179661 // TODO : comment se connecter automatiquement à la bonne file ?

struct message_buffer
{
    long mtype;
    long pid;
    char mtext[BUFFER_MESSAGE_LENGTH];
};

int main() {
    struct msqid_ds informationsFile;

    struct message_buffer msg_buffer;
    strcpy(msg_buffer.mtext,"Hello there");
    msg_buffer.mtype = 1;
    msg_buffer.pid = getpid();

    //  Etape 1 : Ouverture de la file des messages
    // On évite les blocages avec IPC_NOWAIT
    CHECK(msgget(ID_IPC,IPC_NOWAIT),"Client - Erreur système - impossible d'ouvrir' la file de messages.\n",-1);

    fprintf(stdout,"Connexion à la file %d réussie.\n",ID_IPC);
    // Etape 2 : envoi d'un message à la file

    while(1) {
        fscanf(stdin,"%s\n",msg_buffer.mtext);
        for (size_t i = 0; i < 10; i++) {
            CHECK(msgsnd(MESSAGE_QUEUE_ID, (void *) &msg_buffer, sizeof(msg_buffer), IPC_NOWAIT),
                  "Client - Erreur système - Impossible d'insérer un message dans la file.\n", -1);
            fprintf(stdout, "Message envoyée dans la file %d.\n", ID_IPC);
        }
    }

    return 0;
}