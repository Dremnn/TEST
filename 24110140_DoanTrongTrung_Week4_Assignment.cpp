#include <bits/stdc++.h>
using namespace std;

// Forward declarations
class Doctor;
class Appointment;

// Base Patient class
class Patient {
protected:
    string name;
    int patientID;
    int age;
    vector<string> medicalHistory;

public:
    // Constructor
    Patient(const string& name, int id, int age) 
        : name(name), patientID(id), age(age) {}
    
    // Virtual destructor for proper inheritance
    virtual ~Patient() {}
    
    // Getters
    string getName() const { return name; }
    int getPatientID() const { return patientID; }
    int getAge() const { return age; }
    vector<string> getMedicalHistory() const { return medicalHistory; }
    
    // Methods
    void addMedicalRecord(const string& record) {
        medicalHistory.push_back(record);
    }
    
    // Virtual method for scheduling appointments (to be overridden)
    virtual string getAppointmentFrequency() const {
        return "As needed";
    }
    
    // Virtual method for patient type
    virtual string getPatientType() const {
        return "Regular Patient";
    }
    
    // Display patient information
    virtual void displayInfo() const {
        cout << "Patient Type: " << getPatientType() << endl;
        cout << "Name: " << name << endl;
        cout << "ID: " << patientID << endl;
        cout << "Age: " << age << endl;
        cout << "Appointment Frequency: " << getAppointmentFrequency() << endl;
        cout << "Medical History: ";
        if (medicalHistory.empty()) {
            cout << "None recorded" << endl;
        } else {
            cout << endl;
            for (const auto& record : medicalHistory) {
                cout << "  - " << record << endl;
            }
        }
    }
};

// Derived ChronicPatient class (demonstrating inheritance)
class ChronicPatient : public Patient {
private:
    string conditionType;
    string lastCheckupDate;

public:
    // Constructor
    ChronicPatient(const string& name, int id, int age, 
                   const string& condition, const string& lastCheckup)
        : Patient(name, id, age), conditionType(condition), lastCheckupDate(lastCheckup) {}
    
    // Getters
    string getConditionType() const { return conditionType; }
    string getLastCheckupDate() const { return lastCheckupDate; }
    
    // Setters
    void setLastCheckupDate(const string& date) { lastCheckupDate = date; }
    
    // Override virtual methods
    string getAppointmentFrequency() const override {
        return "Every 3 months (chronic condition)";
    }
    
    string getPatientType() const override {
        return "Chronic Patient";
    }
    
    // Override display method
    void displayInfo() const override {
        Patient::displayInfo(); // Call base class method
        cout << "Chronic Condition: " << conditionType << endl;
        cout << "Last Checkup: " << lastCheckupDate << endl;
    }
};

// Doctor class
class Doctor {
private:
    string name;
    int doctorID;
    string specialty;
    vector<int> appointmentIDs; // Store appointment IDs assigned to this doctor

public:
    // Constructor
    Doctor(const string& name, int id, const string& specialty)
        : name(name), doctorID(id), specialty(specialty) {}
    
    // Getters
    string getName() const { return name; }
    int getDoctorID() const { return doctorID; }
    string getSpecialty() const { return specialty; }
    vector<int> getAppointmentIDs() const { return appointmentIDs; }
    
    // Methods
    void assignAppointment(int appointmentID) {
        appointmentIDs.push_back(appointmentID);
    }
    
    void removeAppointment(int appointmentID) {
        appointmentIDs.erase(
            remove(appointmentIDs.begin(), appointmentIDs.end(), appointmentID),
            appointmentIDs.end()
        );
    }
    
    void displayInfo() const {
        cout << "Doctor: " << name << endl;
        cout << "ID: " << doctorID << endl;
        cout << "Specialty: " << specialty << endl;
        cout << "Assigned Appointments: " << appointmentIDs.size() << endl;
    }
};

// Appointment class
class Appointment {
public:
    enum Status { SCHEDULED, COMPLETED, CANCELED };
    
private:
    static int nextAppointmentID;
    int appointmentID;
    string date;
    string time;
    string reason;
    Status status;
    int patientID;
    int doctorID;

public:
    // Constructor
    Appointment(const string& date, const string& time, 
                const string& reason, int patientID, int doctorID)
        : appointmentID(nextAppointmentID++), date(date), time(time), 
          reason(reason), status(SCHEDULED), patientID(patientID), doctorID(doctorID) {}
    
