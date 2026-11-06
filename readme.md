# Neighbourhood Tool-Lending System

**Course:** CAP455 – Object-Oriented Programming using C++ (MCA, Session 2026–27)
**Student:** [Your Name] | **Roll No.:** [Your Roll No.] | **Section:** [Section]
**Faculty:** [Faculty Name]
**Current phase:** CA1 – Design & Create (Units I–II)

---

## 1. About the Project

Neighbours often own tools (hammers, drills, spanners) that others could borrow, but nobody knows who has what. This console application keeps an organised catalogue of the tools available in a neighbourhood.

In CA1 the system supports the **Create** and **Read** operations on an in-memory list of tools, with input validation and duplicate-key prevention. Later phases add file storage, search, lending, update/delete and more.

- **Main entity / class:** `Tool`
- **Unique key:** `toolID`
- **Storage (CA1):** fixed array `Tool tools[100]` (data is not saved after exit)

---

## 2. Features by Phase

| Phase | Status | Features |
|---|---|---|
| **CA1 – Design & Create** | Done | `Tool` class with static, inline and friend members, `this` pointer, `std::string`; array of objects; pointer traversal; Add Tool; View All Tools; validation; no duplicate IDs; capacity check (max 100) |
| **CA2 – Persist & Search** | Planned | Default, parameterised and copy constructors; destructor; binary fixed-size record file; search with `seekg`; basic update with `seekp`; data survives restart |
| **CA3 – Reuse & Delete** | Planned | Base class + at least 2 derived classes; operator overloading (`<<`, `>>`, `==`, `<` + one more); type conversion; `new`/`delete`; virtual functions and virtual destructor; safe update and delete with confirmation |
| **CA4 – Integrate & Defend** | Planned | Full CRUD menu; `try`/`catch`/`throw` with custom exception class; function and class templates; STL `vector`/`list` with `sort`, `find_if`, `count_if`; at least one advanced feature (backup, CSV export, undo stack, etc.) |

---

## 3. CA1 Features in Detail

### Menu

```
NEIGHBOURHOOD TOOL-LENDING SYSTEM
1. Add Tool
2. View All Tools
3. Exit
```

### Add Tool
Prompts for Tool ID, Name, Category and Type (1 = Hand Tool, 2 = Power Tool).

| Input | Validation |
|---|---|
| Tool ID | Not empty; must be unique |
| Tool Name | Not empty |
| Category | Not empty |
| Tool Type | Only `1` or `2` accepted |
| Storage | Stops with a message when 100 tools are stored |

### View All Tools
Displays every stored tool and the total count. Shows a friendly message if no tools have been added yet.

### OOP Concepts Used

| Concept | Where |
|---|---|
| Encapsulation | Private data members in `Tool` |
| Default constructor | `Tool()` |
| Static data member and functions | `totalTools`, `increaseTotalTools()`, `getTotalTools()` |
| Inline function | `getToolID()` |
| `this` pointer | `setToolDetails()`, `getToolID()` |
| Friend function | `displayTool()` |
| Array of objects | `Tool tools[MAX_TOOLS]` |
| Pointers / pointer arithmetic | `Tool *ptr = tools; *(ptr + i)` in `viewAllTools()` |
| `std::string` | All text fields and input |

---

## 4. Requirements

- A standard C++ compiler with C++11 support or later (g++, clang++ or MSVC)
- Any terminal / command prompt

---

## 5. Build and Run

**Linux / macOS / Windows (MinGW):**

```bash
cd src
g++ -std=c++11 -Wall -o toollending ToolLending.cpp
./toollending          # on Windows: toollending.exe
```

**Windows (Visual Studio Developer Command Prompt):**

```bat
cd src
cl /EHsc ToolLending.cpp
ToolLending.exe
```

---

## 6. Sample Run

```
Enter your choice: 1

---- ADD NEW TOOL ----
Enter Tool ID: T101
Enter Tool Name: Hammer
Enter Tool Category: Carpentry

Select Tool Type:
1. Hand Tool
2. Power Tool
Enter your choice: 1

Tool added successfully!
Total Number of Tools: 1
```

```
Enter your choice: 2

---- ALL AVAILABLE TOOLS ----
-----------------------------
Tool ID       : T101
Tool Name     : Hammer
Category      : Carpentry
Tool Type     : Hand Tool
-----------------------------
Total Number of Tools: 1
```

### Sample data for testing

| Tool ID | Name | Category | Type |
|---|---|---|---|
| T101 | Hammer | Carpentry | Hand Tool (1) |
| T102 | Drill | Electrical | Power Tool (2) |
| T103 | Spanner Set | Plumbing | Hand Tool (1) |
| T104 | Electric Saw | Carpentry | Power Tool (2) |

> CA1 stores data in memory only. File-based sample data (`/data`) will be added in CA2.

---

## 7. Testing

Test cases cover normal, invalid and boundary inputs, for example:

- **Normal:** add valid tools, view all tools
- **Invalid:** empty ID/name/category, duplicate ID, tool type `3` or `abc`, wrong menu choice
- **Boundary:** view with no records, add exactly 100 tools, attempt the 101st

The full test table is in the CA1 report (`/docs`), with screenshots in `/tests`.

---

## 8. Repository Layout

```
/docs     proposal, data dictionary, UML, flowchart, CA1 report
/src      ToolLending.cpp
/data     sample data files (used from CA2 onwards)
/tests    test table and output screenshots
README.md
```

Submission files follow the naming rule `<RollNo>_CAP455_<Phase>_<Title>`, e.g. `[RollNo]_CAP455_CA1_ToolLending`.

---

## 9. Known Limitations (CA1)

- Data is lost when the program exits (fixed in CA2).
- No search, update or delete yet.
- Maximum capacity is 100 tools.
- Whitespace-only input is accepted as non-empty.
- Lending and borrower records are not modelled yet.

---

## 10. Academic Integrity

This is individual work. Any books, online documentation or AI tools used for learning and clarification are disclosed in the project report. I can explain and modify every line of the submitted code.
