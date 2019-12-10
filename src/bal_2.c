/**
 * \name bal_2.c
 * \author Nathan SALEZ
 * \date 20/11/2019
 * \brief crée une boîte aux lettres, écrit un message dans la boîte aux lettres, et
 * examine les informations concernant la boite aux lettres avant et après l’envoi d’un message.
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
    strcpy(msg_buffer.mtext,"Hello there");
    msg_buffer.mtype = 2;

    //  Etape 1 : Création de la file des messages
    CHECK(identifiantFile = msgget(identifiantFile,IPC_CREAT | IPC_EXCL | 0600),"Erreur système - impossible de créer la file de messages.\n",-1);

    // Etape 2 : récupération des infos de la file
    // int msgctl(int ​ msqid , ​ int ​ cmd ​ , struct msqid_ds *​ buf ​ );

    // on utilise la commande IPC_STAT sur l'IPC d'identifiant donné, on récupère les infos dans informationsFile
    CHECK(msgctl(identifiantFile,IPC_STAT,&informationsFile),"Erreur système - impossible de récupérer les informations de la file de messages.\n",-1);

    // Etape 3 : affichage des infos de la file
    afficher_stats_ipc(&informationsFile);

    // Etape 4 : envoi d'un message à la file

    // int msgsnd(int ​ msqid ​ , const void *​ msgp ​ , size_t ​ msgsz , ​ int ​ msgflg ​ );

    // msgp étant de type générique, on crée une structure de données message_buffer
    // en 3e paramètre, on met la taille de la mémoire allouée au champ mtext.
    // On ne souhaite pas attendre l'envoi du message, pour avoir un échec d'appel système.
    CHECK(msgsnd(identifiantFile,(void *)&msg_buffer,sizeof(msg_buffer.mtext), IPC_NOWAIT ),"Erreur système - Impossible d'insérer un message de taille 50.\n",-1);
    fprintf(stdout,"Message envoyée dans la file.\n");

    // Etape 5 : réaffichage des infos de la file
    CHECK(msgctl(identifiantFile,IPC_STAT,&informationsFile),"Erreur système - impossible de récupérer les informations de la file de messages.\n",-1);

    afficher_stats_ipc(&informationsFile);


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
    fprintf(stdout,"stats_ipc (id = %d) - heure du dernier message envoyé : %s\n",identifiant_file,buff);

    transform_timestamp_to_string(stats_ipc->msg_ctime,buff,BUFFER_TIME_LENGTH);
    fprintf(stdout,"stats_ipc (id = %d) - heure de la dernière modification : %s\n",identifiant_file,buff);

    fprintf(stdout,"stats_ipc (id = %d) - nombre de messages dans la file : %lu\n",identifiant_file,stats_ipc->msg_qnum);

}