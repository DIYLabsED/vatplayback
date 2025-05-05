/*
    VATPlayback Archivist
    Command line tool for recording and processing VATSIM network traffic

    (C) 2025 DIY Labs
    Licensed under GNU GPL V3

*/

#include <iostream>
#include <csignal>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


using namespace std;

const string VP_DEFAULT_CONFIG_PATH = "/config.json";
const string VP_LICENSE_URL = "https://www.gnu.org/licenses/gpl-3.0.html";
const string VP_VERSION = "1.0.0";


void safeExit(int s);
void setup();
void processArguments(int argc, char *argv[]);
void printHelp();


int main(int argc, char *argv[]){

    setup();

    processArguments(argc, argv);

    return 0;

}


void safeExit(int s){

    exit(0);

}

void setup(){

    signal(SIGINT, safeExit);

    cout << "VATPlayback Archivist v" + VP_VERSION + "\n"
              << "Copyright (C) 2025 DIY Labs\n"
              << "Licensed under GNU GPL V3. Read more at " + VP_LICENSE_URL + "\n";

}

void processArguments(int argc, char *argv[]){

    if (argc < 2){  // No args, print help and exit
        printHelp();
        safeExit(SIGINT);
    }

}

void printHelp(){

    cout << "Command list:\n"
         << "-h, --help: Print help message/n"
         << "-v, --version: Print version information\n"
         << "-r, --record: Record VATSIM network traffic\n"
         << "-c, --config <config file path>: Specify config file. Uses \"" + VP_DEFAULT_CONFIG_PATH + "\" by default.\n";

}