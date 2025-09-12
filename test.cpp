#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <iomanip>
using namespace std;

// Forward declarations
class Station;
class Vehicle;
class Route;

// Class để quản lý thời gian
class Time {
private:
    int hour;
    int minute;

public:
    Time(int h = 0, int m = 0) : hour(h), minute(m) {
        normalize();
    }
    
    void normalize() {
        if (minute >= 60) {
            hour += minute / 60;
            minute = minute % 60;
        }
        if (hour >= 24) {
            hour = hour % 24;
        }
    }
    
    string toString() const {
        return (hour < 10 ? "0" : "") + to_string(hour) + ":" + 
               (minute < 10 ? "0" : "") + to_string(minute);
    }
    
    int getHour() const { return hour; }
    int getMinute() const { return minute; }
    
    bool operator<(const Time& other) const {
        return (hour < other.hour) || (hour == other.hour && minute < other.minute);
    }
    
    bool operator==(const Time& other) const {
        return hour == other.hour && minute == other.minute;
    }
};

// Class để quản lý Route (tuyến đường)
class Route {
private:
    int routeID;
    int fromStationID;
    int toStationID;
    Time departureTime;
    Time arrivalTime;
    static int nextRouteID;

public:
    Route(int from, int to, Time dept, Time arr) 
        : routeID(nextRouteID++), fromStationID(from), toStationID(to), 
          departureTime(dept), arrivalTime(arr) {}
    
    // Getters
    int getRouteID() const { return routeID; }
    int getFromStationID() const { return fromStationID; }
    int getToStationID() const { return toStationID; }
    Time getDepartureTime() const { return departureTime; }
    Time getArrivalTime() const { return arrivalTime; }
    
    void displayInfo() const {
        cout << "Route ID: " << routeID 
             << ", From Station " << fromStationID 
             << " to Station " << toStationID
             << ", Departure: " << departureTime.toString() 
             << ", Arrival: " << arrivalTime.toString() << endl;
    }
};

// Enhanced Vehicle class với route management
class Vehicle {
protected:
    string vehicleName;
    int capacity;
    int currentPassengers;
    bool status; // true = on-time, false = delayed
    static int nextVehicleID;
    int vehicleID;
    vector<Route> assignedRoutes; // Các tuyến đường được gán cho xe này

public:
    // Constructor
    Vehicle(const string& name, int cap) 
        : vehicleName(name), capacity(cap), currentPassengers(0), 
          status(true), vehicleID(nextVehicleID++) {}
    
    // Virtual destructor
    virtual ~Vehicle() {}
    
    // Virtual methods for polymorphism
    virtual double calculateTravelTime(double distance) {
        return distance / 40.0; // Base speed: 40 km/h
    }
    
    virtual string getVehicleType() const {
        return "Regular Vehicle";
    }
    
    virtual void displayInfo() const {
        cout << "\n=== VEHICLE INFORMATION ===" << endl;
        cout << "Vehicle Type: " << getVehicleType() << endl;
        cout << "ID: " << vehicleID << endl;
        cout << "Name: " << vehicleName << endl;
        cout << "Capacity: " << capacity << endl;
        cout << "Current Passengers: " << currentPassengers << endl;
        cout << "Status: " << (status ? "On-time" : "Delayed") << endl;
        cout << "Assigned Routes: " << endl;
        if (assignedRoutes.empty()) {
            cout << "  No routes assigned" << endl;
        } else {
            for (const auto& route : assignedRoutes) {
                cout << "  ";
                route.displayInfo();
            }
        }
    }
    
    // Getters
    int getVehicleID() const { return vehicleID; }
    string getVehicleName() const { return vehicleName; }
    int getCapacity() const { return capacity; }
    int getCurrentPassengers() const { return currentPassengers; }
    bool getStatus() const { return status; }
    vector<Route> getAssignedRoutes() const { return assignedRoutes; }
    
    // Route management methods
    void addRoute(const Route& route) {
        assignedRoutes.push_back(route);
        cout << "Route added to Vehicle " << vehicleID << ": ";
        route.displayInfo();
    }
    
    bool hasRoute(int routeID) const {
        for (const auto& route : assignedRoutes) {
            if (route.getRouteID() == routeID) {
                return true;
            }
        }
        return false;
    }
    
    Route* findRoute(int routeID) {
        for (auto& route : assignedRoutes) {
            if (route.getRouteID() == routeID) {
                return &route;
            }
        }
        return nullptr;
    }
    
    // Seat management
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
};

