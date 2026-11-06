#include <iostream>
#include <string>
using namespace std;

// Maximum number of tools that can be stored in the array
const int MAX_TOOLS = 100;
class Tool
{
private:
    string toolID;
    string toolName;
    string category;
    string toolType;

    // Static data member shared by all objects
    static int totalTools;
public:

    // Default constructor
    Tool()
    {
        toolID = "";
        toolName = "";
        category = "";
        toolType = "";
    }
    // Inline function to return Tool ID
    inline string getToolID() const
    {
        return this->toolID;
    }
    // Function to store tool details using this pointer
    void setToolDetails(string id, string name,
                        string cat, string type)
    {
        this->toolID = id;
        this->toolName = name;
        this->category = cat;
        this->toolType = type;
    }
    // Static function to increase total tools
    static void increaseTotalTools()
    {
        totalTools++;
    }

    // Static function to return total tools
    static int getTotalTools()
    {
        return totalTools;
    }

    // Friend function to display tool details
    friend void displayTool(const Tool &t);
};

// Definition of static data member
int Tool::totalTools = 0;

// Friend function
void displayTool(const Tool &t)
{
    cout << "\n-----------------------------";
    cout << "\nTool ID       : " << t.toolID;
    cout << "\nTool Name     : " << t.toolName;
    cout << "\nCategory      : " << t.category;
    cout << "\nTool Type     : " << t.toolType;
    cout << "\n-----------------------------";
}

// Function to check whether Tool ID already exists
bool isDuplicateID(Tool tools[], int count, string id)
{
    for (int i = 0; i < count; i++)
    {
        if (tools[i].getToolID() == id)
        {
            return true;
        }
    }
    return false;
}

// Function to add a new tool
void addTool(Tool tools[], int &count)
{
    if (count >= MAX_TOOLS)
    {
        cout << "\nTool storage is full!";
        return;
    }
    string id, name, category, type;
    cout << "\n---- ADD NEW TOOL ----";

    // Enter and validate Tool ID
    while (true)
    {
        cout << "\nEnter Tool ID: ";
        getline(cin, id);
        if (id == "")
        {
            cout << "Tool ID cannot be empty. Try again.\n";
        }
        else if (isDuplicateID(tools, count, id))
        {
            cout << "This Tool ID already exists! Enter a unique ID.\n";
        }
        else
        {
            break;
        }
    }

    // Enter and validate Tool Name
    while (true)
    {
        cout << "Enter Tool Name: ";
        getline(cin, name);

        if (name == "")
        {
            cout << "Tool name cannot be empty. Try again.\n";
        }
        else
        {
            break;
        }
    }

    // Enter and validate Category
    while (true)
    {
        cout << "Enter Tool Category: ";
        getline(cin, category);
        if (category == "")
        {
            cout << "Category cannot be empty. Try again.\n";
        }
        else
        {
            break;
        }
    }

    // Enter and validate Tool Type
    while (true)
    {
        cout << "\nSelect Tool Type:";
        cout << "\n1. Hand Tool";
        cout << "\n2. Power Tool";
        cout << "\nEnter your choice: ";
        getline(cin, type);
        if (type == "1")
        {
            type = "Hand Tool";
            break;
        }
        else if (type == "2")
        {
            type = "Power Tool";
            break;
        }
        else
        {
            cout << "Invalid choice! Please enter 1 or 2.\n";
        }
    }

    // Store details in the array
    tools[count].setToolDetails(id, name, category, type);

    // Increase the number of tools
    count++;

    // Update static total
    Tool::increaseTotalTools();
    cout << "\nTool added successfully!";
    cout << "\nTotal Number of Tools: "
         << Tool::getTotalTools() << "\n";
}

// Function to display all tools
void viewAllTools(Tool tools[], int count)
{
    cout << "\n---- ALL AVAILABLE TOOLS ----";
    if (count == 0)
    {
        cout << "\nNo tools have been added yet.\n";
        return;
    }

    // Pointer pointing to the first object of the array
    Tool *ptr = tools;
    for (int i = 0; i < count; i++)
    {

        // Access objects using pointer arithmetic
        displayTool(*(ptr + i));
    }
    cout << "\nTotal Number of Tools: "
         << Tool::getTotalTools() << "\n";
}

// Main function
int main()
{
    // Array of Tool objects
    Tool tools[MAX_TOOLS];
    int count = 0;
    string choice;
    while (true)
    {
        cout << "\n\n--------------------------------";
        cout << "\n NEIGHBOURHOOD TOOL-LENDING SYSTEM";
        cout << "\n----------------------------------";

        cout << "\n1. Add Tool";
        cout << "\n2. View All Tools";
        cout << "\n3. Exit";

        cout << "\n\nEnter your choice: ";
        getline(cin, choice);

        if (choice == "1")
        {
            addTool(tools, count);
        }
        else if (choice == "2")
        {
            viewAllTools(tools, count);
        }
        else if (choice == "3")
        {
            cout << "\nThank you for using the system!";
            cout << "\nProgram closed successfully.\n";
            break;
        }
        else
        {
            cout << "\nInvalid choice! Please select 1, 2 or 3.\n";
        }
    }
    return 0;
}