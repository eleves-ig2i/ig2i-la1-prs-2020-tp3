//
// Created by naysson on 22/11/2019.
//

#ifndef TP3_CLIENTS_H
#define TP3_CLIENTS_H

#include <unistd.h>
#include <stdbool.h>

#define NB_MAXIMUM_CLIENTS 30

struct clients
{
    pid_t tab_pid[NB_MAXIMUM_CLIENTS];
    size_t nb_clients;
};

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


#endif //TP3_CLIENTS_H