// Enhanced ExpressBus class
class ExpressBus : public Vehicle {
private:
    double speed;
    int maxStops;

public:
    ExpressBus(const string& name, int capacity, double speed, int maxStops)
        : Vehicle(name, capacity), speed(speed), maxStops(maxStops) {}
    
    double calculateTravelTime(double distance) override {
        double baseTime = distance / speed;
        return baseTime * 0.8; // 20% faster
    }
    
    string getVehicleType() const override {
        return "Express Bus";
    }
    
    void displayInfo() const override {
        Vehicle::displayInfo();
        cout << "Speed: " << speed << " km/h" << endl;
        cout << "Max Stops: " << maxStops << endl;
        cout << "Express Service: 20% faster travel time" << endl;
    }
    
    double getSpeed() const { return speed; }
    int getMaxStops() const { return maxStops; }
};

// Enhanced Station class với lịch trình chi tiết
class Station {
private:
    string name;
    string location;
    string type;
    int stationID;
    static int nextStationID;
    
    // Lịch trình đến và đi theo vehicle và route
    map<int, vector<int>> arrivals;  // vehicleID -> list of routeIDs arriving
    map<int, vector<int>> departures;  // vehicleID -> list of routeIDs departing

public:
    Station(const string& name, const string& location, const string& type)
        : name(name), location(location), type(type), stationID(nextStationID++) {}
    
    // Getters
    string getName() const { return name; }
    int getStationID() const { return stationID; }
    string getLocation() const { return location; }
    string getType() const { return type; }
    
    // Schedule management methods
    void addArrival(int vehicleID, int routeID) {
        arrivals[vehicleID].push_back(routeID);
        cout << "Added incoming schedule: Vehicle " << vehicleID 
             << " Route " << routeID << " to Station " << stationID << endl;
    }
    
    void addDeparture(int vehicleID, int routeID) {
        departures[vehicleID].push_back(routeID);
        cout << "Added outgoing schedule: Vehicle " << vehicleID 
             << " Route " << routeID << " from Station " << stationID << endl;
    }
    
    vector<int> getArrivingVehicles() const {
        vector<int> vehicles;
        for (const auto& pair : arrivals) {
            vehicles.push_back(pair.first);
        }
        return vehicles;
    }
    
    vector<int> getDepartingVehicles() const {
        vector<int> vehicles;
        for (const auto& pair : departures) {
            vehicles.push_back(pair.first);
        }
        return vehicles;
    }
    
    vector<int> getArrivalRoutes(int vehicleID) const {
        auto it = arrivals.find(vehicleID);
        if (it != arrivals.end()) {
            return it->second;
        }
        return vector<int>();
    }
    
    vector<int> getDepartureRoutes(int vehicleID) const {
        auto it = departures.find(vehicleID);
        if (it != departures.end()) {
            return it->second;
        }
        return vector<int>();
    }
    
    void displayInfo() const {
        cout << "\n=== STATION INFORMATION ===" << endl;
        cout << "Station ID: " << stationID << endl;
        cout << "Name: " << name << endl;
        cout << "Location: " << location << endl;
        cout << "Type: " << type << endl;
        
        cout << "\n--- INCOMING SCHEDULES ---" << endl;
        if (arrivals.empty()) {
            cout << "No incoming schedules" << endl;
        } else {
            for (const auto& vehiclePair : arrivals) {
                cout << "Vehicle " << vehiclePair.first << " (Routes: ";
                for (size_t i = 0; i < vehiclePair.second.size(); ++i) {
                    cout << vehiclePair.second[i];
                    if (i < vehiclePair.second.size() - 1) cout << ", ";
                }
                cout << ")" << endl;
            }
        }
        
        cout << "\n--- OUTGOING SCHEDULES ---" << endl;
        if (departures.empty()) {
            cout << "No outgoing schedules" << endl;
        } else {
            for (const auto& vehiclePair : departures) {
                cout << "Vehicle " << vehiclePair.first << " (Routes: ";
                for (size_t i = 0; i < vehiclePair.second.size(); ++i) {
                    cout << vehiclePair.second[i];
                    if (i < vehiclePair.second.size() - 1) cout << ", ";
                }
                cout << ")" << endl;
            }
        }
    }
};

// Enhanced Passenger class với chi tiết booking
struct BookingInfo {
    int vehicleID;
    int routeID;
    int fromStationID;
    int toStationID;
    Time departureTime;
    Time arrivalTime;
    
    BookingInfo(int vID, int rID, int from, int to, Time dept, Time arr)
        : vehicleID(vID), routeID(rID), fromStationID(from), toStationID(to),
          departureTime(dept), arrivalTime(arr) {}
    
