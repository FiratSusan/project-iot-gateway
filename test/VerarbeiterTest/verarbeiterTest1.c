#include "verarbeiter.h"
#include <time.h>


int main(int argc, char* argv[]){
	
	srand(time(NULL));

	// Setup config
	IG_Config* config = IG_Config_create(argv[1], IG_CONFIG_STANDARD);


	// Setup versender
	IG_Datenversender * sender = IG_Datenversender_create(config);

	// Setup erfasser and fill its queue with Data
	IG_Datenerfasser * erfasser = IG_Datenerfasser_create_nonBlocking(config);

	for(int i = 0; i<100; ++i){

		IG_Data* data = (IG_Data*)malloc(sizeof(IG_Data));
		if(data==NULL) return EXIT_FAILURE;		

		data->id = (rand() % 2)?4:1;
		data->datatype = IG_INT32;

		int* record = (int*)malloc(sizeof(int));
		if(record==NULL) return EXIT_FAILURE;

		*(record) = i*i;		
		data->data = (void*)record;
		data->timestamp = 0;
		// Inserting into the queue
		IG_Queue_put(erfasser->queue,data);
	}

	IG_Verarbeiter* verarbeiter = IG_Verarbeiter_create(config, sender, erfasser);

	IG_Verarbeiter_init(verarbeiter);

	// After starting the verarbeiter we just wait and watch the output
	while(1){
		IG_Data* data = IG_Queue_take(sender->queue);
		if(data==NULL) continue;
		printf("Recieved JSON String: %s for OutID: %d \n", (IG_Char*)data->data, data->id);
	}
	

	return 0;
}
