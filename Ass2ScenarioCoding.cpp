#include <iostream>
#include <iomanip>  // Required for formatting
#include <string>
using namespace std;

// Constants
const double TAX_RATE = 0.02;
const double SOCIAL_SECURITY_TAX = 10.0;
const int MAX_WORKERS = 100;  // Maximum number of workers

// Worker class
class Worker
{
private:
    string workerId;
    string workerName;
    string jobTitle;
    string employmentType;  // "fulltime" or "parttime"
    int sickDays;  // Only for full-time workers
    int extraHours;  // Only for full-time workers
    double partTimeHours;  // Working hours for part-time workers

public:
    // Constructor
    Worker() : sickDays(0), extraHours(0), partTimeHours(0) {}

    // Member function to input worker data
    void input()
    {
        cout << "\nEnter Worker ID      : ";
        cin >> workerId;

        cout << "Enter Worker Name    : ";
        cin.ignore();
        getline(cin, workerName);

        // Show the menu before inputting job title
        showJobMenu();

        cout << "\nEnter Job Title      : ";
        getline(cin, jobTitle);

        cout << "Enter Employment Type (fulltime/parttime): ";
        cin >> employmentType;

        // Input validation for employment type
        while (employmentType != "fulltime" && employmentType != "parttime")
        {
            cout << "Invalid type! Please enter 'fulltime' or 'parttime': ";
            cin >> employmentType;
        }

        if (employmentType == "parttime")
        {
            cout << "Enter Working Hours   : ";
            cin >> partTimeHours;
            while (partTimeHours < 0)
            {
                cout << "Invalid input! Working hours cannot be negative. Please re-enter: ";
                cin >> partTimeHours;
            }
            extraHours = 0;
            sickDays = 0;  // Reset sick days for part-time
        }
        else
        { // full-time
            partTimeHours = 160;  // Default working hours for full-time workers
            cout << "Enter Sick Days       : ";
            cin >> sickDays;
            while (sickDays < 0)
            {
                cout << "Invalid input! Sick days cannot be negative. Please re-enter: ";
                cin >> sickDays;
            }

            cout << "Enter Extra Hours     : ";
            cin >> extraHours;

            while (extraHours < 0)
            {
                cout << "Invalid input! Extra hours cannot be negative. Please re-enter: ";
                cin >> extraHours;
                extraHours = static_cast<int>(extraHours);  // Ensure extra hours are treated as an integer
            }
        }
    }

    // Getter methods
    string getId() const { return workerId; }
    string getName() const { return workerName; }
    string getType() const { return employmentType; }
    string getJobTitle() const { return jobTitle; }
    int getSickDays() const { return sickDays; }
    double getExtraHours() const { return extraHours; }
    double getPartTimeHours() const { return partTimeHours; }

    // Function to display available job titles and basic salaries
    void showJobMenu() const
    {
        cout << "\nAvailable Job Titles and Basic Salaries:\n";
        cout << "-----------------------------------------\n";
        cout << "Barista                     : $1920\n";
        cout << "Cashier                     : $1790\n";
        cout << "Kitchen Staff (part-time)   : $1850\n";
        cout << "Cleaner (part-time)         : $1400\n";
        cout << "Waiter/waitress (part-time) : $1700\n";
        cout << "Supervisor                  : $2400\n";
        cout << "-----------------------------------------\n";
    }

    // Member function to calculate the worker's salary
    double calculateSalary(double &grossSalary, double &taxCost) const
    {
        double baseSalary = getBaseSalary();
        double hourlyRate = baseSalary / (30 * 8);

        if (employmentType == "parttime")
        {
            if (partTimeHours > 0)
            {
                grossSalary = hourlyRate * partTimeHours;
                taxCost = 0; // Part-time workers don't have taxes
                return grossSalary;
            }
            else
            {
                cout << "Error: Invalid working hours for part-time worker." << endl;
                return 0;
            }
        }
        else if (employmentType == "fulltime")
        {  // Full-time worker salary calculation
            double extraPay = 2 * hourlyRate * extraHours;
            double sickBonus = (sickDays <= 2) ? getSickBonus() : 0;
            grossSalary = baseSalary + extraPay + sickBonus;

            // Calculate tax and deductions for full-time workers
            taxCost = TAX_RATE * grossSalary + SOCIAL_SECURITY_TAX;
            double netSalary = grossSalary - taxCost;

            // Deduct 1% for full-time workers with 3 or more sick days and extra hours
            if (sickDays >= 3 && extraHours > 0)
            {
                double deduction = 0.01 * grossSalary;
                netSalary -= deduction;
            }

            return netSalary < 0 ? 0 : netSalary;
        }

        return 0;  // Default case
    }

private:
    // Helper function to get base salary based on job title
    double getBaseSalary() const
    {
        if (employmentType == "parttime")
        {
            if (jobTitle == "Kitchen Staff") return 1850;
            if (jobTitle == "Cleaner") return 1400;
            if (jobTitle == "Waiter/waitress") return 1700;
        }
        else
        {
            if (jobTitle == "Barista") return 1920;
            if (jobTitle == "Cashier") return 1790;
            if (jobTitle == "Supervisor") return 2400;
        }
        return 0;
    }

