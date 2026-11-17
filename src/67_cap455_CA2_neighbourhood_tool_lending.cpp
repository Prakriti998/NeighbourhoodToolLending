#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <cctype>
using namespace std;

const int MAX_ID = 20;
const int MAX_NAME = 50;
const int MAX_CATEGORY = 30;
const int MAX_TYPE = 20;

const string FILE_NAME = "tools.dat";
const string TEXT_FILE_NAME = "tools_data.txt";

// Fixed-size record used for binary file storage
struct ToolRecord
{
    char toolID[MAX_ID];
    char toolName[MAX_NAME];
    char category[MAX_CATEGORY];
    char toolType[MAX_TYPE];
};

// Copy a string into a fixed-size char array safely
void copyField(char *dest, const string &src, int maxLen)
{
    strncpy(dest, src.c_str(), maxLen - 1);
    dest[maxLen - 1] = '\0';
}

class Tool
{
private:
    string toolID, toolName, category, toolType;
    static int totalTools;

public:
    // Default constructor
    Tool() {}

    // Parameterized constructor
    Tool(string id, string name, string cat, string type)
        : toolID(id), toolName(name), category(cat), toolType(type) {}

    // Copy constructor
    Tool(const Tool &t)
        : toolID(t.toolID), toolName(t.toolName),
          category(t.category), toolType(t.toolType) {}

    // Destructor
    ~Tool() {}

    // Inline function
    inline string getToolID() const { return this->toolID; }

    // Set tool details using this pointer
    void setToolDetails(string id, string name, string cat, string type)
    {
        this->toolID = id;
        this->toolName = name;
        this->category = cat;
        this->toolType = type;
    }

    // Static functions
    static void increaseTotalTools() { totalTools++; }
    static int getTotalTools() { return totalTools; }
    static void setTotalTools(int total) { totalTools = total; }

    // Convert Tool object into fixed-size record
    void toRecord(ToolRecord &r) const
    {
        copyField(r.toolID, toolID, MAX_ID);
        copyField(r.toolName, toolName, MAX_NAME);
        copyField(r.category, category, MAX_CATEGORY);
        copyField(r.toolType, toolType, MAX_TYPE);
    }

    // Friend function
    friend void displayTool(const Tool &t);
};

int Tool::totalTools = 0;

// Friend function to display tool details
void displayTool(const Tool &t)
{
    cout << "\n-----------------------------";
    cout << "\nTool ID       : " << t.toolID;
    cout << "\nTool Name     : " << t.toolName;
    cout << "\nCategory      : " << t.category;
    cout << "\nTool Type     : " << t.toolType;
    cout << "\n-----------------------------";
}

// ======================= VALIDATION HELPERS =======================

// Remove leading and trailing spaces
string trim(const string &s)
{
    size_t start = 0, end = s.length();
    while (start < end && isspace(static_cast<unsigned char>(s[start]))) start++;
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}

// Tool ID: letters, digits, '-' and '_' only (no spaces)
bool isValidID(const string &s)
{
    for (size_t i = 0; i < s.length(); i++)
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (!isalnum(c) && c != '-' && c != '_') return false;
    }
    return true;
}

// Name/Category: must start with a letter; letters, spaces, hyphens
// (and digits if allowed); no consecutive spaces
bool isValidText(const string &s, bool allowDigits)
{
    if (s.empty() || !isalpha(static_cast<unsigned char>(s[0]))) return false;
    for (size_t i = 0; i < s.length(); i++)
    {
        unsigned char c = static_cast<unsigned char>(s[i]);
        bool ok = isalpha(c) || c == ' ' || c == '-' || (allowDigits && isdigit(c));
        if (!ok || (c == ' ' && s[i - 1] == ' ')) return false;
    }
    return true;
}

// Keep asking until a valid name/category is entered
string inputText(const string &prompt, const string &label, int maxLen,
                 bool allowDigits, const string &invalidMsg)
{
    string text;
    while (true)
    {
        cout << prompt;
        getline(cin, text);
        text = trim(text);

        if (text == "")
            cout << label << " cannot be empty. Try again.\n";
        else if (text.length() >= (size_t)maxLen)
            cout << label << " must be shorter than " << maxLen << " characters.\n";
        else if (!isValidText(text, allowDigits))
            cout << invalidMsg;
        else
            return text;
    }
}