    void displayInfo() const {
        cout << "  Vehicle " << vehicleID << ", Route " << routeID 
             << ", From Station " << fromStationID << " to Station " << toStationID
             << ", " << departureTime.toString() << " - " << arrivalTime.toString() << endl;
    }
};

class Passenger {
private:
    string name;
    int passengerID;
    vector<BookingInfo> bookings;
    static int nextPassengerID;

public:
    Passenger(const string& name) 
        : name(name), passengerID(nextPassengerID++) {}
    
    // Getters
    string getName() const { return name; }
    int getPassengerID() const { return passengerID; }
    vector<BookingInfo> getBookings() const { return bookings; }
    
    // Enhanced booking methods
    bool bookRoute(Vehicle* vehicle, int routeID) {
        if (!vehicle) {
            cout << "Vehicle not found!" << endl;
            return false;
        }
        
        Route* route = vehicle->findRoute(routeID);
        if (!route) {
            cout << "Route " << routeID << " not found on Vehicle " << vehicle->getVehicleID() << endl;
            return false;
        }
        
        if (!vehicle->hasAvailableSeats()) {
            cout << "No available seats on Vehicle " << vehicle->getVehicleID() << endl;
            return false;
        }
        
        // Check if already booked
        for (const auto& booking : bookings) {
            if (booking.vehicleID == vehicle->getVehicleID() && booking.routeID == routeID) {
                cout << "Already booked this route!" << endl;
                return false;
            }
        }
        
        if (vehicle->bookSeat()) {
            BookingInfo booking(vehicle->getVehicleID(), routeID, 
                              route->getFromStationID(), route->getToStationID(),
                              route->getDepartureTime(), route->getArrivalTime());
            bookings.push_back(booking);
            cout << name << " successfully booked: ";
            booking.displayInfo();
            return true;
        }
        
        return false;
    }
    
    bool cancelRoute(Vehicle* vehicle, int routeID) {
        if (!vehicle) return false;
        
        for (auto it = bookings.begin(); it != bookings.end(); ++it) {
            if (it->vehicleID == vehicle->getVehicleID() && it->routeID == routeID) {
                vehicle->cancelSeat();
                cout << name << " cancelled booking: ";
                it->displayInfo();
                bookings.erase(it);
                return true;
            }
        }
        
        cout << "Booking not found for " << name << endl;
        return false;
    }
    
    void displayInfo() const {
        cout << "\n=== PASSENGER INFORMATION ===" << endl;
        cout << "Name: " << name << endl;
        cout << "ID: " << passengerID << endl;
        cout << "Bookings: " << endl;
        if (bookings.empty()) {
            cout << "  No bookings" << endl;
        } else {
            for (const auto& booking : bookings) {
                booking.displayInfo();
            }
        }
    }
};

// Enhanced Transportation Management System
class TransportationSystem {
private:
    vector<Vehicle*> vehicles;
    vector<Station*> stations;
    vector<Passenger*> passengers;

public:
    ~TransportationSystem() {
        for (Vehicle* vehicle : vehicles) delete vehicle;
        for (Station* station : stations) delete station;
        for (Passenger* passenger : passengers) delete passenger;
    }
    
    void addVehicle(Vehicle* vehicle) {
        vehicles.push_back(vehicle);
        cout << "Added " << vehicle->getVehicleType() << " '" 
             << vehicle->getVehicleName() << "' (ID: " << vehicle->getVehicleID() 
             << ") to the system." << endl;
    }
    
    void addStation(Station* station) {
        stations.push_back(station);
        cout << "Added station '" << station->getName() 
             << "' (ID: " << station->getStationID() << ") to the system." << endl;
    }
    
    void addPassenger(Passenger* passenger) {
        passengers.push_back(passenger);
        cout << "Added passenger '" << passenger->getName() 
             << "' (ID: " << passenger->getPassengerID() << ") to the system." << endl;
    }
    
    Vehicle* findVehicle(int vehicleID) {
        for (Vehicle* vehicle : vehicles) {
            if (vehicle->getVehicleID() == vehicleID) {
                return vehicle;
            }
        }
        return nullptr;
    }
    
    Station* findStation(int stationID) {
        for (Station* station : stations) {
            if (station->getStationID() == stationID) {
                return station;
            }
        }
        return nullptr;
    }
    
    Passenger* findPassenger(int passengerID) {
        for (Passenger* passenger : passengers) {
            if (passenger->getPassengerID() == passengerID) {
                return passenger;
            }
        }
        return nullptr;
    }
    
