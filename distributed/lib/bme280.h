#ifndef __BME280__
#define __BME280__

int bme280_init(int iAddr);
int bme280_read_values(int *T, int *P, int *H);
void get_temperature_and_humidity(float *temperature, float *humidity);
void close_bme();

#endif // __BME280__