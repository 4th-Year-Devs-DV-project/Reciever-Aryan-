/**
* Ben Earle and Kyle Bjornson
* ARSLab - Carleton University
*
* Blinky:
* Simple modle to toggle the LED using DEVS internal transitions.
*/

#ifndef BOOST_SIMULATION_PDEVS_RFID_HPP
#define BOOST_SIMULATION_PDEVS_RFID_HPP

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include <limits>
#include <math.h> 
#include <assert.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <random>
#include "../drivers/MFRC522.cpp"
#include <cadmium/real_time/arm_mbed/embedded_error.hpp>

using namespace cadmium;
using namespace std;

//const char* SPI_MISO;
//const char* SPI_MOSI;
//const char* SPI_SCK;
//const char* SPI_CS;
//const char* USB_OTG_FS_VBUS;

//Port definition
    
struct rfid_defs {
        struct dataOut : public out_port<bool> { };
        struct in : public in_port<bool> { };
};


template<typename TIME>
class Rfid	 {
    
    using defs=rfid_defs; // putting definitions in context
    public:
    
    //Parameters to be overwriten when instantiating the atomic model


    TIME   slowToggleTime;
    //TIME   fastToggleTime;

	/* Rfid() noexcept {
		printf("here");
              slowToggleTime  = TIME("00:00:05:00");
              fastToggleTime  = TIME("00:00:10:00");
              state.lightOn = false;
              state.fastToggle = false;
		printf("here");
		
		//state.temp_humid_sensor = new drivers::MFRC522(D11, D12, D13, D10, D8);
		state.temp_humid_sensor = new drivers::MFRC522(D11, D12, D13, D10, D8);
		//state.temp_humid_sensor->PCD_Init();
		//new (this) Blinky(sda, scl, TIME("00:00:01:000"));
		


		 if (state.temp_humid_sensor->PICC_IsNewCardPresent() && state.temp_humid_sensor->PICC_ReadCardSerial() ) 
		{
			printf("found it");
		   // state.temperature = state.temp_humid_sensor->read_temperature();
			state.id[0] = state.temp_humid_sensor->uid.uidByte[0];
			state.id[1] = state.temp_humid_sensor->uid.uidByte[1];
			state.id[2] = state.temp_humid_sensor->uid.uidByte[2];
			state.id[3] = state.temp_humid_sensor->uid.uidByte[3];
			state.id[4] = state.temp_humid_sensor->uid.uidByte[4];
			state.id[5] = state.temp_humid_sensor->uid.uidByte[5];
			state.id[6] = state.temp_humid_sensor->uid.uidByte[6];
			state.id[7] = state.temp_humid_sensor->uid.uidByte[7];
			state.id[8] = state.temp_humid_sensor->uid.uidByte[8];
			state.id[9] = state.temp_humid_sensor->uid.uidByte[9];

		    //state.humidity = state.temp_humid_sensor->read_humidity();
		}
	   	 for (uint8_t i = 0; i < 4; i++)
	    	{
	      		printf(" %X02", state.id[i]);
	    	}
	    	printf("\n\r");
	    	

   	 }
*/
    	//Need to provide valid I2C pins to use this sensor
 	Rfid(PinName sda, PinName scl, PinName scl2, PinName scl3, PinName scl4) {
		slowToggleTime  = TIME("00:00:01:00");
		//fastToggleTime  = TIME("00:00:05:00");
		
		state.lightOn = false;
		state.fastToggle = false;
		state.temp_humid_sensor = new drivers::MFRC522(sda,scl,scl2,scl3,scl4);
		//state.temp_humid_sensor->PCD_Init();
		//new (this) Blinky(sda, scl, TIME("00:00:01:000"));
		
		/*

		 if (state.temp_humid_sensor->PICC_IsNewCardPresent() && state.temp_humid_sensor->PICC_ReadCardSerial() ) 
		{
			printf("found it");
		   // state.temperature = state.temp_humid_sensor->read_temperature();
			state.id[0] = state.temp_humid_sensor->uid.uidByte[0];
			state.id[1] = state.temp_humid_sensor->uid.uidByte[1];
			state.id[2] = state.temp_humid_sensor->uid.uidByte[2];
			state.id[3] = state.temp_humid_sensor->uid.uidByte[3];
			state.id[4] = state.temp_humid_sensor->uid.uidByte[4];
			state.id[5] = state.temp_humid_sensor->uid.uidByte[5];
			state.id[6] = state.temp_humid_sensor->uid.uidByte[6];
			state.id[7] = state.temp_humid_sensor->uid.uidByte[7];
			state.id[8] = state.temp_humid_sensor->uid.uidByte[8];
			state.id[9] = state.temp_humid_sensor->uid.uidByte[9];

		    //state.humidity = state.temp_humid_sensor->read_humidity();
		}
		printf("sdddddddddd\n");
	   	 for (uint8_t i = 0; i < 4; i++)
	    	{
	      		printf(" %X02", state.id[i]);
	    	}
	    	printf("\n\r");
	    	*/

	  }
	/////////////////////////////////////////////////////////////

            
            // state definition
            struct state_type{
				uint8_t id[10];
        		drivers::MFRC522* temp_humid_sensor;
				bool newTag;
            	bool lightOn;
              	bool fastToggle;
            }; 
            state_type state;
            // ports definition

