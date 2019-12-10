//
// Created by naysson on 22/11/2019.
//

#include <assert.h>
#include "ipc_utils.h"

void afficher_stats_ipc(const struct msqid_ds * stats_ipc)
{
    assert(stats_ipc != NULL);
    char buff[BUFFER_TIME_LENGTH+1];
    int identifiant_file_ipc = stats_ipc->msg_perm.__key;

    transform_timestamp_to_string(stats_ipc->msg_stime,buff,BUFFER_TIME_LENGTH);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - heure du dernier message envoyé : %s\n",identifiant_file_ipc,buff);

    transform_timestamp_to_string(stats_ipc->msg_ctime,buff,BUFFER_TIME_LENGTH);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - heure de la dernière modification : %s\n",identifiant_file_ipc,buff);

    fprintf(stdout,"Serveur - stats_ipc (id = %d) - nombre de messages dans la file : %lu\n",identifiant_file_ipc,stats_ipc->msg_qnum);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - Nombre maximum d'octets dans la file : %lu\n",identifiant_file_ipc,stats_ipc->msg_qbytes);
    fprintf(stdout,"Serveur - stats_ipc (id = %d) - Nombre actuel d'octets dans la file : %lu\n",identifiant_file_ipc,stats_ipc->__msg_cbytes);

}

int generate_key_ipc(char * path,int salt)
{
    assert(path != NULL);
    return ftok(path,salt);
}