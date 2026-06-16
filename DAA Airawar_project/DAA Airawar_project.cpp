

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include<unordered_map>
#include <map>
#include<climits>
#include <fstream> // For file handling
#include <sstream> // For CSV parsing
#include <algorithm> // For sorting and filtering

using namespace std;

// ====================== Pollution Data Class ======================
class PollutionData {
public:
    string cityId;
    string cityName;
    int population;
    string monitoringStationName;
    double distanceFromState; // in km
    string pollutantName;
    int dangerousLevel;
    string alertMessage;

    PollutionData() {}

    PollutionData(string id, string name, int pop, string station, double dist, string pollutant, int dangerLevel, string alert)
        : cityId(id), cityName(name), population(pop), monitoringStationName(station), distanceFromState(dist),
          pollutantName(pollutant), dangerousLevel(dangerLevel), alertMessage(alert) {}

    string toCSV() const {
        stringstream ss;
        ss << cityId << "," << cityName << "," << population << "," << monitoringStationName << "," << distanceFromState
           << "," << pollutantName << "," << dangerousLevel << "," << alertMessage;
        return ss.str();
    }

    static PollutionData fromCSV(const string &line) {
        stringstream ss(line);
        string id, name, station, pollutant, alert;
        int pop, dangerLevel;
        double dist;

        getline(ss, id, ',');
        getline(ss, name, ',');
        ss >> pop; ss.ignore();
        getline(ss, station, ',');
        ss >> dist; ss.ignore();
        getline(ss, pollutant, ',');
        ss >> dangerLevel; ss.ignore();
        getline(ss, alert);

        return PollutionData(id, name, pop, station, dist, pollutant, dangerLevel, alert);
    }
};

// ====================== State Pollution Monitoring System ======================
class StatePollutionMonitor {
private:
    map<string, vector<PollutionData>> stateCityData;
    const string PASSWORD = "DAA";

    // Function to read data from file and add cities to the stateCityData
    void readFromFileAndAddCities(const string &filename) {
        ifstream file(filename); // Open the file
        if (!file) {
            cout << "Failed to open file: " << filename << "\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string state, cityId, cityName, stationName, pollutantName, alertMessage;
            int population, dangerousLevel;
            double distance;

            // Reading the data from the file (assuming CSV format)
            getline(ss, state, ',');
            getline(ss, cityId, ',');
            getline(ss, cityName, ',');
            ss >> population; ss.ignore();
            getline(ss, stationName, ',');
            ss >> distance; ss.ignore();
            getline(ss, pollutantName, ',');
            ss >> dangerousLevel; ss.ignore();
            getline(ss, alertMessage);

            // Create a PollutionData object
            PollutionData newCity(cityId, cityName, population, stationName, distance, pollutantName, dangerousLevel, alertMessage);

            // Add the city to the stateCityData map
            stateCityData[state].push_back(newCity);
        }

        file.close(); // Close the file
        cout << "Cities data loaded successfully from " << filename << "\n";
    }

public:
    // ====================== Authentication ======================
    bool authenticate() {
        string inputPassword;
        cout << "Enter the password to access the system: ";
        cin >> inputPassword;
        return inputPassword == PASSWORD;
    }

