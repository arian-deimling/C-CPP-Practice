# Planes
### By: Arian Michael Deimling
### On: 10-31-2020

<p>This program demonstrates the creation of child processes using C Standard Library function <code>fork()</code> and communication between a parent process and its child processes using the C Standard Library function <code>kill()</code>.</p>

<p>Using a simple command line interface, the user of the program can launch planes (child processes), send signals to the planes, and check what child processes currently exist. The child processes gradually consume fuel and terminate once their fuel level drops to zero.
<br><br>Commands:
  <ul>
    <li>s - status: prints out the IDs of all live child processes (planes)</li>
    <li>l - launch: launches a new plane (creates a new child process)</li>
    <li>r {id} - refuel: refuel the plane with the specified ID</li>
    <li>b {id} - bomb: signal the plane with the specified ID to drop a bomb</li>
    <li>q {id} - quit: close all child processes as well as the parent process</li>
    <li>help - help: print out a list of commands</li>
</p>
