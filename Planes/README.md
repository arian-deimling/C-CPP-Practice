# Planes
### By: Arian Michael Deimling
### On: 10-31-2020

<p>This program demonstrates the creation of child processes using C Standard Library function <code>fork()</code> and communication between a parent process and its child processes using the C Standard Library function <code>kill()</code>.</p>

<p>Using a simple command line interface, the user of the program can launch planes (child processes), send signals to the planes, and check what child processes currently exist. The child processes gradually consume fuel and terminate once their fuel level drops to zero.
Commands:
  - s - status: prints out the IDs of all live child processes (planes)</li>
  - l - launch: launches a new plane (creates a new child process)</li>
</p>
      
                cout << "Commands:\n"
                     << "s\t= status: prints out the IDs of all live planes\n"
                     << "l\t= launch: launches a new plane\n"
                     << "r <id>\t= refuel: refuels the plane with the specified ID\n"
                     << "b <id>\t= bomb: drop a bomb from the plane with the specified ID\n"
                     << "q\t= quit: quit the program\n";
Using a simple command line interface, the user of the program can
launch planes (child processes), and send signals from the parent
process to make the planes refuel or bomb. The child processes 
gradually consume fuel and are terminated once their fuel level
drops to zero. The command line interface also allows the user to
retrieve a list of live child process IDs or quit the parent and 
all child processes. Type `help` for a list of commands or `q` to 
quit.
