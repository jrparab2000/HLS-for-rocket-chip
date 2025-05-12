SystemC with RISCV ISA (Spike) Simulation

This introduces transaction-level modeling (TLM)
with the Spike RISCV ISA simulator. 

 - Source the setup script with the command "source ../setup.sh"
 - Build the main.x executable with the command "make"
 - Change to another directory containing the rocket_sim
 - Execute (simulate) the project with the commands "make"
     and  "make sim" in the rocket_sim directory.
 - Repeat the "make" commands in each directory, followed by
     "make sim" in the rocket_sim directory as needed to rebuild
     and re-simulate the project.
 - To run high-level synthesis (HLS), change to the hls directory
     and use the command "make"
 - To run a Verilog simulation with the HLS result, do the following:
     (1) Execute "make" in the vsim directory
     (2) Swap the RISCV_SIM variable in rocket_sim/Makefile
           from the currently used line that runs sc/main.x 
           to the currently unused line that runs vsim/simv
     (3) Execute "make sim" in the rocket_sim directory

Notes:
 - Use the "make clean" command in each directory to delete 
     all generated files, in order to prepare the directory 
     for archiving.