    // Getters
    int getAppointmentID() const { return appointmentID; }
    string getDate() const { return date; }
    string getTime() const { return time; }
    string getReason() const { return reason; }
    Status getStatus() const { return status; }
    int getPatientID() const { return patientID; }
    int getDoctorID() const { return doctorID; }
    
    // Methods
    void setStatus(Status newStatus) { status = newStatus; }
    
    string getStatusString() const {
        switch (status) {
            case SCHEDULED: return "Scheduled";
            case COMPLETED: return "Completed";
            case CANCELED: return "Canceled";
            default: return "Unknown";
        }
    }
    
    void displayInfo() const {
        cout << "Appointment ID: " << appointmentID << endl;
        cout << "Date: " << date << endl;
        cout << "Time: " << time << endl;
        cout << "Reason: " << reason << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Patient ID: " << patientID << endl;
        cout << "Doctor ID: " << doctorID << endl;
    }

    void reschedule(const string& newDate , const string& newTime) {
        if (status == SCHEDULED) {
            date = newDate;
            time = newTime;
            cout << "Appointment rescheduled to: " << newDate << " at " << newTime << endl;
        } else {
            cout << "Cannot reschedule. Current status: " << getStatusString() << endl;

        }
    }
};

// Medicine class
class Medicine {
private:
    int medicineID;
    string name;
    string description;
    double price;
    int stockQuantity;
    string expiryDate;
    string manufacturer;

public:
    Medicine(int id, string n, string desc, double p, int stock, string exp, string manu)
        : medicineID(id), name(n), description(desc), price(p),
          stockQuantity(stock), expiryDate(exp), manufacturer(manu) {}

    bool isExpired(string currentDate) {
        return expiryDate < currentDate;
    }

    bool checkStock(int quantity) {
        return stockQuantity >= quantity;
    }

    void updateStock(int quantity) {
        if (quantity <= stockQuantity) stockQuantity -= quantity;
    }

    void prescribe() {
        cout << "Prescribed: " << name << " - " << description << endl;
    }

    double getPrice() const { return price; }
    string getName() const { return name; }
};

// Prescription class
class Prescription {
private:
    int prescriptionID;
    int patientID;
    int doctorID;
    vector<Medicine> medicines;
    string prescriptionDate;
    string instructions;

public:
    Prescription(int pid, int did, string date, string instr)
        : patientID(pid), doctorID(did), prescriptionDate(date), instructions(instr) {}

    void addMedicine(const Medicine& med) {
        medicines.push_back(med);
    }

    double calculateTotalCost() const {
        double total = 0;
        for (const auto& med : medicines) {
            total += med.getPrice();
        }
        return total;
    }

    void printPrescription() const {
        cout << "Prescription for Patient ID: " << patientID << endl;
        for (const auto& med : medicines) {
            cout << "- " << med.getName() << endl;
        }
        cout << "Instructions: " << instructions << endl;
    }
};

// Bill/Invoice Class
class Bill {
private:
    int billID;
    int patientID;
    int appointmentID;
    double consultationFee;
    double medicationCost;
    double totalAmount;
    string billDate;
    bool isPaid;

public:
    Bill(int bid, int pid, int aid, double fee, double medCost, string date)
        : billID(bid), patientID(pid), appointmentID(aid),
          consultationFee(fee), medicationCost(medCost),
          billDate(date), isPaid(false) {
        calculateTotal();
    }

    void calculateTotal() {
        totalAmount = consultationFee + medicationCost;
    }

    void processPayment() {
        isPaid = true;
        cout << "Payment processed for Bill ID: " << billID << endl;
    }

    void printBill() const {
        cout << "Bill ID: " << billID << "\nTotal: $" << totalAmount
             << "\nStatus: " << (isPaid ? "Paid" : "Unpaid") << endl;
    }
};

// Initialize static member
int Appointment::nextAppointmentID = 1;

// Clinic Management System class
class ClinicManagementSystem {
private:
    vector<Patient*> patients;
    vector<Doctor> doctors;
    vector<Appointment> appointments;
    vector<Medicine> medicineInventory;
    vector<Prescription> prescriptions;
    vector<Bill> bills;

public:
    // Destructor to clean up dynamic memory
    ~ClinicManagementSystem() {
        for (Patient* patient : patients) {
            delete patient;
        }
    }
    
    // Add regular patient
    void addPatient(const string& name, int age) {
        int id = patients.size() + 1;
        patients.push_back(new Patient(name, id, age));
        cout << "Regular patient added successfully. ID: " << id << endl;
    }
    
