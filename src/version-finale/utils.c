//
// Created by naysson on 22/11/2019.
//

#include "utils.h"

void deroute(int num_signal)
{
    switch(num_signal)
    {
        case SIGINT :
        case SIGTERM :
            CHECK(msgctl(identifiant_file,IPC_RMID,NULL),"Serveur - impossible de supprimer la file.",-1);
            exit(EXIT_SUCCESS);

        default:
            fprintf(stderr,"Serveur - signal %d non pris en compte.\n",num_signal);
    }
}

void transform_timestamp_to_string(time_t timestamp, char * buffer,size_t buffer_length)
{
    struct tm *tm = localtime(&timestamp);
    strftime(buffer,buffer_length,"%F %T",tm);
}
