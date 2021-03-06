
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <open62541.h>
#include "RoboterarmObjekt.h"
#include <pthread.h>

UA_Boolean running = true;

static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

int main(void) {
    pthread_t threadClacTemperatur, threadCalcPressure, threadGetState;

    initVariables();

    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);
    srand(time(NULL));

    UA_ServerConfig config = UA_ServerConfig_standard;
    UA_ServerNetworkLayer nl =
    UA_ServerNetworkLayerTCP(UA_ConnectionConfig_standard, 16664);
    config.networkLayers = &nl;
    config.networkLayersSize = 1;
    UA_Server *server = UA_Server_new(config);

    //Bereich zum Adden der einzelnen Nodes
    defineObjectTypes(server);
    addRoboterarmObjectInstance(server, "Roborterarm");
    addRoboterarmTypeConstructor(server);
    
    if(pthread_create( &threadClacTemperatur, NULL, calcCurrentTemp, server) != 0 ) {
        printf("threadClacTemperatur konnte nicht erzeugt werden\n");
        return EXIT_FAILURE;
    }
    // Thread 2 erzeugen
   
    if(pthread_create( &threadCalcPressure, NULL, calcCurrentPressure, server) != 0 ) {
        printf("threadCalcPressure konnte nicht erzeugt werden\n");
        return EXIT_FAILURE;
    }

    if(pthread_create( &threadGetState, NULL, askForState, server) != 0 ) {
        printf("threadGetState konnte nicht erzeugt werden\n");
        return EXIT_FAILURE;
    }

    UA_Server_run(server, &running);
    UA_Server_delete(server);
    nl.deleteMembers(&nl);
    printf("SERVER OUT!\n");    
    return 0;
}