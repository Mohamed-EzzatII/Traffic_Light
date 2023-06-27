#include"APP/app.h"
int main()
{

	APP_init();

	while(1){
		APP_traffic_light_control();
	}
}
