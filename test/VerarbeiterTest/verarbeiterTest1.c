#include "verarbeiter.h"

int main(int argc, char* argv[]){
	
	// Setup config
	IG_Config* config = IG_Config_create(argv[1], IG_CONFIG_STANDARD);


	// Setup versender
	IG_Datenversender * sender = IG_Datenversender_create(config)

	return 0;
}
