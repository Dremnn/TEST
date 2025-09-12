#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Forward declarations
class Station;
class Vehicle;

// Base Vehicle class
class Vehicle {
protected:
    string route;
    int capacity;
    int currentPassengers;
    bool status; // true = on-time, false = delayed
    static int nextVehicleID;
    int vehicleID;

public:
    // Constructor
    Vehicle(const string& route, int capacity) 
        : route(route), capacity(capacity), currentPassengers(0), 
          status(true), vehicleID(nextVehicleID++) {}
    
    // Virtual destructor
    virtual ~Vehicle() {}
    
    // Virtual methods for polymorphism
    virtual double calculateTravelTime(double distance) {
        // Base speed: 40 km/h for regular vehicles
        return distance / 40.0;
    }
    
    virtual string getVehicleType() const {
        return "Regular Vehicle";
    }
    
    virtual void displayInfo() const {
        cout << "Vehicle Type: " << getVehicleType() << endl;
        cout << "ID: " << vehicleID << endl;
        cout << "Route: " << route << endl;
        cout << "Capacity: " << capacity << endl;
        cout << "Current Passengers: " << currentPassengers << endl;
        cout << "Status: " << (status ? "On-time" : "Delayed") << endl;
    }
    
    // Getters
    int getVehicleID() const { return vehicleID; }
    string getRoute() const { return route; }
    int getCapacity() const { return capacity; }
    int getCurrentPassengers() const { return currentPassengers; }
    bool getStatus() const { return status; }
    
    // Methods
    bool hasAvailableSeats() const {
        return currentPassengers < capacity;
    }
    
    bool bookSeat() {
        if (hasAvailableSeats()) {
            currentPassengers++;
            return true;
        }
        return false;
    }
    
    bool cancelSeat() {
        if (currentPassengers > 0) {
            currentPassengers--;
            return true;
        }
        return false;
    }
    
    void setStatus(bool newStatus) { status = newStatus; }
    void assignToStation(Station* station);
};

// Derived ExpressBus class
class ExpressBus : public Vehicle {
private:
    double speed; // Higher speed for express buses
    int maxStops; // Fewer stops than regular buses

public:
    // Constructor
    ExpressBus(const string& route, int capacity, double speed, int maxStops)
        : Vehicle(route, capacity), speed(speed), maxStops(maxStops) {}
    
    // Override virtual methods
    double calculateTravelTime(double distance) override {
        // Express buses are 20% faster
        double baseTime = distance / speed;
        return baseTime * 0.8; // 20% reduction in travel time
    }
    
    string getVehicleType() const override {
        return "Express Bus";
    }
    
    void displayInfo() const override {
        Vehicle::displayInfo(); // Call base class method
        cout << "Speed: " << speed << " km/h" << endl;
        cout << "Max Stops: " << maxStops << endl;
        cout << "Express Service: 20% faster travel time" << endl;
    }
    
    // Additional methods specific to ExpressBus
    double getSpeed() const { return speed; }
    int getMaxStops() const { return maxStops; }
};

// Station class
class Station {
private:
    string name;
    string location;
    string type; // "Bus" or "Train"
    vector<int> scheduledVehicleIDs; // Store vehicle IDs
    static const int MAX_SCHEDULES = 10;

public:
    // Constructor
    Station(const string& name, const string& location, const string& type)
        : name(name), location(location), type(type) {}
    
    // Getters
    string getName() const { return name; }
    string getLocation() const { return location; }
    string getType() const { return type; }
    vector<int> getScheduledVehicles() const { return scheduledVehicleIDs; }
    
    // Methods
    bool addSchedule(int vehicleID) {
        if (scheduledVehicleIDs.size() < MAX_SCHEDULES) {
            scheduledVehicleIDs.push_back(vehicleID);
            cout << "Vehicle " << vehicleID << " scheduled at " << name << " station." << endl;
            return true;
        } else {
            cout << "Cannot schedule vehicle " << vehicleID << ". Maximum schedules (" 
                 << MAX_SCHEDULES << ") reached for " << name << " station." << endl;
            return false;
        }
    }
    
    bool removeSchedule(int vehicleID) {
        auto it = find(scheduledVehicleIDs.begin(), scheduledVehicleIDs.end(), vehicleID);
        if (it != scheduledVehicleIDs.end()) {
            scheduledVehicleIDs.erase(it);
            cout << "Vehicle " << vehicleID << " removed from " << name << " station schedule." << endl;
            return true;
        } else {
            cout << "Vehicle " << vehicleID << " not found in " << name << " station schedule." << endl;
            return false;
        }
    }
    