    // Add chronic patient
    void addChronicPatient(const string& name, int age, 
                          const string& condition, const string& lastCheckup) {
        int id = patients.size() + 1;
        patients.push_back(new ChronicPatient(name, id, age, condition, lastCheckup));
        cout << "Chronic patient added successfully. ID: " << id << endl;
    }
    
    // Add doctor
    void addDoctor(const string& name, const string& specialty) {
        int id = doctors.size() + 1;
        doctors.push_back(Doctor(name, id, specialty));
        cout << "Doctor added successfully. ID: " << id << endl;
    }

    // Add medicine to inventory
    void addMedicine(const Medicine& med) {
        medicineInventory.push_back(med);
        cout << "Medicine added: " << med.getName() << endl;
    }

    // Schedule appointment
    bool scheduleAppointment(const string& date, const string& time,
                           const string& reason, int patientID, int doctorID) {
        // Validate patient and doctor IDs
        if (patientID < 1 || patientID > static_cast<int>(patients.size())) {
            cout << "Error: Invalid patient ID." << endl;
            return false;
        }
        if (doctorID < 1 || doctorID > static_cast<int>(doctors.size())) {
            cout << "Error: Invalid doctor ID." << endl;
            return false;
        }
        
        appointments.push_back(Appointment(date, time, reason, patientID, doctorID));
        int appointmentID = appointments.back().getAppointmentID();
        
        // Assign appointment to doctor
        doctors[doctorID - 1].assignAppointment(appointmentID);
        
        cout << "Appointment scheduled successfully. Appointment ID: " << appointmentID << endl;
        return true;
    }
    
    // Cancel appointment
    bool cancelAppointment(int appointmentID) {
        for (auto& appointment : appointments) {
            if (appointment.getAppointmentID() == appointmentID) {
                if (appointment.getStatus() != Appointment::CANCELED) {
                    appointment.setStatus(Appointment::CANCELED);
                    
                    // Remove from doctor's appointment list
                    int doctorID = appointment.getDoctorID();
                    doctors[doctorID - 1].removeAppointment(appointmentID);
                    
                    cout << "Appointment " << appointmentID << " canceled successfully." << endl;
                    return true;
                } else {
                    cout << "Appointment " << appointmentID << " is already canceled." << endl;
                    return false;
                }
            }
        }
        cout << "Error: Appointment ID not found." << endl;
        return false;
    }
    
    // Complete appointment
    bool completeAppointment(int appointmentID, const string& treatmentRecord) {
        for (auto& appointment : appointments) {
            if (appointment.getAppointmentID() == appointmentID) {
                if (appointment.getStatus() == Appointment::SCHEDULED) {
                    appointment.setStatus(Appointment::COMPLETED);
                    
                    // Add treatment record to patient's medical history
                    int patientID = appointment.getPatientID();
                    patients[patientID - 1]->addMedicalRecord(treatmentRecord);
                    
                    cout << "Appointment " << appointmentID << " completed successfully." << endl;
                    return true;
                } else {
                    cout << "Appointment " << appointmentID << " cannot be completed (not scheduled)." << endl;
                    return false;
                }
            }
        }
        cout << "Error: Appointment ID not found." << endl;
        return false;
    }

    // Reschedule appointment
    bool rescheduleAppointment(int appointmentID, const string& newDate, const string& newTime) {
        for (auto& appointment : appointments) {
            if (appointment.getAppointmentID() == appointmentID) {
                if (appointment.getStatus() == Appointment::SCHEDULED) {
                    appointment.reschedule(newDate, newTime);
                    cout << "Appointment " << appointmentID << " rescheduled successfully." << endl;
                    return true;
                } else {
                    cout << "Cannot reschedule. Appointment is " << appointment.getStatusString() << "." << endl;
                    return false;
                }
            }
        }
        cout << "Error: Appointment ID not found." << endl;
        return false;
    }

    // Create prescription
    void createPrescription(int patientID, int doctorID, const string& date, const string& instructions, const vector<int>& medicineIDs) {
        if (patientID < 1 || patientID > static_cast<int>(patients.size())) {
            cout << "Error: Invalid patient ID." << endl;
            return;
        }
        if (doctorID < 1 || doctorID > static_cast<int>(doctors.size())) {
            cout << "Error: Invalid doctor ID." << endl;
            return;
        }

        Prescription p(patientID, doctorID, date, instructions);
        for (int id : medicineIDs) {
            if (id >= 0 && id < static_cast<int>(medicineInventory.size())) {
                p.addMedicine(medicineInventory[id]);
            } else {
                cout << "Invalid medicine ID: " << id << endl;
            }
        }
        prescriptions.push_back(p);
        cout << "Prescription created for Patient ID: " << patientID << endl;
    }

