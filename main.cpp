#include <iostream>
#include "Utils.h"
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <string>
#include <cstdint>

using namespace std;

class Store {
    public:

        // Store attributes.
        string name;
        int squareFeet;
        string size;
        string address;
        string neighborhood;
        double latitude;
        double longitude;

        string neighborhoodName;
        int largeSize;
        int smallSize;

    // Constructor to initialize a Store object with given parameters.
    Store(string name, int squareFeet, string size, string address, string neighborhood, double latitude, double longitude) {
        this->name = name;
        this->squareFeet = squareFeet;
        this->size = size;
        this->address = address;
        this->neighborhood = neighborhood;
        this->latitude = latitude;
        this->longitude = longitude;
    }

    Store(string neighborhoodName, int largeSize, int smallSize) {
        this->neighborhoodName = neighborhoodName;
        this->largeSize = largeSize;
        this->smallSize = smallSize;
    }

    bool operator<(const Store& other) const {
        if ((largeSize + smallSize) == (other.largeSize + other.smallSize)) {
            return neighborhoodName < other.neighborhoodName;
        }
        return (largeSize + smallSize) > (other.largeSize + other.smallSize);  
    }

};

class StoreList {
    public:
        // Vector to store list of Store objects.
        vector <Store> StoresLists;

    // Reads store information from a file and populates the StoresList vector.
    void loadStoresFromFile(const string& filename) {
        StoresLists.clear();
        ifstream fileIn(filename);
        while (!fileIn.eof()) {
            string name, sqFeet, size, address, neighborhoodName, latitude, longitude;

            // Parses each line of the file into Store attributes.
            getline(fileIn, name, ',');
            getline(fileIn, sqFeet, ',');
            getline(fileIn, size, ',');
            getline(fileIn, address, ',');
            getline(fileIn, neighborhoodName, ',');
            getline(fileIn, latitude, ',');
            getline(fileIn, longitude);

            int sqFeetSize = stoi(sqFeet);
            double latitudeSize = stod(latitude);
            double longitudeSize = stod(longitude);

            Store store(name, sqFeetSize, size, address, neighborhoodName, latitudeSize, longitudeSize);
            
            
            // Adds the new Store object to StoresList.
            StoresLists.push_back(store);
        }
    }

    // Displays general information about the grocery stores.
    void displayGeneralGroceryStoreInformation() {

        // Calculates and displays the number of stores, number of large stores, and average size of large stores.
        cout << "Number of grocery stores: " << StoresLists.size() << endl;
        int count = 0;
        int total = 0;
        int noSqFeet = 0;
        for (Store& currStore : StoresLists) {
            if (currStore.size == "Large") {
                count++;
                if (currStore.squareFeet != 0) {
                    total += currStore.squareFeet;
                }    
                else {
                    noSqFeet++;
                }
                
            }
        }
        double average = static_cast <double> (total) / (count - noSqFeet);

        cout << "Number of large grocery stores: " << count << endl;            
        cout << "Average size of large grocery stores: " << fixed << setprecision(1) << average << endl << endl;
    }

    // Displays a chart of neighborhoods with the counts of large and small stores.
    void displayNeighborhoodCharts() {
        vector <Store> StoresInfosLists;

        // Loop to populate StoresInfosList with unique neighborhoods and their store counts.
        for (Store& currStore : StoresLists) {
            string neigh = currStore.neighborhood;
            Store neighObj(neigh, 0, 0);
            bool duplicate = false;
            for (Store moreStores : StoresInfosLists) {
                if (moreStores.neighborhoodName == neigh) {
                    duplicate = true;
                }
            }
            if (!duplicate) {
                StoresInfosLists.push_back(neighObj);
            }
        }

        // For each neighborhood, loop through all stores to count large and small stores
        for (Store& moreNeighborhoods : StoresInfosLists) {
            for (const Store& currStore : StoresLists) {
                if (currStore.neighborhood == moreNeighborhoods.neighborhoodName) {
                    if (currStore.size == "Large") {
                        moreNeighborhoods.largeSize++;
                    }
                    else if (currStore.size == "Small") {
                        moreNeighborhoods.smallSize++;
                    }
                }
            }
        }

        // Sorts neighborhoods by store count and name.
        sort(StoresInfosLists.begin(), StoresInfosLists.end());

        // Displays the neighborhood chart.
        for (const Store& info : StoresInfosLists) {
            cout << left << setw(20) << info.neighborhoodName;
            cout << string(info.largeSize, 'L') << string(info.smallSize, 'S') << endl;
        }
        
    }

