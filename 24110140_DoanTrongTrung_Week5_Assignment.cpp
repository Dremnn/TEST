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
int Passenger::nextPassengerID = 1001;

// Implementation of Vehicle::assignToStation
void Vehicle::assignToStation(Station* station) {
    if (station) {
        station->addSchedule(vehicleID);
    }
}

// Test and demonstration functions
void demonstratePolymorphism(TransportationSystem& ts) {
    cout << "\n=== POLYMORPHISM DEMONSTRATION ===" << endl;
    
    Vehicle* regularBus = new Vehicle("Route A", 40);
    Vehicle* expressBus = new ExpressBus("Route B", 30, 60, 5);
    
    ts.addVehicle(regularBus);
    ts.addVehicle(expressBus);
    
    double distance = 120.0; // 120 km
    
    cout << "\nTravel time calculation for 120km journey:" << endl;
    cout << "Regular Vehicle: " << regularBus->calculateTravelTime(distance) << " hours" << endl;
    cout << "Express Bus: " << expressBus->calculateTravelTime(distance) << " hours" << endl;
    
    cout << "\nPolymorphic method calls through base class pointers:" << endl;
    regularBus->displayInfo();
    cout << "\n";
    expressBus->displayInfo();
}

void demonstrateBookingSystem(TransportationSystem& ts) {
    cout << "\n=== BOOKING SYSTEM DEMONSTRATION ===" << endl;
    
    // Create a small capacity vehicle for testing
    Vehicle* smallBus = new Vehicle("City Center Route", 2);
    ts.addVehicle(smallBus);
    
    // Create passengers
    Passenger* passenger1 = new Passenger("John Doe");
    Passenger* passenger2 = new Passenger("Jane Smith");
    Passenger* passenger3 = new Passenger("Bob Wilson");
    
    ts.addPassenger(passenger1);
    ts.addPassenger(passenger2);
    ts.addPassenger(passenger3);
    
    cout << "\nTesting booking with capacity limits:" << endl;
    passenger1->bookRide(smallBus); // Should succeed
    passenger2->bookRide(smallBus); // Should succeed
    passenger3->bookRide(smallBus); // Should fail (capacity full)
    
    cout << "\nCurrent vehicle status:" << endl;
    smallBus->displayInfo();
    
    cout << "\nTesting cancellation:" << endl;
    passenger1->cancelRide(smallBus); // Should succeed
    passenger3->bookRide(smallBus);   // Should now succeed
}

void demonstrateStationScheduling(TransportationSystem& ts) {
    cout << "\n=== STATION SCHEDULING DEMONSTRATION ===" << endl;
    
    Station* centralStation = new Station("Central Station", "City Center", "Bus");
    ts.addStation(centralStation);
    
    // Create vehicles and schedule them
    Vehicle* bus1 = new Vehicle("Route X", 50);
    Vehicle* bus2 = new ExpressBus("Route Y", 40, 70, 4);
    
    ts.addVehicle(bus1);
    ts.addVehicle(bus2);
    
    cout << "\nScheduling vehicles at station:" << endl;
    bus1->assignToStation(centralStation);
    bus2->assignToStation(centralStation);
    
    centralStation->displayInfo();
}

int main() {
    cout << "=== PUBLIC TRANSPORTATION SYSTEM DEMO ===" << endl;
    
    TransportationSystem system;
    
    // Test 1: Demonstrate inheritance and polymorphism
    demonstratePolymorphism(system);
    
    // Test 2: Demonstrate booking system with capacity limits
    demonstrateBookingSystem(system);
    
    // Test 3: Demonstrate station scheduling
    demonstrateStationScheduling(system);
    
    // Display system overview
    cout << "\n=== FINAL SYSTEM STATE ===" << endl;
    system.displayAllVehicles();
    system.displayAllStations();
    system.displayAllPassengers();
    
    return 0;
}