            //using input_ports=std::tuple<>;
	   		using input_ports=std::tuple<typename defs::in>;
            using output_ports=std::tuple<typename defs::dataOut>;

            // internal transition
            void internal_transition() {
            	printf("internal");
            	state.newTag = false;
            	
	            //
	            if (state.temp_humid_sensor->PICC_ReadCardSerial() ) 
	            	printf("true");
	             if (state.temp_humid_sensor->PICC_IsNewCardPresent() )
	             	printf("true2");
	            for(int i =0; i <5; i++)
		        {
		            if (state.temp_humid_sensor->PICC_IsNewCardPresent() && state.temp_humid_sensor->PICC_ReadCardSerial() ) 
		            //if (state.temp_humid_sensor->PICC_IsNewCardPresent())
					{
						//wait_ms(500);
						state.newTag = true;
						printf("fouoooooooooooooooooooooooooooooooooooooooond it");
						   // state.temperature = state.temp_humid_sensor->read_temperature();
						state.id[0] = state.temp_humid_sensor->uid.uidByte[0];
						state.id[1] = state.temp_humid_sensor->uid.uidByte[1];
						state.id[2] = state.temp_humid_sensor->uid.uidByte[2];
						state.id[3] = state.temp_humid_sensor->uid.uidByte[3];
						state.id[4] = state.temp_humid_sensor->uid.uidByte[4];
						state.id[5] = state.temp_humid_sensor->uid.uidByte[5];
						state.id[6] = state.temp_humid_sensor->uid.uidByte[6];
						state.id[7] = state.temp_humid_sensor->uid.uidByte[7];
						state.id[8] = state.temp_humid_sensor->uid.uidByte[8];
						state.id[9] = state.temp_humid_sensor->uid.uidByte[9];
					}else{
							state.newTag = false;
					}
					if( state.newTag == true)
					{
					   	for (uint8_t i = 0; i < 10; i++)
					    {
					    	printf(" %X02", state.id[i]);
					    }
					    printf("\n\r");
					    break;
					}
				}	    
            }


            // external transition
            void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {

            }
            // confluence transition
            void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
              internal_transition();
              external_transition(TIME(), std::move(mbs));
            }

            // output function
            typename make_message_bags<output_ports>::type output() const {
              typename make_message_bags<output_ports>::type bags;
              bool out;              
              out = (state.newTag ? 1 : 0);
              get_messages<typename defs::dataOut>(bags).push_back(out);
                
              return bags;
            }

            // time_advance function
            TIME time_advance() const {  
              //if(state.fastToggle)
                //return fastToggleTime;
              //else 
                return slowToggleTime;
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename Rfid<TIME>::state_type& i) {
              os << "Output: " << (i.newTag ? 1 : 0); 
              return os;
            }
        };     
#endif // BLINKY_HPP

