/*
Using a simple command line interface, the user of the program can
launch planes (child processes), and send signals from the parent
process to make the planes refuel or bomb. The command line interface
also allows the user to retrieve a list of live planes or quit the
parent and all child processes. Type `help` for a list of commands
or `q` to quit.
By: AMD
On: 10-31-2020
*/
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

using std::cin;
using std::cout;
using std::endl;
using std::flush;
using std::string;
using std::getline;
using std::vector;

// commands available to the user
// `quit` is also acceptable but is handled differently
enum commands{HELP, STATUS, LAUNCH, REFUEL, BOMB, INVALID_CMD};

// global flags to indicate whether signals have been received
int refuelFlag = 0;
int bombFlag = 0;

// prints a special message if an empty vector is passed otherwise, 
// prints the values held in the vector; values are child process (plane) IDs 
void printStatus(const vector<pid_t>& listOfPlanes);

// prompt and wait for user input; then parse it
// returns 0 if `quit` command or `EOF` is received; otherwise, 1
int parseInput(commands& cmd, pid_t& id);

// removes terminated child processes from the list of child processes
void removeDeadChildren(vector<pid_t>& children);

// sets refuel flag to 1 upon receiving SIGUSR2 from the parent process
void refuelHandler(int signum);

// sets bomb flag to 1 upon receiving SIGUSR1 from the parent process
void bombHandler(int signum);

// print a message if SIGUSR2 is received from a child process
void childCrashHandler(int signum);

// calculate the difference (in seconds) between two times
double getTimeDifference(timespec& begin, timespec& end);

// children processes terminate upon receiving SIGTERM from parent process
void childTerminateHandler(int signum);

// close all child processes
void closeChildren(const vector<pid_t>& children);

int main(int argc, char* argv[]) {

    // child sends SIGUSR2 to parent upon running out of fuel
    signal(SIGUSR2, childCrashHandler);

    // holders for user's desired command and id after being parsed
    commands command;
    pid_t commandID;
    
    // list of process IDs for child processes (planes)
    vector<pid_t> planesList{};

    // holds the process ID for the most recently created child initialized
    // to non-zero value to indicate this is the parent process
    pid_t currentPlaneID = 1;

    // loop until user wants to quit (i.e. program receives `quit`
    // or EOF from stdin is interpreted); loop terminates if run by a child
    // process (i.e. currentPlaneID == 0)
    while ((currentPlaneID != 0) && parseInput(command, commandID)) {

        // remove dead child processes (crashed planes) from the planes list
        removeDeadChildren(planesList);

        switch (command) {

            // print commands
            case HELP:
                cout << "Commands:\n"
                     << "s\t= status: prints out the IDs of all live planes\n"
                     << "l\t= launch: launches a new plane\n"
                     << "r <id>\t= refuel: refuels the plane with the specified ID\n"
                     << "b <id>\t= bomb: drop a bomb from the plane with the specified ID\n"
                     << "q\t= quit: quit the program\n";
                break;

            // print IDs of current children (planes)
            case STATUS:
                printStatus(planesList);
                break;

            // create a new child process (plane)
            case LAUNCH:

                // create a child process and stores its process ID
                currentPlaneID = fork();

                // fork() returns negative value upon error
                if (currentPlaneID < 0) {
                    cout << "There was a problem launching!" << endl;
                }
                // fork() returns the child process ID to the parent process
                else if (currentPlaneID > 0) {

                    // store the ID in the list of children (planes)
                    planesList.push_back(currentPlaneID);
                }
                // fork() returns 0 to the child process; in this case,
                // do nothing, the while loop will terminate and child
                // code in the while loop below will execute
                // else {}

                break; // end of LAUNCH case

            // send SIGUSR2 (refuel) signal to child process
            case REFUEL:
                kill(commandID, SIGUSR2);
                break;

            // send SIGUSR1 (bomb) signal to child process
            case BOMB:
                kill(commandID, SIGUSR1);
                break;

            // print a message if and invalid command is given
            case INVALID_CMD:
                cout << "Invalid command - type `help` for a list of commands."
                     << endl;
                break;

            // should never happen, but if it does, it would be good to know
            // that it happened
            default:
                cout << "If this line is printed, and error has occured." 
                     << endl;
                break;
        }
    }

    // child process code
    if (currentPlaneID == 0) {

        // direct signal SIGTERM to the child terminate handler
        signal(SIGTERM, childTerminateHandler);

        // direct signals SIGUSR1 and SIGUSR2 to the appropriate handlers
        // each handler sets a flag to 1 upon being called
        signal(SIGUSR2, refuelHandler);
        signal(SIGUSR1, bombHandler);

        // fuel obtained upon launch and refuel
        const int FUEL_MAX{100};

        // current fuel level
        int fuel{FUEL_MAX};
        
        // timespec structs hold time from clock_gettime()
        // lastRefuelTime - launch time or refuel time
        // lastNoticeTime - last time a notice of fuel level was printed
        // now - current time; calculated for each loop
        timespec lastRefuelTime, lastNoticeTime, now;

        // measure current time as last refuel time
        if (clock_gettime(CLOCK_REALTIME, &lastRefuelTime) == -1) {
            perror("could not get time");
            exit(EXIT_FAILURE);
        }

        // used to calculate current fuel units
        double timeSinceRefuel;
        
        // used to ensure fuel notices are printed at least 9 seconds apart
        double timeSinceNotice;

        // initialize last notice time; allows for relational operator use
        lastNoticeTime = lastRefuelTime;

        // child process runs continuously until fuel reaches 0
        while (fuel > 0) {

            // sleep between loops (1ms)
            usleep(1000);

            // measure current time and calculate time since last refuel and notice
            clock_gettime(CLOCK_REALTIME, &now);
            timeSinceRefuel = getTimeDifference(lastRefuelTime, now);
            timeSinceNotice = getTimeDifference(lastNoticeTime, now);

            // plane uses 5 units of fuel per 3 seconds
            fuel = FUEL_MAX - (static_cast<int>(timeSinceRefuel / 3) * 5);

            // plane prints a notice of fuel level every 9 seconds if fuel
            // level is below 50 units; don't print if fuel level is 0
            if ((fuel < 50) && (timeSinceNotice > 9) && !(fuel <= 0)) {

                // print notice
                cout << "***Bomber " << getpid() << " to base, "
                     << fuel << " fuel left***\nCommand: " << flush;

                // record notice time
                if (clock_gettime(CLOCK_REALTIME, &lastNoticeTime) == -1) {
                    perror("could not get time");
                    exit(EXIT_FAILURE);
                }
            }

            // if the bomb flag is 1, drop a bomb and reset it to 0
            if (bombFlag == 1) {
                bombFlag = 0;
                cout << "Bomber " << getpid() << " to base, bombs away!"
                     << "\nCommand: " << flush;
            }

            // if refuel flag is 1, reset last refuel time, reset flag to 0,
            // and set fuel value to max
            if (refuelFlag == 1) {
                
                // reset fuel level to max and reset refuel flag
                fuel = FUEL_MAX;
                refuelFlag = 0;

                // record refuel time
                if (clock_gettime(CLOCK_REALTIME, &lastRefuelTime) == -1) {
                    perror("could not get time");
                    exit(EXIT_FAILURE);
                }
            }
        }

        // send SIGUSR2 to parent upon running out of fuel
        kill(getppid(), SIGUSR2);
    }
    // one parent process exits while loop, terminate all children
    else {
        closeChildren(planesList);
    }

    return 0;
}

