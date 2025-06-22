# Activity Logging System

This document describes the activity logging system that tracks order activities in the game.

## Overview

The activity logging system automatically logs all order-related activities to `activity.log` file, including:

- Order creation
- Order execution  
- Order deletion
- Person removal from cities
- Person return to cities
- Game turns
- Computer tactic generation

## Logging Filter Control

The system includes a global flag `g_LogOnlyUserKing` that controls whether order-related logging only occurs for activities involving the user's king (player's faction).

### Default Behavior
- **Enabled by default**: Only logs activities where the person belongs to the user's king
- This reduces log noise by filtering out AI/computer activities
- Game turns and computer tactics are still logged regardless of the flag

### Control Functions

#### C++ Functions
```cpp
// Set the logging filter
void setLogOnlyUserKing(bool enabled);

// Get current filter status
bool getLogOnlyUserKing();
```

#### C Wrapper Functions
```cpp
// Set the logging filter (0 = disabled, 1 = enabled)
void setLogOnlyUserKingC(int enabled);

// Get current filter status (0 = disabled, 1 = enabled)
int getLogOnlyUserKingC();
```

### Usage Examples

```cpp
// Enable logging for all activities (not just user king)
setLogOnlyUserKing(false);

// Disable logging for non-user king activities (default)
setLogOnlyUserKing(true);

// Check current status
bool isFiltered = getLogOnlyUserKing();
```

## Log File Format

The log file (`activity.log`) uses the following format:

```
[Timestamp] Activity: Details
```

Example entries:
```
[Wed Dec 20 10:30:15 2023] ORDER_CREATED: Person=刘备 (ID:0), Order=开垦 (ID:1), City=成都 (ID:0)
[Wed Dec 20 10:30:16 2023] PERSON_REMOVED: Person=刘备 (ID:0), City=成都 (ID:0), Reason=Order Assignment
[Wed Dec 20 10:30:17 2023] ORDER_EXECUTED: Person=刘备 (ID:0), Order=开垦 (ID:1), City=成都 (ID:0)
[Wed Dec 20 10:30:18 2023] PERSON_RETURNED: Person=刘备 (ID:0), City=成都 (ID:0), Reason=Order Completion
[Wed Dec 20 10:30:19 2023] ORDER_DELETED: Person=刘备 (ID:0), Order=开垦 (ID:1), City=成都 (ID:0)
[Wed Dec 20 10:30:20 2023] GAME_TURN: Year=184, Month=1
[Wed Dec 20 10:30:21 2023] COMPUTER_TACTIC: City=成都 (ID:0), Tactic=Interior
```

## Activity Types

### ORDER_CREATED
Logged when a new order is added to the order queue.
- **Person**: The officer executing the order
- **Order**: The type of order (开垦, 招商, etc.)
- **City**: The city where the order is issued
- **Target**: The target city/person (if different from source city)

### ORDER_EXECUTED  
Logged when an order is executed from the order queue.
- Same details as ORDER_CREATED

### ORDER_DELETED
Logged when an order is removed from the order queue.
- Same details as ORDER_CREATED

### PERSON_REMOVED
Logged when an officer is removed from a city (usually for order assignment).
- **Person**: The officer being removed
- **City**: The city they're being removed from
- **Reason**: Why they're being removed (usually "Order Assignment")

### PERSON_RETURNED
Logged when an officer returns to a city (usually after order completion).
- **Person**: The officer returning
- **City**: The city they're returning to
- **Reason**: Why they're returning (usually "Order Completion")

### GAME_TURN
Logged at the end of each game turn.
- **Year**: Current game year
- **Month**: Current game month

### COMPUTER_TACTIC
Logged when computer AI generates tactics for a city.
- **City**: The city receiving computer tactics
- **Tactic**: Type of tactic (Interior, Diplomatism, Armament, etc.)

## Functions

### Core Logging Functions

```cpp
// Log general activity
void logActivity(const std::string& activity, const std::string& details = "");

// Log order-related activities
void logOrderCreated(const OrderType* order);
void logOrderExecuted(const OrderType* order);
void logOrderDeleted(const OrderType* order);

// Log person-related activities
void logPersonReturned(U8 personId, U8 cityId, const std::string& reason = "");
void logPersonRemoved(U8 personId, U8 cityId, const std::string& reason = "");

// Log game state activities
void logGameTurn();
void logComputerTactic(U8 cityId, const std::string& tacticType);
```

### C Wrapper Functions

For compatibility with C code, C wrapper functions are provided:

```cpp
void logActivityC(const char* activity, const char* details);
void logOrderCreatedC(const OrderType* order);
void logOrderExecutedC(const OrderType* order);
void logPersonReturnedC(U8 personId, U8 cityId, const char* reason);
void logPersonRemovedC(U8 personId, U8 cityId, const char* reason);
void logOrderDeletedC(const OrderType* order);
void logGameTurnC();
void logComputerTacticC(U8 cityId, const char* tacticType);
```

### Test Function

```cpp
void testActivityLogging();
```

Call this function to test the logging system and verify it's working correctly.

## Integration Points

The logging system is integrated into the following key functions:

1. **AddOrderHead()** - Logs order creation
2. **DelOrder()** - Logs order deletion  
3. **PolicyExec()** - Logs order execution
4. **DelPerson()** - Logs person removal
5. **AddPerson()** - Logs person return
6. **GameDevDrv()** - Logs game turns
7. **ComputerTactic()** - Logs computer tactics

## Error Handling

The logging system includes comprehensive error handling:

- File I/O errors are caught and reported to stderr
- Invalid parameters are validated before logging
- Exceptions are caught to prevent crashes
- Error conditions are logged with ERROR_ prefix

## Usage

The logging system is automatically active when the game runs. No additional setup is required.

To test the system:
```cpp
testActivityLogging();
```

To manually log activities:
```cpp
logActivityC("CUSTOM_ACTIVITY", "Custom details");
```

To control logging filter:
```cpp
// Enable logging for all activities
setLogOnlyUserKingC(0);

// Disable logging for non-user king activities (default)
setLogOnlyUserKingC(1);
```

## File Management

- Log file: `activity.log`
- Appends to existing file (doesn't overwrite)
- Timestamps are automatically added
- No log rotation (manual cleanup may be needed for long sessions)

## Dependencies

- Standard C++ libraries (iostream, fstream, string, ctime)
- Game data structures (OrderType, g_Cities, g_Persons)
- GBK to UTF-8 conversion functions
- Game constants (order IDs, city/person limits) 