    // Generate bill
    void generateBill(int patientID, int appointmentID, double consultationFee, int prescriptionIndex, const string& date) {
        if (patientID < 1 || patientID > static_cast<int>(patients.size())) {
            cout << "Error: Invalid patient ID." << endl;
            return;
        }
        if (prescriptionIndex < 0 || prescriptionIndex >= static_cast<int>(prescriptions.size())) {
            cout << "Error: Invalid prescription index." << endl;
            return;
        }

        double medicationCost = prescriptions[prescriptionIndex].calculateTotalCost();
        int billID = bills.size() + 1;
        Bill b(billID, patientID, appointmentID, consultationFee, medicationCost, date);
        bills.push_back(b);
        cout << "Bill generated. ID: " << billID << endl;
    }
    
    //Display separated patient
    void displayPatient(int patientID) const {
        if (patientID < 1 || patientID > static_cast<int>(patients.size())) {
            cout << "Error: Invalid patient ID." << endl;
            return;
        }
        cout << "\n=== PATIENT DETAILS ===" << endl;
        patients[patientID - 1]->displayInfo();
    }

    // Display all patients
    void displayAllPatients() const {
        cout << "\n=== ALL PATIENTS ===" << endl;
        if (patients.empty()) {
            cout << "No patients registered." << endl;
            return;
        }
        
        for (size_t i = 0; i < patients.size(); ++i) {
            cout << "\n--- Patient " << (i + 1) << " ---" << endl;
            patients[i]->displayInfo();
        }
    }

    // Display separated doctor
    void displayDoctor(int doctorID) const {
        if (doctorID < 1 || doctorID > static_cast<int>(doctors.size())) {
            cout << "Error: Invalid doctor ID." << endl;
            return;
        }
        cout << "\n=== DOCTOR DETAILS ===" << endl;
        doctors[doctorID - 1].displayInfo();
    }
    
    // Display all doctors
    void displayAllDoctors() const {
        cout << "\n=== ALL DOCTORS ===" << endl;
        if (doctors.empty()) {
            cout << "No doctors registered." << endl;
            return;
        }
        
        for (size_t i = 0; i < doctors.size(); ++i) {
            cout << "\n--- Doctor " << (i + 1) << " ---" << endl;
            doctors[i].displayInfo();
        }
    }
    
    // Display all appointments
    void displayAllAppointments() const {
        cout << "\n=== ALL APPOINTMENTS ===" << endl;
        if (appointments.empty()) {
            cout << "No appointments scheduled." << endl;
            return;
        }
        
        for (size_t i = 0; i < appointments.size(); ++i) {
            cout << "\n--- Appointment " << (i + 1) << " ---" << endl;
            appointments[i].displayInfo();
        }
    }

    // Display all medicines
    void displayAllMedicines() const {
        cout << "\n=== MEDICINE INVENTORY ===" << endl;
        if (medicineInventory.empty()) {
            cout << "No medicines available." << endl;
            return;
        }
        for (const auto& med : medicineInventory) {
            cout << "- " << med.getName() << " | Price: $" << med.getPrice() << endl;
        }
    }

    // Display all prescriptions
    void displayAllPrescriptions() const {
        cout << "\n=== PRESCRIPTIONS ===" << endl;
        if (prescriptions.empty()) {
            cout << "No prescriptions available." << endl;
            return;
        }
        for (const auto& p : prescriptions) {
            p.printPrescription();
            cout << "------------------------" << endl;
        }
    }

    // Display all bills
    void displayAllBills() const {
        cout << "\n=== BILLS ===" << endl;
        if (bills.empty()) {
            cout << "No bills available." << endl;
            return;
        }
        for (const auto& b : bills) {
            b.printBill();
            cout << "------------------------" << endl;
        }
    }
    
    // Find patient by ID
    Patient* findPatient(int patientID) {
        if (patientID >= 1 && patientID <= static_cast<int>(patients.size())) {
            return patients[patientID - 1];
        }
        return nullptr;
    }
    
    // Find doctor by ID
    Doctor* findDoctor(int doctorID) {
        if (doctorID >= 1 && doctorID <= doctors.size()) {
            return &doctors[doctorID - 1];
        }
        return nullptr;
    }
};

