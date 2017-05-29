#include "internals.h"

#define BUFFSIZE 30

IG_Data * IG_Data_create(IG_Id id, IG_Datatype type, void * dataptr, IG_DateTime timestamp) {
    IG_Data * data = (IG_Data *) malloc(sizeof(IG_Data));
    data->id = id;
    data->datatype = type;
    data->data = dataptr;
    data->timestamp = timestamp;
    return data;
}

//care this doesnt free the memory needed for the data pointer
void IG_Data_delete(IG_Data * data){
    free(data);
}

// this frees the data ptr inside
void IG_Data_delete_members(IG_Data * data){
    free(data->data);
}


IG_Char * IG_Data_toString(IG_Data* data){
	IG_Char* value = malloc(BUFFSIZE);
	void* dataToEncode = data->data;
	switch(data->datatype){
		case IG_DOUBLE:
		case IG_FLOAT:
			snprintf(value, BUFFSIZE, "{value:%f}", *((float*)dataToEncode));
			break;
		case IG_INT32:
			snprintf(value, BUFFSIZE, "{value:%i}", *((int*)dataToEncode));
			break;
		case IG_UINT32:
			snprintf(value, BUFFSIZE, "{value:%u}", *((unsigned int*)dataToEncode));
			break;
		case IG_INT64:
			snprintf(value, BUFFSIZE, "{value:%ld}",*((long int*)dataToEncode));
			break;
		case IG_UINT64:
		case IG_DATETIME:
			snprintf(value, BUFFSIZE, "{value:%lu}",*((long unsigned int*)dataToEncode));
			break;
		case IG_BOOL:
			snprintf(value, BUFFSIZE, "{value:%s}", *((bool*)dataToEncode)?"TRUE":"FALSE");
			break;
		case IG_NULL:
			snprintf(value, BUFFSIZE, "{value:NULL}");
			break;
		default:
			// Maybe add a log here to inform there was a failure
			break;
	}
	return value;
}
