---

## 📘 Log Message Types – Usage Guidelines

### Overview

This document defines the five logging levels used in the project and provides guidance on when to use each type of log message.

---

### 🔴 `CRITICAL`

**Use when:** The application encounters an unrecoverable error and must stop or cannot continue normal operation.

**Examples:**

* Failure to bind to a required network port.
* Missing essential configuration file.
* Uncaught fatal exception.

**Impact:** System shutdown, alert required.

```cpp
log(Log::CRITICAL, "Failed to start server: unable to bind to port.");
```

---

### 🔶 `ERROR`

**Use when:** An error occurs, but the application can **continue running**. These are serious issues but potentially recoverable.

**Examples:**

* Cannot connect to the database but will retry.
* Failure to write to a non-critical file.
* Corrupted input from a client connection.

**Impact:** Operation failed, may retry or fallback.

```cpp
log(Log::ERROR, "Database query failed, using fallback data.");
```

---

### ⚠️ `WARNING`

**Use when:** Something unexpected happened, but it's **not yet an error**. May indicate a future problem if not addressed.

**Examples:**

* Deprecated API used.
* High memory usage approaching limits.
* Configuration file missing optional section.

**Impact:** No failure, but potential risk.

```cpp
log(Log::WARNING, "Using default timeout value; config entry missing.");
```

---

### 🟢 `INFO`

**Use when:** You want to record **normal operations or important events** that are part of regular flow.

**Examples:**

* Server started successfully.
* User connected or disconnected.
* Periodic status update.

**Impact:** Informational only, no action required.

```cpp
log(Log::INFO, "New client connected from 192.168.1.10.");
```

---

### 🔍 `DEBUG`

**Use when:** You need detailed, low-level information for debugging during development or investigation.

**Examples:**

* Variable values at specific points.
* Entering and exiting functions.
* Internal state changes.

**Impact:** Verbose output, usually disabled in production.

```cpp
log(Log::DEBUG, "Parsing header: version=1, length=128.");
```

---

### Summary Table

| Level      | Description                         | Should the App Continue? | Use in Production?      |
| ---------- | ----------------------------------- | ------------------------ | ----------------------- |
| `CRITICAL` | Unrecoverable failure               | ❌ No                     | ✅ Yes (rare)            |
| `ERROR`    | Serious issue, but recoverable      | ✅ Yes                    | ✅ Yes                   |
| `WARNING`  | Unexpected but non-fatal issue      | ✅ Yes                    | ✅ Yes                   |
| `INFO`     | Regular operation messages          | ✅ Yes                    | ✅ Yes                   |
| `DEBUG`    | Developer-only detailed diagnostics | ✅ Yes                    | ❌ No (unless debugging) |

---