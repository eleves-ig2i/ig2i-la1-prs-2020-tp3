/**
 * \name bal_client_final.c
 * \author Nathan SALEZ
 * \date 20/11/2019
 * \brief ouvre la boîte aux lettres créée par bal3_s, ÉCRIT 1 message saisi par l’utilisateur. Ces messages sont de type 1.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#include "types.h"
#include "utils.h"

#define ID_IPC 17104952

int main() {
    long message_queue_id =0;

    struct message_buffer msg_sent, msg_received;
    strcpy(msg_sent.mtext, "Hello there");
    msg_sent.mtype = 1;
    msg_sent.pid = getpid();

    //  Etape 1 : Ouverture de la file des messages
    // On évite les blocages avec IPC_NOWAIT
    CHECK(message_queue_id = msgget(ID_IPC,IPC_NOWAIT),"Client - Erreur système - impossible d'ouvrir' la file de messages.\n",-1);

    fprintf(stdout,"Connexion à la file %d réussie.\n",ID_IPC);

    // Etape 2 : envoi d'un message à la file
    while(strcmp(msg_sent.mtext, "SALUT") != 0)
    {
        fprintf(stdout,"Veuillez saisir un mot :");
        fscanf(stdin, "%s", msg_sent.mtext);

        CHECK(msgsnd(message_queue_id, (void *) &msg_sent, sizeof(struct message_buffer), IPC_NOWAIT),
                "Client - Erreur système - Impossible d'insérer un message dans la file.\n", -1);
        fprintf(stdout, "Message envoyée dans la file %d.\n", ID_IPC);

        CHECK(msgrcv(message_queue_id, (void *) &msg_received, sizeof(struct message_buffer), getpid(),MSG_NOERROR),
                "Client - Erreur système - Impossible de recevoir un message dans la file.\n", -1);
        fprintf(stdout, "Client - message reçu : '%s'\n", msg_received.mtext);

    }

    fprintf(stdout,"\ncya !\n");
    return 0;
}
