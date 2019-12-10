/**
 * \name bal5_s.c
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
#include <assert.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

/**
 * \def CHECK
 * \brief Macro vérifiant le bon fonctionnement de l'appel système, en obtenant sa valeur de retour en 1ere argument.
 */
#define CHECK(sts,msg,code_err) if ((sts) == (code_err)) {perror(msg);exit(-1);}

#define BUFFER_TIME_LENGTH 20

#define BUFFER_MESSAGE_LENGTH 100

#define NB_MAXIMUM_CLIENTS 30

#define PATH_KEY_IPC "/var/www"

struct message_buffer
{
    long mtype;
    char mtext[BUFFER_MESSAGE_LENGTH];
    long pid;
};

struct clients
{
    pid_t tab_pid[NB_MAXIMUM_CLIENTS];
    size_t nb_clients;
};

int identifiant_file = 0;

/**
 * Affiche quelques informations sur l'IPC.
 * @param stats_ipc
 */
void afficher_stats_ipc(const struct msqid_ds *stats_ipc);

/**
 * Génère la clé de l'ipc à l'aide des arguments en paramètre.
 * @param path
 * @param salt
 * @return
 */
int generate_key_ipc(char * path,int salt);

/**
 * Fonction utilisée pour transformer un timestamp (nombre de secondes depuis le 01 01 1970), en un format de date lisible.
 * @param tm
 * @param buffer
 * @param buffer_length
 */
void transform_timestamp_to_string(time_t timestamp, char * buffer,size_t buffer_length);

/**
 * Retourne true si pid_in est contenu dans tab_pid, false sinon.
 * @param tab_clients
 * @param pid_in
 * @return true si pid_in est contenu dans tab_pid, false sinon.
 */
bool contains_pid(const struct clients * tab_clients, pid_t pid_in);

/**
 * Insère le client pid_in dans le tableau de clients.
 * @param tab_clients
 * @param pid_in
 * @return true si l'insertion a eu lieu, false sinon.
 */
bool insert_pid(struct clients * tab_clients, pid_t pid_in);

/**
 * Supprime le client pid_in du tableau de clients.
 * @param tab_clients
 * @param pid_in
 * @return true si la suppression a eu lieu, false sinon.
 */
bool remove_pid(struct clients * tab_clients, pid_t pid_in);


/**
 * Fonction gérant la réception des signaux.
 * si on reçoit un SIGTERM ou un SIGINT, notre programme doit supprimer la file de message et se terminer.
 * @param num_signal
 */
void deroute(int num_signal);

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

    //  Etape 1 : Création de la file des messages
    CHECK(identifiant_file = msgget(identifiant_file, IPC_CREAT | IPC_EXCL | 0600), "Erreur système - impossible de créer la file de messages.\n", -1);

    // Etape 2 : récupération des infos de la file
    // int msgctl(int ​ msqid , ​ int ​ cmd ​ , struct msqid_ds *​ buf ​ );

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

int generate_key_ipc(char * path,int salt)
{
    assert(path != NULL);
    return ftok(path,salt);
}

void transform_timestamp_to_string(time_t timestamp, char * buffer,size_t buffer_length)
{
    struct tm *tm = localtime(&timestamp);
    strftime(buffer,buffer_length,"%F %T",tm);
}

void afficher_stats_ipc(const struct msqid_ds * stats_ipc)
{
    assert(stats_ipc != NULL);
    char buff[BUFFER_TIME_LENGTH];
    int identifiant_file_ipc = stats_ipc->msg_perm.__key;

    transform_timestamp_to_string(stats_ipc->msg_stime,buff,BUFFER_TIME_LENGTH);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - heure du dernier message envoyé : %s\n",identifiant_file_ipc,buff);

    transform_timestamp_to_string(stats_ipc->msg_ctime,buff,BUFFER_TIME_LENGTH);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - heure de la dernière modification : %s\n",identifiant_file_ipc,buff);

    fprintf(stdout,"Serveur - stats_ipc (id = %d) - nombre de messages dans la file : %lu\n",identifiant_file_ipc,stats_ipc->msg_qnum);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - Nombre maximum d'octets dans la file : %lu\n",identifiant_file_ipc,stats_ipc->msg_qbytes);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - Nombre actuel d'octets dans la file : %lu\n",identifiant_file_ipc,stats_ipc->__msg_cbytes);

}

bool contains_pid(const struct clients * tab_clients, pid_t pid_in)
{
    for(size_t index = 0; index < tab_clients->nb_clients ; index++)
    {
        if( pid_in == tab_clients->tab_pid[index])
            return true;
    }
    return false;
}

bool insert_pid(struct clients * tab_clients, pid_t pid_in)
{
    assert(tab_clients != NULL);

    if( tab_clients->nb_clients >= NB_MAXIMUM_CLIENTS )
        return false;

    if( contains_pid(tab_clients,pid_in) == false) {
        tab_clients->tab_pid[tab_clients->nb_clients++] = pid_in;
        return true;
    }

    return false;
}

/**
 * Renvoie l'index associé au pid dans le tableau de clients. Si le pid ne s'y trouve pas, renvoie NB_MAXIMUM_CLIENTS
 * @param tab_clients
 * @param pid_in
 * @return
 */
size_t get_index_from_pid(const struct clients * tab_clients, pid_t pid_in)
{
    for(size_t index =0; index < tab_clients->nb_clients; index++)
    {
        if( tab_clients->tab_pid[index] == pid_in)
            return index;
    }

    return NB_MAXIMUM_CLIENTS;
}


bool remove_pid(struct clients * tab_clients, pid_t pid_in)
{
    assert(tab_clients != NULL);
    size_t index_pid_a_supprimer;

    if( ( index_pid_a_supprimer = get_index_from_pid(tab_clients, pid_in)) != NB_MAXIMUM_CLIENTS )
    {
        // on décale les index au dessus
        for(size_t index = index_pid_a_supprimer; index < tab_clients->nb_clients-1;index++ )
        {
            tab_clients[index] = tab_clients[index+1];
        }
        return true;
    } else
    {
        return false;
    }
}

void deroute(int num_signal)
{
    switch(num_signal)
    {
        case SIGINT :
        case SIGTERM :
            CHECK(msgctl(identifiant_file,IPC_RMID,NULL),"Serveur - mpossible de supprimer la file.",-1);
            exit(EXIT_SUCCESS);

        default:
            fprintf(stderr,"Serveur - signal %d non pris en compte.\n",num_signal);
    }
}