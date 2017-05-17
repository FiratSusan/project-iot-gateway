
#include<stdio.h>
#include "config_mqtt.h"

int main(void) {
    IG_Config * conf = IG_Config_create("mqtt_config.xml",IG_CONFIG_MQTT);
    if(conf == NULL) {
        printf("wrong xml root node\n");
        return -1;
    }

    IG_ConfigResponse mqttconfig_res;
    IG_Status rt = IG_Config_MQTT_get_ClientConfig(conf,&mqttconfig_res);
    if(rt != IG_STATUS_GOOD) {
        printf("ConfigRequest failed\n");
        return -1;
    }

    IG_Config_MQTT_ClientConfig * mqttconfig = (IG_Config_MQTT_ClientConfig*) mqttconfig_res.data;
    printf("MQTTClientConfig:\n\tconn_string=\t'%s'\n\tclient_name=\t'%s'\n\tqos_level=\t'%i'\n",
            mqttconfig->conn_string,
            mqttconfig->client_name, 
            mqttconfig->qos_level
            );


    return 0;
}