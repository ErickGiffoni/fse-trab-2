#ifndef SENSORS_H
#define SENSORS_H

extern volatile int sensors_values[];
extern volatile float temperature, humidity;

extern char output_sensor_names[10][50];
extern char input_sensor_names[13][55];

void receive_all_sensors_data(char* message);
void update_sensor_data(char * payload);
void update_temp_and_humidity(char * payload);
void update_gpio_device_value(char * payload);

#endif /* SENSORS_H */