    // Helper function to calculate sick bonus for full-time workers
    double getSickBonus() const
    {
        if (sickDays == 0) return 500;
        if (sickDays == 1) return 200;
        if (sickDays == 2) return 100;
        return 0;
    }
};

// WorkerManagement class to manage multiple workers
class WorkerManagement
{
private:
    Worker workers[MAX_WORKERS];  // Array to store workers
    int workerCount;  // Track the number of workers added

public:
    // Constructor
    WorkerManagement() : workerCount(0) {}

    // Function to add workers
    void addWorkers()
    {
        int numWorkers;
        cout << "\nHow many workers would you like to add? ";
        cin >> numWorkers;

        for (int i = 0; i < numWorkers; ++i)
        {
            if (workerCount < MAX_WORKERS)
            {
                workers[workerCount].input();
                workerCount++;
            }
            else
            {
                cout << "Worker list is full.\n";
                break;
            }
        }
        cout << "\nWorker(s) Added!\n\n";
    }

    // Print worker report based on type
    void printWorkerReport() const
    {
        string type;
        cout << "\nEnter Worker Type to view (fulltime/parttime): ";
        cin >> type;

        while (type != "fulltime" && type != "parttime")
        {
            cout << "Invalid input! Please enter 'fulltime' or 'parttime': ";
            cin >> type;
        }

        if (type == "fulltime")
        {
            printFullTimeReport();
        }
        else
        {
            printPartTimeReport();
        }
    }

    // Sort workers by name using insertion sort
	void sortWorkersByName()
	{
    	for (int i = 1; i < workerCount; i++)
    	{
        	Worker key = workers[i];
        	int j = i - 1;

        	// Compare names in ascending order
        	while (j >= 0 && workers[j].getName() > key.getName())
        	{
           		workers[j + 1] = workers[j];
            	j--;
        	}
        	workers[j + 1] = key;
    	}
	}

    // Print full-time workers in a column format
    void printFullTimeReport() 
	const
    {
        cout << "\nFull-Time Worker Report\n\n";
        cout << left << setw(8) << "ID"
             << setw(12) << "Name"
             << setw(15) << "Job Title"
             << setw(12) << "Type"
             << setw(12) << "Sick Days"
             << setw(10) << "Extra Hours"
             << setw(12) << "Before Tax"
             << setw(10) << "Tax Cost"
             << "Net Salary\n";
        cout << "-----------------------------------------------------------------------------------------------------\n";
        for (int i = 0; i < workerCount; ++i)
        {
            if (workers[i].getType() == "fulltime")
            {
                double grossSalary = 0, taxCost = 0;
                double netSalary = workers[i].calculateSalary(grossSalary, taxCost);

                cout << left << setw(8) << workers[i].getId()
                     << setw(12) << workers[i].getName()
                     << setw(15) << workers[i].getJobTitle()
                     << setw(12) << workers[i].getType()
                     << setw(12) << workers[i].getSickDays()
                     << setw(10) << static_cast<int>(workers[i].getExtraHours())
                     << "$" << setw(11) << fixed << setprecision(2) << grossSalary
                     << "$" << setw(10) << fixed << setprecision(2) << taxCost
                     << "$" << fixed << setprecision(2) << netSalary << "\n";
            }
        }
        cout << "-----------------------------------------------------------------------------------------------------\n\n";
    }