string inputToolName(const string &prompt)
{
    return inputText(prompt, "Tool name", MAX_NAME, true,
        "Invalid tool name! It must start with a letter and contain only "
        "letters, digits, spaces or hyphens (no double spaces).\n");
}

string inputCategory(const string &prompt)
{
    return inputText(prompt, "Category", MAX_CATEGORY, false,
        "Invalid category! Use only letters, spaces or hyphens "
        "(no digits or symbols).\n");
}

// Keep asking until a valid tool type is selected
string inputToolType(const string &heading)
{
    string type;
    while (true)
    {
        cout << "\n" << heading;
        cout << "\n1. Hand Tool";
        cout << "\n2. Power Tool";
        cout << "\nEnter your choice: ";
        getline(cin, type);
        type = trim(type);

        if (type == "1") return "Hand Tool";
        if (type == "2") return "Power Tool";
        cout << "Invalid choice! Enter 1 or 2.\n";
    }
}

// ==================================================================

// Create a Tool object from a binary record
Tool recordToTool(const ToolRecord &r)
{
    Tool tool;
    tool.setToolDetails(r.toolID, r.toolName, r.category, r.toolType);
    return tool;
}

// Read the record at a given index using seekg()
bool readRecord(int index, ToolRecord &record)
{
    ifstream file(FILE_NAME.c_str(), ios::binary);
    if (!file) return false;
    file.seekg(index * sizeof(ToolRecord), ios::beg);
    return static_cast<bool>(file.read(reinterpret_cast<char*>(&record),
                                       sizeof(ToolRecord)));
}

// Find a tool using its ID
int findToolIndex(string id)
{
    ToolRecord record;
    for (int index = 0; readRecord(index, record); index++)
    {
        if (id == record.toolID) return index;
    }
    return -1;
}

// Load total number of records when program starts
void loadTotalTools()
{
    ifstream file(FILE_NAME.c_str(), ios::binary | ios::ate);
    if (!file)
    {
        Tool::setTotalTools(0);
        return;
    }
    streamoff fileSize = file.tellg();
    Tool::setTotalTools(static_cast<int>(fileSize / sizeof(ToolRecord)));
}

// Add a new tool to the binary file
void addTool()
{
    string id;
    cout << "\n---- ADD NEW TOOL ----";

    // Enter and validate Tool ID
    while (true)
    {
        cout << "\nEnter Tool ID: ";
        getline(cin, id);
        id = trim(id);

        if (id == "")
            cout << "Tool ID cannot be empty. Try again.\n";
        else if (id.length() >= MAX_ID)
            cout << "Tool ID must be shorter than 20 characters.\n";
        else if (!isValidID(id))
            cout << "Invalid Tool ID! Use only letters, digits, '-' or '_' (no spaces).\n";
        else if (findToolIndex(id) != -1)
            cout << "This Tool ID already exists!\n";
        else
            break;
    }

    string name     = inputToolName("Enter Tool Name: ");
    string category = inputCategory("Enter Tool Category: ");
    string type     = inputToolType("Select Tool Type:");

    Tool newTool(id, name, category, type); // Parameterized constructor
    Tool copiedTool(newTool);               // Copy constructor

    ToolRecord record;
    copiedTool.toRecord(record);

    // Write record at the end of binary file
    ofstream file(FILE_NAME.c_str(), ios::binary | ios::app);
    if (!file)
    {
        cout << "\nUnable to open the data file!\n";
        return;
    }

    file.write(reinterpret_cast<char*>(&record), sizeof(ToolRecord));
    if (!file)
    {
        cout << "\nError while saving tool details!\n";
        return;
    }
    file.close();

    Tool::increaseTotalTools();
    cout << "\nTool added and saved successfully!";
    cout << "\nTotal Number of Tools: " << Tool::getTotalTools() << "\n";
}

// Display all tools from binary file
void viewAllTools()
{
    ifstream file(FILE_NAME.c_str(), ios::binary);
    cout << "\n---- ALL AVAILABLE TOOLS ----";

    if (!file)
    {
        cout << "\nNo tools have been added yet.\n";
        return;
    }

    ToolRecord record;
    int count = 0;

    while (file.read(reinterpret_cast<char*>(&record), sizeof(ToolRecord)))
    {
        count++;
        cout << "\nTool Number: " << count;
        displayTool(recordToTool(record));
    }

    if (count == 0)
        cout << "\nNo tools have been added yet.\n";
    cout << "\nTotal Number of Tools: " << count << "\n";
}

