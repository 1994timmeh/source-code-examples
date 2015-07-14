// Includes
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "user_config.h"
#include "user_interface.h"
#include "espconn.h"

// Defines
#define user_procTaskPrio        0
#define user_procTaskQueueLen    1

// Variables
os_event_t    user_procTaskQueue[user_procTaskQueueLen];

// Functions
static void loop(os_event_t *events);
static void recv_callback (void *arg);

// Recv data callback
static void recv_callback (void *arg){
	os_printf("Recieved data (apparently)\n\r");
}

//Main code function
static void ICACHE_FLASH_ATTR loop(os_event_t *events) {  
	uint8_t status = wifi_station_get_connect_status();  
    os_printf("Status %d\n\r", status);
    
    //Do blinky stuff
    if (GPIO_REG_READ(GPIO_OUT_ADDRESS) & BIT2) {
        //Set GPIO2 to LOW
        gpio_output_set(0, BIT2, BIT2, 0);
    } else {
        //Set GPIO2 to HIGH
        gpio_output_set(BIT2, 0, BIT2, 0);
    }
    
    os_delay_us(1000000);
    
    // Tell FreeRTOS that this task is complete
    system_os_post(user_procTaskPrio, 0, 0 );
}

void user_set_station_config(){
	char ssid[32] = SSID;
    char password[64] = SSID_PASSWORD;
    struct station_config stationConf;
    stationConf.bssid_set = 0; //need not check MAC address of AP
   
    os_memcpy(&stationConf.ssid, ssid, 32);
    os_memcpy(&stationConf.password, password, 64);
    wifi_station_set_config(&stationConf);
}

//Init function 
void ICACHE_FLASH_ATTR user_init() {
    char ssid[32] = SSID;
    char password[64] = SSID_PASSWORD;
    struct station_config stationConf;
    struct espconn espconn;
    struct _esp_tcp tcp_con;
    gpio_init();
    
    os_delay_us(3000000);
    
    //Set GPIO2 to output mode
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    
    //Set GPIO2 low
    gpio_output_set(0, BIT2, BIT2, 0);
    
    // Set the USART speed to be 9600 baud for debug prints
    uart_div_modify(0, UART_CLK_FREQ / 9600);
    
    wifi_set_opmode(STATIONAP_MODE);
    user_set_station_config();

	/*tcp_con.remote_port = 2000;
    tcp_con.local_port = 2000;
    tcp_con.local_ip[0] = 10;
    tcp_con.local_ip[1] = 0;
    tcp_con.local_ip[2] = 1;
    tcp_con.local_ip[3] = 100;
    
    tcp_con.remote_ip[0] = 10;
    tcp_con.remote_ip[1] = 0;
    tcp_con.remote_ip[2] = 1;
    tcp_con.remote_ip[3] = 100;
    
    espconn.type = ESPCONN_TCP;
    espconn.state = ESPCONN_LISTEN;
    espconn.proto.tcp = &tcp_con;
    espconn.recv_callback = (void*)recv_callback;
   
	espconn_accept(&espconn);*/
	
    //Start os task
    system_os_task(loop, user_procTaskPrio,user_procTaskQueue, user_procTaskQueueLen);
    
	// Tell FreeRTOS that this task has completed for now (ever in this case)
    system_os_post(user_procTaskPrio, 0, 0 );
}
