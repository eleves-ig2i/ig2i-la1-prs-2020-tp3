/**
 * \name bal3_s.c
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

/**
 * \def CHECK
 * \brief Macro vérifiant le bon fonctionnement de l'appel système, en obtenant sa valeur de retour en 1ere argument.
 */
#define CHECK(sts,msg,code_err) if ((sts) == (code_err)) {perror(msg);exit(-1);}

#define BUFFER_TIME_LENGTH 20

#define BUFFER_MESSAGE_LENGTH 50

#define PATH_KEY_IPC "/var/www"

struct message_buffer
{
    long mtype;
    char mtext[BUFFER_MESSAGE_LENGTH];
};

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
void transform_timestamp_to_string(time_t tm, char * buffer,size_t buffer_length);

int main() {

    int identifiantFile = generate_key_ipc(PATH_KEY_IPC,0001);

    struct msqid_ds informationsFile;

    struct message_buffer msg_buffer;

    //  Etape 1 : Création de la file des messages
    CHECK(identifiantFile = msgget(identifiantFile,IPC_CREAT | IPC_EXCL | 0600),"Erreur système - impossible de créer la file de messages.\n",-1);

    // Etape 2 : récupération des infos de la file
    // int msgctl(int ​ msqid , ​ int ​ cmd ​ , struct msqid_ds *​ buf ​ );

    // on utilise la commande IPC_STAT sur l'IPC d'identifiant donné, on récupère les infos dans informationsFile
    CHECK(msgctl(identifiantFile,IPC_STAT,&informationsFile),"Erreur système - impossible de récupérer les informations de la file de messages.\n",-1);

    // Etape 3 : affichage des infos de la file
    afficher_stats_ipc(&informationsFile);

    // Etape 4 : lecture des messages
    while(1)
    {
        // ssize_t msgrcv(int ​ msqid ​ , void *​ msgp , ​ size_t ​ msgsz , ​ long ​ msgtyp ​ ,int ​ msgflg ​ );
        ssize_t nombreOctetsCopies = 0;
        CHECK(nombreOctetsCopies = msgrcv(identifiantFile,(void *)&msg_buffer, sizeof(msg_buffer),1, MSG_NOERROR),"Impossible de lire un message",-1);
        fprintf(stdout,"Serveur - message reçu : '%s' - nb octets reçu = %lu\n",msg_buffer.mtext,nombreOctetsCopies);

    }


    return 0;
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
    int identifiant_file = stats_ipc->msg_perm.__key;

    transform_timestamp_to_string(stats_ipc->msg_stime,buff,BUFFER_TIME_LENGTH);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - heure du dernier message envoyé : %s\n",identifiant_file,buff);

    transform_timestamp_to_string(stats_ipc->msg_ctime,buff,BUFFER_TIME_LENGTH);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - heure de la dernière modification : %s\n",identifiant_file,buff);

    fprintf(stdout,"Serveur - stats_ipc (id = %d) - nombre de messages dans la file : %lu\n",identifiant_file,stats_ipc->msg_qnum);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - Nombre maximum d'octets dans la file : %lu\n",identifiant_file,stats_ipc->msg_qbytes);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - Nombre actuel d'octets dans la file : %lu\n",identifiant_file,stats_ipc->__msg_cbytes);

}