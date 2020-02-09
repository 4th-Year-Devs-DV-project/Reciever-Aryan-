/**
* Ben Earle and Kyle Bjornson
* ARSLab - Carleton University
*
* Blinky:
* Simple modle to toggle the LED using DEVS internal transitions.
*/

#ifndef BOOST_SIMULATION_PDEVS_receiver_HPP
#define BOOST_SIMULATION_PDEVS_receiver_HPP

#include <stdio.h>
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
#include "../drivers/nRF24L01P.h"

#define TRANSFER_SIZE   4

using namespace cadmium;
using namespace std;

//Port definition
    struct receiver_defs {
        struct dataOut : public out_port<bool> { };
        struct in : public in_port<bool> { };
    };

    template<typename TIME>
    class receiver {
        using defs=receiver_defs; // putting definitions in context
        public:
            //Parameters to be overwriten when instantiating the atomic model
            TIME   slowToggleTime;
            TIME   fastToggleTime;
            // default constructor
            receiver(PinName s, PinName t, PinName q, PinName w, PinName r, PinName a){
              slowToggleTime  = TIME("00:00:10:00");
              fastToggleTime  = TIME("00:00:10:00");
              state.temp = new nRF24L01P(s,t,q,w,r,a);
              state.newTag = false;
              state.isCard = false;
              state.lightOn = false;
              state.secondCar = false;
              state.fastToggle = false;

            }

            // state definition
            struct state_type{
              char s[32] = " " ;
              nRF24L01P* temp;
              bool newTag;
              bool isCard;
              bool lightOn;
              bool secondCar;
              bool fastToggle; 
            };
            state_type state;
            // ports definition

            using input_ports=std::tuple<typename defs::in>;
            using output_ports=std::tuple<typename defs::dataOut>;

            // used to power up the antenna once
            int w = 0 ;

            // internal transition
            void internal_transition() {
              if(w==0){
                  state.temp->powerUp();
                  w= 1;
                }
                state.fastToggle = 0;
              state.newTag = 0;
              state.secondCar = 0;
              state.temp->setTransferSize(32);
              state.temp->setReceiveMode();
              state.temp->enable();
              //for(int a = 0; a <= 2 ; a = a + 1) {
              for (int b = 0 ; b <=200; b= b+1){

                 if( state.temp->readable(0)) {
                     state.temp->read(0, state.s,sizeof(state.s)); // reading hello world
                     printf( "Receiving data :%s  \n",state.s);
                     printf("\n");
                     printf("\n");
                     printf("\n");
                     state.newTag = 1;
                    
                     if(state.s[5] == '1')
                     {
                       if(state.s[0] == '1')
                       {
                          state.isCard = 1;
                          
                       }
                       else
                       {
                        state.isCard = 0;
                        state.lightOn =! state.lightOn; 
                        
                       }
                     }
                     else if(state.s[5] == '2')
                     {
                      printf("-------------------- \n");
                        state.secondCar = 1;
                        state.lightOn =! state.lightOn; 
                        
                        if(state.s[0] == '0' )
                        {
                          state.fastToggle = 1;
                          printf("her?--------------------");
                        }
                        
                     }

                    
                     break;
                  }//if

            
                    else{
                         printf( "not receiving data  \n");
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
              printf("new :%d\n", state.newTag );
               printf("light: %d",state.lightOn);
              if(state.newTag == 1 )
              {
                //out = 1;
                  if(state.secondCar)
                  {
                    out = state.lightOn;
                  }
                  else if(state.isCard == 1)
                  {
                    printf("fucccccccccccccccccccccccccccccccccccck\n");
                    out = 1;  
                  }
                  
                  else
                  {
                    out = state.lightOn;
                  }

              }
              else
              {
                
                out = 0;
              }

              printf("out : %d",out);
              
              //out = (state.newTag ? 1 : 0);
              get_messages<typename defs::dataOut>(bags).push_back(out);

              return bags;
            }

            // time_advance function
            TIME time_advance() const {
                if(state.fastToggle)
                  return fastToggleTime;
                else
                  return slowToggleTime;
            }

            friend std::ostringstream& operator<<(std::ostringstream& os, const typename receiver<TIME>::state_type& o) {
              os << "Output: " << (o.newTag ? 1 : 0);
              return os;
            }
        };


#endif