    // Finds and displays the closest store and the closest large store to the given latitude and longitude.
    void findClosestStores(double latitude, double longitude) {
        double closestDistance = INT64_MAX;
        string closestStoreName;
        string closestStoreAddress;

        double largeClosestDistance = INT64_MAX;
        string largeClosestStoreName;
        string largeClosestStoreAddress;
        
        // Loops through each store to find the closest and the closest large store.
        for (Store& currStore : StoresLists) {
            double latitudeLocation = currStore.latitude;
            double longitudeLocation = currStore.longitude; 
            double distances = distance(latitude, longitude, latitudeLocation, longitudeLocation);

            // Updates information if a closer store is found.
            if (distances < closestDistance) {
                closestDistance = distances;
                closestStoreName = currStore.name;
                closestStoreAddress = currStore.address;
            }

            // Specifically checks and updates for the closest large store.
            if ((currStore.size == "Large") && (distances < largeClosestDistance)) {
                largeClosestStoreName = currStore.name;
                largeClosestDistance = distances;
                largeClosestStoreAddress = currStore.address;
            }


        }

        // Displays the closest store and, if applicable, the closest large store.
        cout << "The closest store is " << fixed << setprecision(1) << closestDistance << " miles away." << endl;
        cout << left << setw(2) << closestStoreName << " is at " << closestStoreAddress << endl;
        if (closestStoreName != largeClosestStoreName) {
            cout << "The closest large store is " << fixed << setprecision(1) << largeClosestDistance << " miles away." << endl;
            cout << left << setw(2) << largeClosestStoreName << " is at " << largeClosestStoreAddress << endl;
        }
        else {
            cout << "That is a large store." << endl;
        }

        // Indicates if the location could be considered a food desert based on the distances to the closest stores.
        if ((closestDistance > 0.5) && (largeClosestDistance > 1)) {
                cout << "This location is a food desert." << endl;
        }

    }

    // Searches for stores by name and displays relevant information.
    void searchForStore(string& searchTerm){
        bool foundStore = false;
        for (Store& currStore : StoresLists) {

            // Displays store information if the search term is found within the store name.
            if (currStore.name.find(searchTerm) != string :: npos) {
                cout << "Store: " << currStore.name << endl;
                cout << "Address: " << currStore.address << endl;
                cout << "Community: " << currStore.neighborhood << endl;
                cout << "Size: " << currStore.size << endl;
                cout << "Square Feet: " << currStore.squareFeet << endl;
                cout << endl;
                foundStore = true;
            }
        }
        if (!foundStore) {
            cout << "No stores found." << endl;
             
        }

    }
};


int main() {
    StoreList storeLists;

    int choice; 

    // Prompts for filename to load store data from.
    cout << "Enter filename: " << endl;
    string filename;
    cin >> filename;

    // Main menu loop to perform different actions based on user choice.
    while (choice != 5) {  
        cout << "Select a menu option:" << endl;
        cout << "   1. Display general grocery stores information" << endl;
        cout << "   2. Display neighborhood charts" << endl;
        cout << "   3. Find closest stores" << endl;
        cout << "   4. Search for store by name" << endl;
        cout << "   5. Exit" << endl;
        cout << "Your choice: " << endl;

        cin >> choice;
        
        switch (choice) {
            case 1: {
                storeLists.loadStoresFromFile(filename);
                storeLists.displayGeneralGroceryStoreInformation();
                break;
            }
            case 2: {
                storeLists.loadStoresFromFile(filename);
                storeLists.displayNeighborhoodCharts();
                break;
            }
            case 3: {
                cout << "Enter latitude: " << endl;
                double latitude;
                cin >> latitude;

                cout << "Enter longitude: "<< endl;
                double longitude;
                cin >> longitude;

                storeLists.loadStoresFromFile(filename);
                storeLists.findClosestStores(latitude, longitude);
                break;
            }
            case 4: {
                cout << "Enter store to search for: " << endl;
                string storeName;

                cin.ignore();
                getline(cin, storeName);

                // Converts search term to uppercase for case-insensitive search.
                for (char& upper : storeName) {
                    upper = toupper(upper);
                }
                storeLists.loadStoresFromFile(filename);
                storeLists.searchForStore(storeName);

                break;
            }
            case 5: {
                break;
            }
            default: {
                cout << "Invalid Entry";
                break;
            }
        }
    }
}
