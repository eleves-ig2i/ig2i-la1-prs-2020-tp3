/**
 * \name bal_serveur_final.c
 * \author Nathan SALEZ
 * \date 20/11/2019
 * \brief crée une boîte aux lettres, qui affiche les informations
 * de cette boîte aux lettres après création,
 * puis qui réalise une boucle sans fin de lecture des messages de type 1
 * où il affiche le message reçu et ses informations
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "clients.h"
#include "types.h"
#include "utils.h"
#include "ipc_utils.h"

#define PATH_KEY_IPC "/var/www"

int main() {

    identifiant_file = generate_key_ipc(PATH_KEY_IPC, 0001);

    struct clients liste_clients;
    liste_clients.nb_clients=0;

    struct msqid_ds informations_file;

    struct message_buffer msg_received,msg_to_send;

    struct sigaction preparationSignal;

    // Association du callback au gestionnaire de signaux.
    preparationSignal.sa_handler=&deroute;

    // On ne souhaite pas bloquer de signal.
    sigemptyset(&preparationSignal.sa_mask);

    //Installation du gestionnaire d'actions
    sigaction(SIGTERM,&preparationSignal,NULL);
    sigaction(SIGINT,&preparationSignal,NULL);

    //  Etape 1 : Création de la file des messages
    CHECK(identifiant_file = msgget(identifiant_file, IPC_CREAT | IPC_EXCL | 0600), "Erreur système - impossible de créer la file de messages.\n", -1);


    // on utilise la commande IPC_STAT sur l'IPC d'identifiant donné, on récupère les infos dans informations_file
    CHECK(msgctl(identifiant_file, IPC_STAT, &informations_file), "Erreur système - impossible de récupérer les informations de la file de messages.\n", -1);

    // Etape 3 : affichage des infos de la file
    afficher_stats_ipc(&informations_file);

    // Etape 4 : lecture des messages
    while(1)
    {
        // ssize_t msgrcv(int ​ msqid ​ , void *​ msgp , ​ size_t ​ msgsz , ​ long ​ msgtyp ​ ,int ​ msgflg ​ );
        ssize_t nombreOctetsCopies = 0;
        CHECK(nombreOctetsCopies = msgrcv(identifiant_file, (void *)&msg_received, sizeof(struct message_buffer), 1, MSG_NOERROR), "Impossible de lire un message", -1);
        fprintf(stdout, "Serveur - message reçu : '%s' du processus %ld - nb octets reçu = %lu\n", msg_received.mtext, msg_received.pid, nombreOctetsCopies);

        // Doit-on supprimer le client du message ?
        if( strcmp(msg_received.mtext,"SALUT") == 0)
        {
            if( remove_pid(&liste_clients,msg_received.pid) == true )
                fprintf(stdout,"Serveur - client %ld supprimé de la liste des clients.\n",msg_received.pid);
            else
                fprintf(stdout,"Serveur - tentative de suppression du client %ld avortée.\n",msg_received.pid);
        } else {
            // On tente d'insérer le pid du nouveau client.
            if (insert_pid(&liste_clients, msg_received.pid) == true)
                fprintf(stdout, "Serveur - ajout du client %ld dans la liste des clients connus.\n", msg_received.pid);
            else
                fprintf(stdout, "Serveur - client %ld déjà inséré.\n", msg_received.pid);
        }

        // Etape 5 : renvoi d'un message au client.
        strcpy(msg_to_send.mtext,"Serveur => Client : Message reçu !!");
        msg_to_send.mtype = msg_received.pid;
        msg_to_send.pid = getpid();
        CHECK(msgsnd(identifiant_file, (void *)&msg_to_send, sizeof(struct message_buffer), 0), "Serveur - Impossible d'envoyer un message à la file.\n", -1);
        fprintf(stdout,"Serveur - Envoi du message de mtype %ld au client %ld.\n",msg_to_send.mtype,msg_received.pid);
    }
}
