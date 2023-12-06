This is my ksim kernel simulator. To build and run:

make
./ksim



after the program is running, you will be prompted as follows:

ksim-000000064>

ksim shows the name of the simulator, it displays the number of ticks
in base 10 format, and prompts the user for input. These are the
ONLY permissible inputs:

add <proc-name>         --Adds a process with and names it <proc-name>. Does not allow multiple processes with same name. 
query <proc-name>       --Prints process with name <proc-name>. If no <proc-name> is given, prints all current processes.
wait <io-dev>           --If there is currently a process running, that process blocks and begins waiting for <io-dev>
io-event <io-dev>       --Initiates an I/O event for <io-dev>. All processes waiting on <io-dev> are now READY.
release                 --If there is currently a process running, that process is moved to EXITING
step                    --Do some housekeeping and advance process states based on 5 state model.
exit                    --Free all dynamically allocated memory and then exit.


The error checking is currently kept to a minimum. Don't fat finger after you have entered all 20 of your precious proccesses
or you will be sad about your seg fault. This program is not for the unwashed masses yet.