    // Method to create and assign route to vehicle and update station schedules
    bool createRoute(int vehicleID, int fromStationID, int toStationID, 
                     int deptHour, int deptMin, int arrHour, int arrMin) {
        Vehicle* vehicle = findVehicle(vehicleID);
        Station* fromStation = findStation(fromStationID);
        Station* toStation = findStation(toStationID);
        
        if (!vehicle || !fromStation || !toStation) {
            cout << "Vehicle, from station, or to station not found!" << endl;
            return false;
        }
        
        Time deptTime(deptHour, deptMin);
        Time arrTime(arrHour, arrMin);
        Route newRoute(fromStationID, toStationID, deptTime, arrTime);
        
        vehicle->addRoute(newRoute);
        fromStation->addDeparture(vehicleID, newRoute.getRouteID());
        toStation->addArrival(vehicleID, newRoute.getRouteID());
        
        return true;
    }
    
    void displayAllVehicles() const {
        cout << "\n=== ALL VEHICLES ===" << endl;
        for (Vehicle* vehicle : vehicles) {
            vehicle->displayInfo();
        }
    }
    
    void displayAllStations() const {
        cout << "\n=== ALL STATIONS ===" << endl;
        for (Station* station : stations) {
            station->displayInfo();
        }
    }
    
    void displayAllPassengers() const {
        cout << "\n=== ALL PASSENGERS ===" << endl;
        for (Passenger* passenger : passengers) {
            passenger->displayInfo();
        }
    }
    
    void displayAvailableRoutes() const {
        cout << "\n=== AVAILABLE ROUTES ===" << endl;
        for (Vehicle* vehicle : vehicles) {
            cout << "\nVehicle " << vehicle->getVehicleID() << " (" 
                 << vehicle->getVehicleName() << "):" << endl;
            for (const Route& route : vehicle->getAssignedRoutes()) {
                cout << "  ";
                route.displayInfo();
            }
        }
    }
};

// Initialize static members
int Vehicle::nextVehicleID = 1;
int Passenger::nextPassengerID = 1;
int Station::nextStationID = 1;
int Route::nextRouteID = 1;

// Sample data initialization
void initializeSampleData(TransportationSystem& ts) {
    // Add stations
    ts.addStation(new Station("Central Station", "City Center", "Bus"));
    ts.addStation(new Station("Airport Terminal", "Airport District", "Bus"));
    ts.addStation(new Station("University Campus", "Education District", "Bus"));
    ts.addStation(new Station("Shopping Mall", "Commercial District", "Bus"));
    
    // Add vehicles
    ts.addVehicle(new Vehicle("City Bus A1", 45));
    ts.addVehicle(new ExpressBus("Express X1", 35, 70, 5));
    ts.addVehicle(new Vehicle("City Bus B2", 50));
    
    // Create some routes
    ts.createRoute(1, 1, 2, 8, 30, 9, 15);  // Vehicle 1: Station 1 to 2, 8:30-9:15
    ts.createRoute(1, 2, 3, 9, 30, 10, 0);  // Vehicle 1: Station 2 to 3, 9:30-10:00
    ts.createRoute(1, 3, 1, 10, 15, 11, 0);  // Vehicle 1: Station 3 to 1, 10:15-11:00
    
    ts.createRoute(2, 1, 4, 7, 0, 7, 30);   // Vehicle 2: Station 1 to 4, 7:00-7:30
    ts.createRoute(2, 4, 2, 7, 45, 8, 10);  // Vehicle 2: Station 4 to 2, 7:45-8:10
    
    // Add passengers
    ts.addPassenger(new Passenger("Nguyen Van A"));
    ts.addPassenger(new Passenger("Tran Thi B"));
    
    cout << "\nSample data initialized successfully!" << endl;
}

int main() {
    cout << "=== ENHANCED PUBLIC TRANSPORTATION MANAGEMENT SYSTEM ===" << endl;
    
    TransportationSystem system;
    initializeSampleData(system);
    
    // Demo booking
    cout << "\n=== DEMO BOOKINGS ===" << endl;
    Vehicle* vehicle1 = system.findVehicle(1);
    Passenger* passenger1 = system.findPassenger(1);
    
    if (vehicle1 && passenger1) {
        passenger1->bookRoute(vehicle1, 1); // Book route 1 on vehicle 1
        passenger1->bookRoute(vehicle1, 2); // Book route 2 on vehicle 1
    }
    
    // Display information
    system.displayAvailableRoutes();
    system.displayAllStations();
    system.displayAllPassengers();
    
    return 0;
}