    // ====================== Data Management ======================
    void addCityToState() {
        string state, cityId, cityName, stationName, pollutantName, alertMessage;
        int population, dangerousLevel;
        double distance;

        cout << "Enter State Name: ";
        cin.ignore();
        getline(cin, state);

        cout << "Enter City ID: ";
        cin >> cityId;
        cout << "Enter City Name: ";
        cin.ignore();
        getline(cin, cityName);
        cout << "Enter Population: ";
        cin >> population;
        cout << "Enter Monitoring Station Name: ";
        cin.ignore();
        getline(cin, stationName);
        cout << "Enter Distance from State HQ (km): ";
        cin >> distance;
        cout << "Enter Pollutant Name: ";
        cin.ignore();
        getline(cin, pollutantName);
        cout << "Enter Dangerous Level: ";
        cin >> dangerousLevel;
        cout << "Enter Alert Message: ";
        cin.ignore();
        getline(cin, alertMessage);

        PollutionData newCity(cityId, cityName, population, stationName, distance, pollutantName, dangerousLevel, alertMessage);
        stateCityData[state].push_back(newCity);
        cout << "City added successfully to " << state << ".\n";
    }
// Function to find the city corresponding to the maximum pollution level
string findCityWithMaxPollution( const string &state) {
    int polluted=INT_MIN;
    for (const auto &city : stateCityData[state]){
        polluted=max(polluted,city.dangerousLevel);
    }
   for (const auto &city : stateCityData[state]) {
              if(city.dangerousLevel==polluted) return city.cityName;
        }
}
    void displayCityData(const string &state) {
        if (stateCityData.find(state) == stateCityData.end()) {
            cout << "No data available for the state: " << state << "\n";
            return;
        }

        cout << "\n================== City Data for " << state << " ==================\n";

        cout << "+---------+------------+------------+----------------------+------------+-------------------+------------------+-------------------+\n";
        cout << "| City ID | City Name  | Population | Monitoring Station   | Distance   | Pollutant Name    | Dangerous Level  | Alert Message     |\n";
        cout << "+---------+------------+------------+----------------------+------------+-------------------+------------------+-------------------+\n";

        for (const auto &city : stateCityData[state]) {
                cout << "| " << setw(7) << city.cityId << " | "
                 << setw(10) << city.cityName << " | "
                 << setw(10) << city.population << " | "
                 << setw(20) << city.monitoringStationName << " | "
                 << setw(10) << fixed << setprecision(2) << city.distanceFromState << " km | "
                 << setw(15) << city.pollutantName << " | "
                 << setw(16) << city.dangerousLevel << " | "
                 << setw(17) << city.alertMessage << " |\n";
        }

        cout << "+---------+------------+------------+----------------------+------------+-------------------+------------------+-------------------+\n";

    }
    string findMinPollutionCity(const string &state){
          int polluted=INT_MAX;
    for (const auto &city : stateCityData[state]){
        polluted=min(polluted,city.dangerousLevel);
    }
   for (const auto &city : stateCityData[state]) {
              if(city.dangerousLevel==polluted) return city.cityName;
        }
    }
   void viewStateData(unordered_map<int,string>mp){
    cout<<"1.View City Data for State"<<endl;
    cout<<"2.Return to main menu"<<endl;
    int n;
    cin>>n;
    switch(n){
        case 1: {
        cout<<"Enter State Number to view city data"<<endl;
        int x;
        cin>>x;
        cout<<"The selected State is: "<<mp[x]<<endl;
        int y;
        cout<<"1.View Most Polluted City"<<endl;
        cout<<"2.View Least Polluted City"<<endl;
        cout<<"3.View all city data"<<endl;
        cout<<"4. Return back"<<endl;
        cin>>y;
        switch (y)
        {
        case 1: cout<<" The most polluted city is  "<<findCityWithMaxPollution(mp[x])<<endl;
        break;
        case 2:
        cout<<" The least Polluted city is "<<findMinPollutionCity(mp[x])<<endl;
        break;
        case 3: if(mp.find(x)!= mp.end())  displayCityData(mp[x]);
        break;
        case 4: return;
            break;
        }

        }
        break;
        case 2 : return;
        break;
    }
   }
   void listStates() {
    if (stateCityData.empty()) {
        cout << "No states available.\n";
        return;
    }
    unordered_map<int,string>mp;
    cout << "\nAvailable States:\n";
    int index = 0; // Initialize an index counter
    for (const auto &entry : stateCityData) {
        cout << index << ". " << entry.first << "\n";
        mp[index]=entry.first;
        index++; // Increment the index for each state
    }
    //cout<<mp.size();
    viewStateData(mp);
}


    // ====================== Persistence ======================
    void saveData(const string &filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Failed to save data to " << filename << "\n";
            return;
        }

        for (const auto &entry : stateCityData) {
            for (const auto &city : entry.second) {
                file << entry.first << "," << city.toCSV() << "\n";
            }
        }

        file.close();
        cout << "Data saved successfully to " << filename << "\n";
    }

    void loadData(const string &filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Failed to load data from " << filename << "\n";
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string state;
            getline(ss, state, ',');
            PollutionData city = PollutionData::fromCSV(ss.str());
            stateCityData[state].push_back(city);
        }

        file.close();
        cout << "Data loaded successfully from " << filename << "\n";
    }

    // ====================== Utility ======================
    void mainMenu() {
        while (true) {
            cout << "\n================== State Pollution Monitoring System ==================\n";
            cout << "1. List States\n";
            cout << "2. Add City Data\n";
            cout << "3. View City Data\n";
            cout << "4. Save Data\n";
            cout << "5. Load Data\n";
            cout << "6. Load Cities from File\n";
            cout << "7. Exit\n";

            int choice;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    listStates();
                    break;
                case 2:
                    addCityToState();
                    break;
                case 3: {
                    string state;
                    cout << "Enter State Name: ";
                    cin.ignore();
                    getline(cin, state);
                    displayCityData(state);
                    break;
                }
                case 4: {
                    string filename = "D:\\DAA Airawar Project\\pollution.txt";
                    cout << "Enter filename to save data: ";
                    cin >> filename;
                    saveData(filename);
                    break;
                }
                case 5: {
                    string filename = "D:\\DAA Airawar Project\\pollution.txt";
                    cout << "Enter filename to load data: ";
                    cin >> filename;
                    loadData(filename);
                    break;
                }
                case 6: {
                       string filename = "D:\\DAA Airawar Project\\pollution.txt";
                       //D:\\DAA Airawar Project\\pollution.txt
                       cout << "Enter full file path to load cities from (e.g., C:\\path\\to\\file.csv): ";
                       cin.ignore();
                       getline(cin, filename); // Use getline for full path input
                       readFromFileAndAddCities(filename);
                         break;
                     }

                case 7:
                    cout << "Exiting the system.\n";
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

// ====================== Main Function ======================
int main() {
    StatePollutionMonitor monitor;

    if (!monitor.authenticate()) {
        cout << "Access Denied.\n";
        return 0;
    }

    monitor.mainMenu();
    return 0;
}
