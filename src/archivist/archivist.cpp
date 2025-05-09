/*
    VATPlayback Archivist
    Command line tool for recording and processing VATSIM network traffic

    (C) 2025 DIY Labs
    Licensed under GNU GPL V3

    Wiki at https://github.com/DIYLabsED/VATPlayback/wiki

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
void printHelp();
void printVersion();



int main(int argc, char *argv[]){

    setup();
    
    if(argc == 1){ // No arguments 

        cout << "No arguments provided. Use -h or --help for help.\n";
        safeExit(SIGINT);

     }

     if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
    
        printHelp();
        safeExit(SIGINT);
        return 0;
        
    }    
     
    else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0){

        printVersion();
        safeExit(SIGINT);
        return 0;
        
    }         

    safeExit(SIGINT);
    return 0; // Should never reach here, unless safeExit fails

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
    // globalConfig is populated at this point

}

void printHelp(){

    cout << "Command list:\n"
         << "-h, --help: Print help message\n"
         << "-v, --version: Print version information\n";

}

void printVersion(){

    cout << "VATPlayback Archivist version" << VP_VERSION << "\n"
         << "nlohmann/json version\n" << NLOHMANN_JSON_VERSION_MAJOR << "." << NLOHMANN_JSON_VERSION_MINOR << "." << NLOHMANN_JSON_VERSION_PATCH << "\n";

}