    void displayInfo() const {
        cout << "\n=== STATION INFORMATION ===" << endl;
        cout << "Name: " << name << endl;
        cout << "Location: " << location << endl;
        cout << "Type: " << type << endl;
        cout << "Scheduled Vehicles: ";
        if (scheduledVehicleIDs.empty()) {
            cout << "None" << endl;
        } else {
            for (size_t i = 0; i < scheduledVehicleIDs.size(); ++i) {
                cout << scheduledVehicleIDs[i];
                if (i < scheduledVehicleIDs.size() - 1) cout << ", ";
            }
            cout << endl;
        }
        cout << "Available Schedule Slots: " << (MAX_SCHEDULES - scheduledVehicleIDs.size()) << "/" << MAX_SCHEDULES << endl;
    }
};

// Passenger class
class Passenger {
private:
    string name;
    int passengerID;
    vector<int> bookedVehicleIDs; // Store IDs of booked vehicles
    static int nextPassengerID;

public:
    // Constructor
    Passenger(const string& name) 
        : name(name), passengerID(nextPassengerID++) {}
    
    // Getters
    string getName() const { return name; }
    int getPassengerID() const { return passengerID; }
    vector<int> getBookedVehicles() const { return bookedVehicleIDs; }
    
    // Methods
    bool bookRide(Vehicle* vehicle) {
        if (vehicle && vehicle->bookSeat()) {
            bookedVehicleIDs.push_back(vehicle->getVehicleID());
            cout << name << " successfully booked a seat on Vehicle " 
                 << vehicle->getVehicleID() << " (" << vehicle->getRoute() << ")" << endl;
            return true;
        } else {
            cout << "Booking failed for " << name << ". Vehicle may be full or unavailable." << endl;
            return false;
        }
    }
    
    bool cancelRide(Vehicle* vehicle) {
        if (vehicle) {
            auto it = find(bookedVehicleIDs.begin(), bookedVehicleIDs.end(), vehicle->getVehicleID());
            if (it != bookedVehicleIDs.end()) {
                bookedVehicleIDs.erase(it);
                vehicle->cancelSeat();
                cout << name << " cancelled booking for Vehicle " << vehicle->getVehicleID() << endl;
                return true;
            }
        }
        cout << "Cancellation failed for " << name << ". No booking found for this vehicle." << endl;
        return false;
    }
    
    void displayInfo() const {
        cout << "\n=== PASSENGER INFORMATION ===" << endl;
        cout << "Name: " << name << endl;
        cout << "ID: " << passengerID << endl;
        cout << "Booked Vehicles: ";
        if (bookedVehicleIDs.empty()) {
            cout << "None" << endl;
        } else {
            for (size_t i = 0; i < bookedVehicleIDs.size(); ++i) {
                cout << bookedVehicleIDs[i];
                if (i < bookedVehicleIDs.size() - 1) cout << ", ";
            }
            cout << endl;
        }
    }
};

// Transportation Management System class
class TransportationSystem {
private:
    vector<Vehicle*> vehicles;
    vector<Station*> stations;
    vector<Passenger*> passengers;

public:
    // Destructor
    ~TransportationSystem() {
        for (Vehicle* vehicle : vehicles) delete vehicle;
        for (Station* station : stations) delete station;
        for (Passenger* passenger : passengers) delete passenger;
    }
    
    // Add vehicle
    void addVehicle(Vehicle* vehicle) {
        vehicles.push_back(vehicle);
        cout << "Added " << vehicle->getVehicleType() << " (ID: " 
             << vehicle->getVehicleID() << ") to the system." << endl;
    }
    
    // Add station
    void addStation(Station* station) {
        stations.push_back(station);
        cout << "Added " << station->getName() << " station to the system." << endl;
    }
    
    // Add passenger
    void addPassenger(Passenger* passenger) {
        passengers.push_back(passenger);
        cout << "Added passenger " << passenger->getName() 
             << " (ID: " << passenger->getPassengerID() << ") to the system." << endl;
    }
    
    // Find vehicle by ID
    Vehicle* findVehicle(int vehicleID) {
        for (Vehicle* vehicle : vehicles) {
            if (vehicle->getVehicleID() == vehicleID) {
                return vehicle;
            }
        }
        return nullptr;
    }
    
