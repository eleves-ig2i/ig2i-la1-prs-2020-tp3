//
// Created by naysson on 22/11/2019.
//

#include <assert.h>
#include "clients.h"


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
