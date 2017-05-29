#include "verarbeiter.h"

IG_Verarbeiter * IG_Verarbeiter_create(IG_Config * config, IG_Datenversender * sender, IG_Datenerfasser * erfasser){
    //create new on the heap
    IG_Verarbeiter * arbeiter = (IG_Verarbeiter *) malloc(sizeof(IG_Verarbeiter));
    arbeiter->config = config;
    arbeiter->sender = sender;
    arbeiter->erfasser = erfasser;
    return arbeiter;
}


void IG_Verarbeiter_delete(IG_Verarbeiter * verarbeiter) {
    free(verarbeiter);
}

IG_Status init_verarbeiter(IG_Verarbeiter * verarbeiter){
	IG_ConfigResponse res;
	IG_Status rt = IG_Config_Verarbeiter_get_RuleSets(conf,&res);
     	IG_WorkLoopArgs* args = (IG_WorkLoopArgs*)malloc(sizeof(IG_WorkLoopArgs));
	args->verarbeiter = verarbeiter;
	args->ruleSetSize = res.responseAmount;
	args->ruleSetArray = (IG_Input_RuleSet *) res.data;

	//Assign function pointers
	IG_Verarbeiter_initFunktionen(args->ruleSetArray, args->ruleSetSize);
	
	pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t));
	
	pthread_create(thread,NULL,IG_WorkLoop,(void*)&args);

	return IG_STATUS_GOOD;
}

void* IG_WorkLoop(void * argument){
	// Parse argmuents
	IG_WorkLoopArgs args = *(IG_WorkLoopArgs *)argument;
	IG_Verarbeiter * verarbeiter = args.verarbeiter;
	IG_UInt32 ruleSetSize = args.ruleSetSize;
	// Array of RuleSets
	IG_Input_RuleSet * ruleSetArray = args.ruleSetArray;
	
	IG_Queue * queueErfasser = verarbeiter->erfasser->queue;
	IG_Queue * queueSender = verarbeiter->sender->queue;

	// endless loop
	while(1){
		/*
		Maybe add some special variable which terminates loop directly (For error cases)
		if(fatalError) break;
		*/
		if(!verarbeiter->running && IG_Queue_isEmpty(queueErfasser))
			break;
		
		IG_Data* data = IG_Queue_take(queueErfasser);

		// If there is data apply all rules
		if(data!=NULL)
			IG_Verarbeiter_applyRules(data,ruleSetArray, ruleSetSize);

		IG_Verarbeiter_checkIntervals(ruleSetArray, ruleSetSize, queueSender);
		// TODO: Maybe add some functions to create logs/print status
	}
}

void IG_Verarbeiter_applyRules(IG_Data * data,IG_Input_RuleSet * ruleSetArray, IG_Int32 ruleSetSize){
	// Find the correct RuleSet
	for(IG_UInt32 i = 0; i < ruleSetSize; i++){
		if(ruleSetArray[i].inputId = data->id){
			// Apply entire RuleSet on data
			IG_Verarbeiter_applyRule(data, &ruleSetArray[i]);
			break;
		}				
	}

}

void IG_Verarbeiter_applyRule(IG_Data * data, IG_Input_RuleSet* ruleSet){
	// Call all rule functions and invoke the data and the rule specific data
	for(IG_UInt32 i = 0; i < (ruleSet->ruleSize);i++){
		(*(ruleSet->rules[i].function))(data, &ruleSet->rules[i]);
	}
}

void IG_Verarbeiter_checkIntervals(IG_Input_RuleSet * ruleSetArray, IG_Int32 ruleSetSize, IG_Queue * queue){
	// Go through every rule and check if to Send
	IG_DateTime now = IG_DateTime_now();
	for(IG_UInt32 i = 0; i < ruleSetSize; i++){
		IG_Input_RuleSet * ruleSet = &(ruleSetArray[i]);
		for(IG_UInt32 j = 0; j < ruleSet->ruleSize; j++){
			if(ruleSet->rules[j].deadline < now){
				IG_Data * dataToSend = malloc(sizeof(IG_Data));
				dataToSend->id = ruleSet->rules[j].outputid;
				dataToSend->datatype = IG_CHAR;
				dataToSend->data = (void*)IG_Data_toString(ruleSet->rules[j].data);
				dataToSend->timestamp = IG_DateTime_now();
				IG_Queue_put(queue, dataToSend);
				rule->deadline = IG_DateTime_add_duration(rule->deadline,rule->interval);
			}	
		}							
	}
}

void IG_Verarbeiter_initFunktionen(IG_Input_RuleSet* ruleSetArray, IG_UInt32 ruleSetSize){
	for(IG_UInt32 i = 0; i < ruleSetSize; i++){
		IG_InputRuleSet* ruleSet = &(ruleSetArray[i]);
		for(IG_UInt32 j = 0; j < ruleSet->ruleSize; j++){
			IG_Rule* rule = &(ruleSet[j]);)
			switch(rule->rule){
				case IG_RULE_TRANSMIT:
					rule->funcPtr = IG_Transmit;
					break;
				case IG_RULE_AVG:
					rule->funcPtr = IG_Average;
					break;
				case IG_RULE_MAX:
					rule->funcPtr = IG_Maximum;
					break;
				case IG_RULE_MIN:
					rule->funcPtr = IG_Minimum;
					break;
			}			
		}
	}
}

void IG_Transmit(IG_Data* data, IG_Rule * rule){
	rule->data = data;
}
void IG_Average(IG_Data* data, IG_Rule * rule){
	*((int*)rule->data->data
}
void IG_Maximum(IG_Data* data, IG_Rule * rule){
	switch(data->datatype){
		case IG_FLOAT:
			IG_Float dataVal1, dataVal2;
			break;
		case IG_DOUBLE:
			IG_Double dataVal1, dataVal2;
		case IG_INT32:
			IG_Int32 dataVal1, dataVal2;
			break;
		case IG_UINT32:
			IG_UInt32 dataVal1, dataVal2;
			break;
		case IG_INT64:
			IG_Int64 dataVal1, dataVal2;
			break;	
		case IG_UINT64:
		case IG_Duration:
		case IG_DateTime:
			IG_UInt64 dataVal1, dataVal2;
			break;	
		case IG_BYTE:
			IG_Byte dataVal1, dataVal2;
			break;
		case IG_CHAR:
			IG_Char dataVal1, dataVal2;
			break;
		case IG_BOOL:
			IG_Bool dataVal1, dataVal2;
	}


	if(dataVal1 > dataVal2) return;
	rule->data = data;
}
void IG_Minimum(IG_Data* data, IG_Rule * rule){
	if(*((int*)rule->data->data < *((int*)data->data)) return;
	rule->data = data;
}