// Test functions
void runBasicTests(ClinicManagementSystem& clinic) {
    cout << "\n" << string(50, '=') << endl;
    cout << "RUNNING BASIC FUNCTIONALITY TESTS" << endl;
    cout << string(50, '=') << endl;
    
    // Test 1: Add patients
    cout << "\nTest 1: Adding Patients" << endl;
    cout << string(25, '-') << endl;
    clinic.addPatient("John Smith", 35);
    clinic.addChronicPatient("Mary Johnson", 62, "Type 2 Diabetes", "2024-01-15");
    clinic.addPatient("Bob Wilson", 28);
    
    // Test 2: Add doctors
    cout << "\nTest 2: Adding Doctors" << endl;
    cout << string(25, '-') << endl;
    clinic.addDoctor("Dr. Sarah Miller", "General Medicine");
    clinic.addDoctor("Dr. Michael Brown", "Endocrinology");
    
    // Test 3: Schedule appointments
    cout << "\nTest 3: Scheduling Appointments" << endl;
    cout << string(25, '-') << endl;
    clinic.scheduleAppointment("2024-03-15", "10:00 AM", "Annual Checkup", 1, 1);
    clinic.scheduleAppointment("2024-03-16", "2:00 PM", "Diabetes Management", 2, 2);
    clinic.scheduleAppointment("2024-03-17", "11:30 AM", "Follow-up Visit", 3, 1);
    
    // Test 4: Complete an appointment
    cout << "\nTest 4: Completing Appointment" << endl;
    cout << string(25, '-') << endl;
    clinic.completeAppointment(1, "Annual checkup completed. Patient in good health. Recommended annual screening.");
    
    // Test 5: Cancel an appointment
    cout << "\nTest 5: Canceling Appointment" << endl;
    cout << string(25, '-') << endl;
    clinic.cancelAppointment(3);
}

void runInheritanceTests(ClinicManagementSystem& clinic) {
    cout << "\n" << string(50, '=') << endl;
    cout << "RUNNING INHERITANCE DEMONSTRATION TESTS" << endl;
    cout << string(50, '=') << endl;
    
    cout << "\nDemonstrating Polymorphism and Method Override:" << endl;
    cout << string(45, '-') << endl;
    
    Patient* regularPatient = clinic.findPatient(1);
    Patient* chronicPatient = clinic.findPatient(2);
    
    if (regularPatient && chronicPatient) {
        cout << "\nRegular Patient Appointment Frequency: " 
                  << regularPatient->getAppointmentFrequency() << endl;
        cout << "Chronic Patient Appointment Frequency: " 
                  << chronicPatient->getAppointmentFrequency() << endl;
        
        cout << "\nPatient Type Demonstration:" << endl;
        cout << "Patient 1 Type: " << regularPatient->getPatientType() << endl;
        cout << "Patient 2 Type: " << chronicPatient->getPatientType() << endl;
    }
}

void runErrorHandlingTests(ClinicManagementSystem& clinic) {
    cout << "\n" << string(50, '=') << endl;
    cout << "RUNNING ERROR HANDLING TESTS" << endl;
    cout << string(50, '=') << endl;
    
    cout << "\nTest: Invalid Patient ID" << endl;
    cout << string(25, '-') << endl;
    clinic.scheduleAppointment("2024-03-20", "9:00 AM", "Checkup", 999, 1);
    
    cout << "\nTest: Invalid Doctor ID" << endl;
    cout << string(25, '-') << endl;
    clinic.scheduleAppointment("2024-03-20", "9:00 AM", "Checkup", 1, 999);
    
    cout << "\nTest: Invalid Appointment ID" << endl;
    cout << string(25, '-') << endl;
    clinic.cancelAppointment(999);
    
    cout << "\nTest: Double Cancellation" << endl;
    cout << string(25, '-') << endl;
    clinic.cancelAppointment(3); // Already canceled
}

int main() {
    cout << string(60, '=') << endl;
    cout << "SMALL CLINIC MANAGEMENT SYSTEM" << endl;
    cout << "Object-Oriented Programming Demonstration" << endl;
    cout << string(60, '=') << endl;
    
    ClinicManagementSystem clinic;
    
    // Run comprehensive tests
    runBasicTests(clinic);
    runInheritanceTests(clinic);
    runErrorHandlingTests(clinic);
    
    // Display final system state
    cout << "\n" << string(50, '=') << endl;
    cout << "FINAL SYSTEM STATE" << endl;
    cout << string(50, '=') << endl;
    
    clinic.displayAllPatients();
    clinic.displayAllDoctors();
    clinic.displayAllAppointments();
    
    cout << "\n" << string(60, '=') << endl;
    cout << "SYSTEM DEMONSTRATION COMPLETED SUCCESSFULLY" << endl;
    cout << string(60, '=') << endl;
    
    return 0;
}