    // Find station by name
    Station* findStation(const string& name) {
        for (Station* station : stations) {
            if (station->getName() == name) {
                return station;
            }
        }
        return nullptr;
    }
    
    // Find passenger by ID
    Passenger* findPassenger(int passengerID) {
        for (Passenger* passenger : passengers) {
            if (passenger->getPassengerID() == passengerID) {
                return passenger;
            }
        }
        return nullptr;
    }
    
    // Display all vehicles
    void displayAllVehicles() const {
        cout << "\n=== ALL VEHICLES ===" << endl;
        for (size_t i = 0; i < vehicles.size(); ++i) {
            cout << "\n--- Vehicle " << (i + 1) << " ---" << endl;
            vehicles[i]->displayInfo();
        }
    }
    
    // Display all stations
    void displayAllStations() const {
        cout << "\n=== ALL STATIONS ===" << endl;
        for (Station* station : stations) {
            station->displayInfo();
        }
    }
    
    // Display all passengers
    void displayAllPassengers() const {
        cout << "\n=== ALL PASSENGERS ===" << endl;
        for (Passenger* passenger : passengers) {
            passenger->displayInfo();
        }
    }
};

// Initialize static members
int Vehicle::nextVehicleID = 1;
int Passenger::nextPassengerID = 1;

// Implementation of Vehicle::assignToStation
void Vehicle::assignToStation(Station* station) {
    if (station) {
        station->addSchedule(vehicleID);
    }
}

// Initialize sample data
void initializeSampleData(TransportationSystem& ts) {
    // Add sample stations
    ts.addStation(new Station("Central Station", "City Center", "Bus"));
    ts.addStation(new Station("Airport Terminal", "Airport District", "Bus"));
    ts.addStation(new Station("University Campus", "Education District", "Bus"));
    
    // Add sample vehicles
    ts.addVehicle(new Vehicle("Route 101", 45));
    ts.addVehicle(new ExpressBus("Express A1", 35, 70, 5));
    ts.addVehicle(new Vehicle("Route 202", 50));
    ts.addVehicle(new ExpressBus("Express B2", 30, 65, 4));
    
    // Add sample passengers
    ts.addPassenger(new Passenger("Nguyen Van A"));
    ts.addPassenger(new Passenger("Tran Thi B"));
    ts.addPassenger(new Passenger("Le Van C"));
    
    cout << "Sample data initialized successfully!" << endl;
}

