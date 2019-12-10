//
// Created by naysson on 22/11/2019.
//

#ifndef TP3_IPC_UTILS_H
#define TP3_IPC_UTILS_H

#include "utils.h"

#define BUFFER_TIME_LENGTH 20

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

#endif //TP3_IPC_UTILS_H
