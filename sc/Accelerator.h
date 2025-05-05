/*
 * 
 */
#pragma once

#include "nvhls_pch.h"
#define TAPS 16
#define TSTEP1 32
#define TSTEP2 48

SC_MODULE(Accelerator)
{
    public:
        sc_in_clk     clk;
        sc_in<bool>   rst;

        typedef sc_uint<64> Data;


        sc_out<sc_uint<8>> st_out;
        Connections::In<sc_uint<8>> ctrl_in;
        Connections::In<sc_uint<64>> w_in;
        Connections::In<sc_uint<64>> x_in;
        Connections::Out<sc_uint<64>> z_out;

        SC_HAS_PROCESS(Accelerator);
        Accelerator(sc_module_name name_) : sc_module(name_),
        st_out("st_out"), ctrl_in("ctrl_in"), w_in("w_in"), x_in("x_in"), z_out("z_out")
        {
            SC_THREAD (run); 
            sensitive << clk.pos(); 
            NVHLS_NEG_RESET_SIGNAL_IS(rst);
        }

        void run()
        {
            ctrl_in.Reset();
            w_in.Reset();
            x_in.Reset();
            z_out.Reset();
            
            int temp;
            if(TSTEP1 <= TSTEP2)
                temp = TSTEP2;
            else
                temp = TSTEP1;
            sc_uint<16> data_i_1[temp];

            sc_uint<64> data_w;
            sc_uint<16> data_w_1[TAPS];

            sc_uint<16> output_1[temp];

            temp =0;
            int i=0;
            bool done;
            sc_uint<8> ctrl=0;
            sc_uint<8> addr=0;

            st_out.write(ctrl);
            wait();                  // wait separates reset from operational behavior
            while (1)
            {
                if (!ctrl_in.Empty()) {
                    ctrl=ctrl_in.Pop();
                    i=0;
                    done = 0;
                }
                if((ctrl == 0x01)&&(done == 0))
                {
                    if (!w_in.Empty()) {
                        data_w=w_in.Pop();
                        data_w_1[i*4]=data_w.range(15,0);
                        data_w_1[i*4+1]=data_w.range(31,16);
                        data_w_1[i*4+2]=data_w.range(47,32);
                        data_w_1[i*4+3]=data_w.range(63,48);
                        i++;
                        if(i == TAPS/4)
                        {
                            i = 0;
                            done = 1;
                        }
                    }
                }
                else if(((ctrl == 0x02)||(ctrl == 0x03))&&(done == 0))
                {
                    if((ctrl == 0x02))
                    {
                        temp = TSTEP1;
                    }
                    else if((ctrl == 0x03))
                    {
                        temp = TSTEP2;
                    }
                    if(!x_in.Empty())
                    {
                        data_w = x_in.Pop();
                        data_i_1[i*4] = data_w.range(15,0);
                        data_i_1[i*4+1]=data_w.range(31,16);
                        data_i_1[i*4+2]=data_w.range(47,32);
                        data_i_1[i*4+3]=data_w.range(63,48);
                        i++;
                        if(i == temp/4)
                        {
                            i = 0;
                            done = 1;
                        }
                    }
                }
                

                else if(((ctrl == 0x04)||(ctrl == 0x05))&&(done == 0))
                {
                    if(ctrl == 0x04)
                    {
                        temp = TSTEP1;
                    }
                    else if(ctrl == 0x05)
                    {
                        temp = TSTEP2;
                    }
                    i = 0;
                    mult_acc_1: for (int n=0; n<temp; n++) {
                        output_1[n]=0;
                        for (int m=0; m<TAPS; m++) {
                            if (n+m-TAPS+1 >= 0) {
                                output_1[n]+=data_w_1[m]*data_i_1[n+m-TAPS+1];
                                }
                        }
                        if(n%4 == 0)
                        {
                            data_w.range(15,0) = output_1[n];
                        }
                        else if(n%4 == 1)
                        {
                            data_w.range(31,16) = output_1[n];
                        }
                        else if(n%4 == 2)
                        {
                            data_w.range(47,32) = output_1[n];
                        }
                        else if(n%4 == 3)
                        {
                            data_w.range(63,48) = output_1[n];
                        }
                        i++;
                        if(i == 4)
                        {
                            z_out.Push(data_w);
                            i = 0;
                        }  
                        
                    }
                    i = 0;
                    done = 1;
                }

                if((ctrl != 0x00)&&(done == 1))
                {
                    st_out.write(ctrl);
                }
                wait();
            }
        }
};