    // Print part-time workers in a column format
    void printPartTimeReport() const
    {
        cout << "\nPart-Time Worker Report\n\n";
        cout << left << setw(8) << "ID"
             << setw(12) << "Name"
             << setw(17) << "Job Title"
             << setw(12) << "Type"
             << setw(17) << "Working Hours"
             << "Net Salary\n";
        cout << "----------------------------------------------------------------------------\n";

        for (int i = 0; i < workerCount; ++i)
        {
            if (workers[i].getType() == "parttime")
            {
                double grossSalary = 0.0;
                double taxCost = 0.0;
                double netSalary = workers[i].calculateSalary(grossSalary, taxCost);

                cout << left << setw(8) << workers[i].getId()
                     << setw(12) << workers[i].getName()
                     << setw(17) << workers[i].getJobTitle()
                     << setw(12) << workers[i].getType()
                     << setw(17) << static_cast<int>(workers[i].getPartTimeHours())
                     << "$" << setw(10) << fixed << setprecision(2) << netSalary << "\n";
            }
        }
        cout << "----------------------------------------------------------------------------\n\n";
    }

    // Print all workers (full-time and part-time) in a combined report
	void printAllWorkersReport() 
	{
    	sortWorkersByName();  // Sort workers by name before printing

    	cout << "\nAll Workers Report (Sorted by Name)\n\n";
    	cout << left << setw(8) << "ID"
         	 << setw(12) << "Name"
         	 << setw(15) << "Job Title"
         	 << setw(12) << "Type"
         	 << "Net Salary\n";
    	cout << "---------------------------------------------------------\n";

    	for (int i = 0; i < workerCount; ++i)
    	{
        	double grossSalary = 0, taxCost = 0;
        	double netSalary = workers[i].calculateSalary(grossSalary, taxCost);

        	cout << left << setw(8) << workers[i].getId()
             	 << setw(12) << workers[i].getName()
             	 << setw(15) << workers[i].getJobTitle()
             	 << setw(12) << workers[i].getType()
             	 << "$" << setw(10) << fixed << setprecision(2) << netSalary << "\n";
    	}
    	cout << "---------------------------------------------------------\n\n";
	}

    // Calculate total monthly payment for all workers with breakdown
    void calculateTotalMonthlyPayment() 
	const
    {
        int fullTimeCount = 0, partTimeCount = 0;
        double fullTimeTotal = 0, partTimeTotal = 0;

        for (int i = 0; i < workerCount; ++i)
        {
            double grossSalary = 0, taxCost = 0;
            double netSalary = workers[i].calculateSalary(grossSalary, taxCost);

            if (workers[i].getType() == "fulltime")
            {
                fullTimeCount++;
                fullTimeTotal += netSalary;
            }
            else if (workers[i].getType() == "parttime")
            {
                partTimeCount++;
                partTimeTotal += netSalary;
            }
        }
        // Display the results in a table format
        cout << "\nMonthly Payment Summary\n";
        cout << "---------------------------------------------------\n";
        cout << left << setw(25) << "Worker Type"
             << setw(10) << "Count"
             << "Total Net Salary\n";
        cout << "---------------------------------------------------\n";
        cout << left << setw(25) << "Full-time Workers"
             << setw(10) << fullTimeCount
             << "$" << fixed << setprecision(2) << fullTimeTotal << "\n";
        cout << left << setw(25) << "Part-time Workers"
             << setw(10) << partTimeCount
             << "$" << fixed << setprecision(2) << partTimeTotal << "\n";
        cout << "---------------------------------------------------\n";
        cout << left << setw(25) << "Total Workers"
             << setw(10) << (fullTimeCount + partTimeCount)
             << "$" << fixed << setprecision(2) << (fullTimeTotal + partTimeTotal) << "\n";
        cout << "---------------------------------------------------\n\n";
    }
};

// Main program
int main()
{
    WorkerManagement workerManagement;

    int choice;
    do {
        cout << "Worker Management System\n";
        cout << "==========================\n";
        cout << "1. Add Workers\n";
        cout << "2. View Worker Reports\n";
        cout << "3. View All Workers\n";
        cout << "4. Calculate Total Monthly Payment for All Workers\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            workerManagement.addWorkers();
            break;
        case 2:
            workerManagement.printWorkerReport();
            break;
        case 3:
            workerManagement.printAllWorkersReport();  // Combined report for all workers
            break;
        case 4:
            workerManagement.calculateTotalMonthlyPayment();
            break;
        case 5:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}