int parseInput(commands& cmd, pid_t& id) {

    // input from stdin is read into this string
    string input;

    // prompt the user for a command
    cout << "Command: " << flush;

    // read input from stdin into string input
    getline(cin, input);

    // do nothing and return zero if the user would like to end the program
    if (cin.eof()) {cout << endl; return 0; }
    if (input == "q") { return 0; }

    // test whether there is a space in the command; find() returns
    // `npos` if there are no matches; the if statement will catch the
    // commands `launch` and `status`
    if (input.find(" ") == string::npos) {

        // assume it is one of the one-word commands above
        if (input == "help") {
            cmd = HELP;
        }
        else if (input == "l") {
            cmd = LAUNCH;
        }
        else if (input == "s") {
            cmd = STATUS;
        }
        else {
            cmd = INVALID_CMD;
        }
    }
    // if there is a <space> in the command, parse it for the command
    // part as well as the ID part for commands:
    // `bomb <id number>` or `refuel <id number>`
    else {

        // store the command part of a two-part command
        string cmdInput = input.substr(0, input.find(" "));

        // store the id part of the two-part command
        string idInput = input.substr(input.find(" "), input.length() - 1);

        if (cmdInput == "b") {
            cmd = BOMB;
        }
        else if (cmdInput == "r") {
            cmd = REFUEL;
        }
        else {
            cmd = INVALID_CMD;
        }

        // convert ID included with the command to an integer and store it
        id = stoi(idInput, nullptr);
    }

    return 1;
}

void printStatus(const vector<pid_t> &listOfPlanes) {

    // if there are no child processes, inform the user
    if (listOfPlanes.empty()) {

        cout << "There are no planes in the sky!" << endl;
    
    } else {

        cout << "The current planes are: ";

        // print each child process ID followed by a space
        for (int planeID : listOfPlanes) {

            cout << planeID << " ";
        }

        // flush buffer and print newline char
        cout << endl;
    }

    return;
}

void removeDeadChildren(vector<pid_t>& children) {

    // iterate over each plane (child process ID) in the planes list
    for (auto idPtr = children.begin(); idPtr != children.end(); idPtr++) {

        // true if child process has terminated; otherwise, false
        if (waitpid(*idPtr, NULL, WNOHANG)) {

            // erase the terminated child ID from the planes list, and decrement the 
            // iterator so the next value isn't skipped; this is because after
            // deletion, the next value takes the position of the deleted value
            children.erase(idPtr--);
        }
    }
}

void refuelHandler(int signum) {
    refuelFlag = 1;
    return;
}

void bombHandler(int signum) {
    bombFlag = 1;
    return;
}

void childCrashHandler(int signum) {
    cout << "SOS! Plane has crashed!\nCommand: " << flush;
    return;
}

void childTerminateHandler(int signum) {
    exit(EXIT_SUCCESS);
}

double getTimeDifference(timespec& begin, timespec& end) {

    // calculate and return the difference of end and beginning in seconds
    return (end.tv_sec - begin.tv_sec) + ((end.tv_nsec - begin.tv_nsec) / 1e9);
}

void closeChildren(const vector<pid_t>& children) {

    // iterate over each child process id
    for (pid_t childProcessID : children) {

        // send SIGTERM signal to child processes
        kill(childProcessID, SIGTERM);
    }
    
    return;
}