// Search for a tool using its ID
void searchTool()
{
    string id;
    cout << "\n---- SEARCH TOOL ----";
    cout << "\nEnter Tool ID to search: ";
    getline(cin, id);
    id = trim(id);

    int index = findToolIndex(id);
    if (index == -1)
    {
        cout << "\nTool not found!\n";
        return;
    }

    ToolRecord record;
    if (readRecord(index, record))
    {
        cout << "\nTool found successfully!";
        displayTool(recordToTool(record));
    }
    else
    {
        cout << "\nUnable to read the tool record!\n";
    }
}

// Update existing tool details using seekp()
void updateTool()
{
    string id;
    cout << "\n---- UPDATE TOOL ----";
    cout << "\nEnter Tool ID to update: ";
    getline(cin, id);
    id = trim(id);

    int index = findToolIndex(id);
    if (index == -1)
    {
        cout << "\nTool not found! Cannot update.\n";
        return;
    }

    ToolRecord oldRecord;
    if (!readRecord(index, oldRecord))
    {
        cout << "\nUnable to read the existing record!\n";
        return;
    }

    cout << "\nCurrent Tool Details:";
    displayTool(recordToTool(oldRecord));

    cout << "\n";
    string name     = inputToolName("Enter New Tool Name: ");
    string category = inputCategory("Enter New Category: ");
    string type     = inputToolType("Select New Tool Type:");

    Tool updatedTool(id, name, category, type);
    ToolRecord updatedRecord;
    updatedTool.toRecord(updatedRecord);

    // Open file for both reading and writing
    fstream file(FILE_NAME.c_str(), ios::in | ios::out | ios::binary);
    if (!file)
    {
        cout << "\nUnable to open file for updating!\n";
        return;
    }

    // Move output pointer to the selected record and replace it
    file.seekp(index * sizeof(ToolRecord), ios::beg);
    file.write(reinterpret_cast<char*>(&updatedRecord), sizeof(ToolRecord));

    if (file)
        cout << "\nTool details updated successfully!\n";
    else
        cout << "\nError while updating tool details!\n";
}

// Convert binary data into a readable text file (called on exit)
void exportBinaryToText()
{
    ifstream inputFile(FILE_NAME.c_str(), ios::binary);
    ofstream textFile(TEXT_FILE_NAME.c_str());

    if (!textFile)
    {
        cout << "\nUnable to create " << TEXT_FILE_NAME << "!\n";
        return;
    }

    textFile << "NEIGHBOURHOOD TOOL-LENDING SYSTEM\n";
    textFile << "=================================\n";

    if (!inputFile)
    {
        textFile << "No tools added yet.\n";
        return;
    }

    ToolRecord record;
    int count = 0;

    while (inputFile.read(reinterpret_cast<char*>(&record), sizeof(ToolRecord)))
    {
        count++;
        textFile << "\nRecord Number: " << count << "\n";
        textFile << "Tool ID: " << record.toolID << "\n";
        textFile << "Tool Name: " << record.toolName << "\n";
        textFile << "Category: " << record.category << "\n";
        textFile << "Tool Type: " << record.toolType << "\n";
        textFile << "---------------------------------\n";
    }

    textFile << "\nTotal Records: " << count << "\n";
    cout << "\nData exported to " << TEXT_FILE_NAME << " successfully.";
}

// Main function
int main()
{
    loadTotalTools();
    string choice;

    while (true)
    {
        cout << "\n\n--------------------------------";
        cout << "\n NEIGHBOURHOOD TOOL-LENDING SYSTEM";
        cout << "\n--------------------------------";
        cout << "\n1. Add Tool";
        cout << "\n2. View All Tools";
        cout << "\n3. Search Tool";
        cout << "\n4. Update Tool";
        cout << "\n5. Exit";
        cout << "\n\nEnter your choice: ";
        getline(cin, choice);
        choice = trim(choice);

        if (choice == "1") addTool();
        else if (choice == "2") viewAllTools();
        else if (choice == "3") searchTool();
        else if (choice == "4") updateTool();
        else if (choice == "5")
        {
            exportBinaryToText(); // Convert binary file to text file on exit
            cout << "\nThank you for using the system!";
            cout << "\nProgram closed successfully.\n";
            break;
        }
        else cout << "\nInvalid choice! Please select 1 to 5.\n";
    }
    return 0;
}