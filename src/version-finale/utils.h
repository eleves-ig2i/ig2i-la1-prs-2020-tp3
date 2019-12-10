//
// Created by naysson on 22/11/2019.
//

#ifndef TP3_UTILS_H
#define TP3_UTILS_H

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#include "ipc_utils.h"

int identifiant_file;

/**
 * \def CHECK
 * \brief Macro vérifiant le bon fonctionnement de l'appel système, en obtenant sa valeur de retour en 1ere argument.
 */
#define CHECK(sts,msg,code_err) if ((sts) == (code_err)) {perror(msg);exit(-1);}

/**
 * Fonction gérant la réception des signaux.
 * si on reçoit un SIGTERM ou un SIGINT, notre programme doit supprimer la file de message et se terminer.
 * @param num_signal
 */
void deroute(int num_signal);

/**
 * Fonction utilisée pour transformer un timestamp (nombre de secondes depuis le 01 01 1970), en un format de date lisible.
 * @param tm
 * @param buffer
 * @param buffer_length
 */
void transform_timestamp_to_string(time_t timestamp, char * buffer,size_t buffer_length);

#endif //TP3_UTILS_H
