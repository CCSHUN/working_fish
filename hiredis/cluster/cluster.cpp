#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis.h>

#ifdef _MSC_VER
#include <winsock2.h> /* For struct timeval */
#endif

int main(int argc, char **argv) {
    Cluster<redisContext>::ptr_t cluster_p;
    // Declare pointer to simple hiredis reply structure
    redisReply * reply;
    // Create cluster passing acceptable address and port of one node of the cluster nodes 
    cluster_p = HiredisCommand<>::createCluster( "127.0.0.1", 7000 );
    // send command to redis passing created cluster pointer, key which you wish to access in the command
    // and command itself with parameters with printf like syntax
    reply = static_cast<redisReply*>( HiredisCommand<>::Command( cluster_p, "FOO", "SET %s %s", "FOO", "BAR1" ) );
    // Check reply state and type
    if( reply->type == REDIS_REPLY_STATUS  || reply->type == REDIS_REPLY_ERROR )
    {
       // Process reply
        cout << " Reply to SET FOO BAR " << endl;
        cout << reply->str << endl;
    }
    // free hiredis reply structure
    freeReplyObject( reply );
    // delete cluster by its pointer
    delete cluster_p;

    return 0;
}