// Menu-driven interface
void runMenu(TransportationSystem& ts) {
    int choice;

    do {
        cout << "\n=== PUBLIC TRANSPORTATION MANAGEMENT SYSTEM ===" << endl;
        cout << "1.  Add Vehicle (Regular or Express Bus)" << endl;
        cout << "2.  Add Station" << endl;
        cout << "3.  Add Passenger" << endl;
        cout << "4.  Schedule Vehicle at Station" << endl;
        cout << "5.  Remove Vehicle from Station" << endl;
        cout << "6.  Book Passenger Ride" << endl;
        cout << "7.  Cancel Passenger Ride" << endl;
        cout << "8.  Calculate Travel Time" << endl;
        cout << "9.  Display All Vehicles" << endl;
        cout << "10. Display All Stations" << endl;
        cout << "11. Display All Passengers" << endl;
        cout << "12. Find Vehicle by ID" << endl;
        cout << "13. Find Station by Name" << endl;
        cout << "14. Find Passenger by ID" << endl;
        cout << "0.  Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string route;
                int capacity;
                cout << "Enter route name: ";
                cin.ignore();
                getline(cin, route);
                cout << "Enter capacity: ";
                cin >> capacity;

                int typeChoice;
                cout << "Select vehicle type:\n";
                cout << "1. Regular Vehicle\n";
                cout << "2. Express Bus\n";
                cout << "Enter choice: ";
                cin >> typeChoice;

                if (typeChoice == 1) {
                    ts.addVehicle(new Vehicle(route, capacity));
                } else if (typeChoice == 2) {
                    double speed;
                    int maxStops;
                    cout << "Enter speed (km/h): ";
                    cin >> speed;
                    cout << "Enter max stops: ";
                    cin >> maxStops;
                    ts.addVehicle(new ExpressBus(route, capacity, speed, maxStops));
                } else {
                    cout << "Invalid vehicle type selected." << endl;
                }
                break;
            }
            case 2: {
                string name, location, type;
                cout << "Enter station name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter location: ";
                getline(cin, location);
                cout << "Enter type (Bus/Train): ";
                getline(cin, type);
                ts.addStation(new Station(name, location, type));
                break;
            }
            case 3: {
                string name;
                cout << "Enter passenger name: ";
                cin.ignore();
                getline(cin, name);
                ts.addPassenger(new Passenger(name));
                break;
            }
            case 4: {
                int vehicleID;
                string stationName;
                cout << "Enter vehicle ID: ";
                cin >> vehicleID;
                cout << "Enter station name: ";
                cin.ignore();
                getline(cin, stationName);

                Vehicle* vehicle = ts.findVehicle(vehicleID);
                Station* station = ts.findStation(stationName);

                if (vehicle && station) {
                    vehicle->assignToStation(station);
                } else {
                    cout << "Vehicle or station not found." << endl;
                }
                break;
            }
            case 5: {
                int vehicleID;
                string stationName;
                cout << "Enter vehicle ID: ";
                cin >> vehicleID;
                cout << "Enter station name: ";
                cin.ignore();
                getline(cin, stationName);

                Station* station = ts.findStation(stationName);
                if (station) {
                    station->removeSchedule(vehicleID);
                } else {
                    cout << "Station not found." << endl;
                }
                break;
            }
            case 6: {
                int passengerID, vehicleID;
                cout << "Enter passenger ID: ";
                cin >> passengerID;
                cout << "Enter vehicle ID: ";
                cin >> vehicleID;

                Passenger* passenger = ts.findPassenger(passengerID);
                Vehicle* vehicle = ts.findVehicle(vehicleID);

                if (passenger && vehicle) {
                    passenger->bookRide(vehicle);
                } else {
                    cout << "Passenger or vehicle not found." << endl;
                }
                break;
            }
            case 7: {
                int passengerID, vehicleID;
                cout << "Enter passenger ID: ";
                cin >> passengerID;
                cout << "Enter vehicle ID: ";
                cin >> vehicleID;

                Passenger* passenger = ts.findPassenger(passengerID);
                Vehicle* vehicle = ts.findVehicle(vehicleID);

                if (passenger && vehicle) {
                    passenger->cancelRide(vehicle);
                } else {
                    cout << "Passenger or vehicle not found." << endl;
                }
                break;
            }
            case 8: {
                int vehicleID;
                double distance;
                cout << "Enter vehicle ID: ";
                cin >> vehicleID;
                cout << "Enter distance (km): ";
                cin >> distance;

                Vehicle* vehicle = ts.findVehicle(vehicleID);
                if (vehicle) {
                    double time = vehicle->calculateTravelTime(distance);
                    cout << "Travel time for " << distance << "km: " << time << " hours" << endl;
                    cout << "Vehicle type: " << vehicle->getVehicleType() << endl;
                } else {
                    cout << "Vehicle not found." << endl;
                }
                break;
            }
            case 9: {
                ts.displayAllVehicles();
                break;
            }
            case 10: {
                ts.displayAllStations();
                break;
            }
            case 11: {
                ts.displayAllPassengers();
                break;
            }
            case 12: {
                int vehicleID;
                cout << "Enter vehicle ID: ";
                cin >> vehicleID;
                Vehicle* vehicle = ts.findVehicle(vehicleID);
                if (vehicle) {
                    cout << "\n=== VEHICLE FOUND ===" << endl;
                    vehicle->displayInfo();
                } else {
                    cout << "Vehicle not found." << endl;
                }
                break;
            }
            case 13: {
                string stationName;
                cout << "Enter station name: ";
                cin.ignore();
                getline(cin, stationName);
                Station* station = ts.findStation(stationName);
                if (station) {
                    station->displayInfo();
                } else {
                    cout << "Station not found." << endl;
                }
                break;
            }
            case 14: {
                int passengerID;
                cout << "Enter passenger ID: ";
                cin >> passengerID;
                Passenger* passenger = ts.findPassenger(passengerID);
                if (passenger) {
                    passenger->displayInfo();
                } else {
                    cout << "Passenger not found." << endl;
                }
                break;
            }
            case 0: {
                cout << "Exiting Transportation Management System. Goodbye!" << endl;
                break;
            }
            default: {
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }

    } while (choice != 0);
}

int main() {
    cout << "=== PUBLIC TRANSPORTATION MANAGEMENT SYSTEM ===" << endl;
    cout << "Initializing system..." << endl;
    
    TransportationSystem system;
    
    // Initialize with sample data
    initializeSampleData(system);
    
    // Run interactive menu
    runMenu(system);
    
    return 0;
}