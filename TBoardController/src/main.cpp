#include <TBoard.h>
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <MCP79410.h>
#include <AutoTimer.h>
#include <Controller.h>
#include <MilliTimer.h>
#include <Timer.h>
#include "m_general.h"
#include "m_usb.h"
//#include <USBSerial.h>
#define TIME_DATE_BUFFERSIZE 12
#define DATETIMERESPONDSIZE  11
#define BUFFERSIZE TIME_DATE_BUFFERSIZE+TIME_DATE_BUFFERSIZE

uint8_t  EEMEM  CurrentStore;
//MilliTimer milli_timer;
//ADC_POT adc_driver;
//Timer timer(&milli_timer);
//CurrentDriver currentdriver(&adc_driver);
//Controller controller(&timer,&currentdriver,&CurrentStore);

typedef struct SERIAL_DATA{
	uint8_t time_count;
	uint8_t date_count;
	uint8_t buffer_count;
	char time_buffer[BUFFERSIZE];
	char date_buffer[BUFFERSIZE];
	char buffer[BUFFERSIZE+BUFFERSIZE];
	//char respondbuffer[DATETIMERESPONDSIZE];
}SERIAL_DATA;

ISR(TIMER0_OVF_vect){
//	milli_timer.isr();
}

ISR(TIMER1_OVF_vect){
	
}

ISR(ADC_vect){
	
}

int main_func();
void TestingUSB();
struct tm new_time;

int main(void){
	TestingUSB();
}

void TestingUSB(){
	SERIAL_DATA serialData;
	int size=0;
	time_t now_t;
	struct tm now_tm;
	MCP79410 rtc;
	rtc.initialize(FALSE);
	m_usb_init();
	while(!m_usb_isconnected());
	while(1){
		bool read=false;	
		if(m_usb_rx_available()){
			int count=0;
			char value=m_usb_rx_char();
			char *temp;
			uint8_t *tempCounter;
			serialData.date_count=0;
			serialData.time_count=0;
			while(value!='\r' && -1!=(int8_t)value && count<255){	
				if(value=='d'){
					temp=serialData.date_buffer;
					tempCounter=&serialData.date_count;
				}else if(value=='t'){
					temp=serialData.time_buffer;
					tempCounter=&serialData.time_count;
				}else if(value=='r'){
					read=true;
				}
				if(!read){
					temp[*tempCounter]=value;
					*tempCounter+=1;
					//serialData.buffer[count]=value;
					//count+=1;
				}
				value=m_usb_rx_char();
			}
			if(read){
					now_t=rtc.Now();
					gmtime_r(&now_t,&now_tm);
					m_usb_tx_int(now_tm.tm_mon+1);	
					m_usb_tx_char('-');				
					m_usb_tx_int(now_tm.tm_mday);	
					m_usb_tx_char('-');				
					m_usb_tx_int(now_tm.tm_year+1900);
					m_usb_tx_char(' ');				
					
					m_usb_tx_int(now_tm.tm_hour);	
					m_usb_tx_char(':');			
					m_usb_tx_int(now_tm.tm_min);	
					m_usb_tx_char(':');			
					m_usb_tx_int(now_tm.tm_sec);
					m_usb_tx_char('\n');
			}else{
				for(int i=0;i<serialData.date_count;i++){
					
					m_usb_tx_char(serialData.date_buffer[i]);
				}
				m_usb_tx_char(' ');
				for(int i=0;i<serialData.time_count;i++){
					m_usb_tx_char(serialData.time_buffer[i]);
				}
				//m_usb_tx_char('\n');
			}

		}
	}
}

void ParseBuffer(){
	//char command=this->serialData.buffer[0];
	//if(command=='d'){
		//if(serialData.buffer[1]==':'){
			//for(int i=2;i<TIME_DATE_BUFFERSIZE;i++){
				//this->serialData.date_buffer[i]=this->serialData.buffer[i];
			//}
		//}
		//}else if(command=='t'){
		//
	//}
}

void TestUSBClass(){
		//USBSerial usb;
		//usb.Initialize();
		//usb.Connect();
		//while(1){
			//usb.Read();
			//usb.Respond();
		//}
}
//void showTime(){
	//now_t=rtc.Now();
	//gmtime_r(&now_t,&now_tm);			
	//m_usb_tx_int(now_tm.tm_mon+1);	0
	//m_usb_tx_char('-');				1
	//m_usb_tx_int(now_tm.tm_mday);		2
	//m_usb_tx_char('-');				3
	//m_usb_tx_int(now_tm.tm_year+2000);4
	//m_usb_tx_char(' ');				5
	//
	//m_usb_tx_int(now_tm.tm_hour);		6
	//m_usb_tx_char(':');				7
	//m_usb_tx_int(now_tm.tm_min);		8
	//m_usb_tx_char(':');				9
	//m_usb_tx_int(now_tm.tm_sec);		10
//}

//int main_func(){
		//controller.setup(SET_TIME);
//
		//controller.start();
		//while(1){
			//controller.check_switchs();
		//}
//}