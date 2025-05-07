/*
    VATPlayback Archivist
    Command line tool for recording and processing VATSIM network traffic

    (C) 2025 DIY Labs
    Licensed under GNU GPL V3

*/

#include <iostream>
#include <fstream>
#include <csignal>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

struct VP_CONFIG{

    string apiURL = "https://data.vatsim.net/v3/vatsim-data.json";
    string recordPath = "recordings/";
    string processPath = "processed/";

};

const string VP_DEFAULT_CONFIG_PATH = "config.json";
const string VP_LICENSE_URL = "https://www.gnu.org/licenses/gpl-3.0.html";
const string VP_VERSION = "1.0.0";

VP_CONFIG globalConfig;

void safeExit(int s);
bool readConfigFile(string fp, VP_CONFIG *outConfig);
bool createConfigFile(string fp, VP_CONFIG inputConfig);  
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

bool createConfigFile(string fp, VP_CONFIG inputConfig){

    ofstream configFile(fp);
    json cfg;


    if(!configFile.is_open()){
        return false;
    }

    cfg["apiURL"] = inputConfig.apiURL;
    cfg["recordPath"] = inputConfig.recordPath;
    cfg["processPath"] = inputConfig.processPath;


    configFile << cfg.dump(4); 
    configFile.close();
    return true;

}

bool readConfigFile(string fp, VP_CONFIG *outConfig){

    ifstream configFile(fp);
    json cfg;

    if(!configFile.is_open()){
        return false;
    }

    configFile >> cfg;
    outConfig->apiURL = cfg["apiURL"].get<string>();
    outConfig->recordPath = cfg["recordPath"].get<string>();
    outConfig->processPath = cfg["processPath"].get<string>();

    configFile.close();
    return true;

}

void setup(){
    
    // Attempt to read config file. If it fails, create a new one with default values.    
    if(!readConfigFile(VP_DEFAULT_CONFIG_PATH, &globalConfig)){

        cout << "Failed to read config file at: " << VP_DEFAULT_CONFIG_PATH << ". Creating new config file with default values.\n";

        if(!createConfigFile(VP_DEFAULT_CONFIG_PATH, VP_CONFIG())){

            cout << "Failed to create config file at: " << VP_DEFAULT_CONFIG_PATH << ". Exiting.\n";
            safeExit(SIGINT);

        }
        else{
                
            cout << "Config file created at: " << VP_DEFAULT_CONFIG_PATH << ".\n";
            globalConfig = VP_CONFIG(); // Use default config values

        }

    }
    // globalConfig is populated

    cout << globalConfig.apiURL << "\n"
         << globalConfig.recordPath << "\n"
         << globalConfig.processPath << "\n";  

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
         << "-h, --help: Print help message\n"
         << "-v, --version: Print version information\n"
         << "-r, --record: Record VATSIM network traffic\n"
         << "-c, --config <config file path>: Specify config file. Uses \"" + VP_DEFAULT_CONFIG_PATH + "\" by default.\n";

}