# Movie Ticket Console

## Project Description

Movie Ticket Console is a console-based cinema ticket booking project written in C. It is designed to simulate ticket-buying behavior in a terminal application, with separate flows for customers and managers.

The project currently includes:

- authentication and role-based menu scaffolding
- customer service modules for viewing movies, searching, booking, and ticket history
- manager service modules for movie and screening management
- CSV data files for movies, screenings, tickets, and users

This repository is structured as a foundation for a larger movie ticket management system and is built with CMake.

## Requirements

- GCC compiler
- CMake
- Ninja build system
- IDE (VS Code is highly recommended)

## Build and Run on Windows

1. Clone the repository:

```powershell
git clone https://github.com/CQ-66-CNTT-Unknown/movie-ticket-console.git
cd movie-ticket-console
```

2. Run the PowerShell build script:
- Normal build
```powershell
./build.ps1
```
- Clean build
```powershell
./build.ps1 -Clean
```
- Build then Run
```powershell
./build.ps1 -Run
```

3. After the build completes, open the project in your IDE or run the generated executable from the build folder.

## Languages

- C
- PowerShell
- CMake

## Author

- TTNguyen2505
- trantrongnguyen2505@outlook.com