---

## 📘 C++ Project Naming Convention Guide

**Project Goal:** Maintain consistent, readable, and industry-standard naming throughout the codebase.

---

### 📌 1. General Principles

* Use **descriptive**, **clear**, and **concise** names.
* Avoid abbreviations unless commonly accepted (`id`, `url`, `xml`, etc.).
* Follow **C++ standard library** and **modern C++ community** conventions.
* Keep naming consistent across all modules.

---

### 📦 2. Naming by Element Type

| Element Type                  | Convention                                     | Example                                       |
| ----------------------------- | ---------------------------------------------- | --------------------------------------------- |
| **Namespace**                 | `snake_case`                                   | `network_utils`, `math_helpers`               |
| **Class / Struct**            | `PascalCase`                                   | `DataPacket`, `UserInfo`                      |
| **Function**                  | `snake_case`                                   | `send_message()`, `load_file()`               |
| **Variable (local/member)**   | `snake_case`                                   | `packet_id`, `user_name`                      |
| **Member Variable (private)** | `snake_case + _`                               | `data_`, `count_`                             |
| **Constant**                  | `SCREAMING_SNAKE_CASE`                         | `MAX_RETRIES`, `DEFAULT_PORT`                 |
| **Macro**                     | `SCREAMING_SNAKE_CASE`                         | `LOG_DEBUG`, `ASSERT_TRUE`                    |
| **File Name**                 | `snake_case`                                   | `data_packet.cpp`, `log_utils.hpp`            |
| **Header Guard**              | `SCREAMING_SNAKE_CASE` with full path          | `NETWORK_UTILS_DATA_PACKET_HPP`               |
| **Enum**                      | `PascalCase`, values in `SCREAMING_SNAKE_CASE` | `enum class Status { OK, ERROR_OCCURRED }`    |
| **Template Parameter**        | `PascalCase` (short or descriptive)            | `template <typename T>`, `Key`, `Value`       |
| **Global Variable**           | `g_` prefix + `snake_case`                     | `g_log_file`, `g_config_path`                 |
| **Static Local Variable**     | `s_` prefix + `snake_case`                     | `s_instance`, `s_retry_limit`                 |
| **Type Alias**                | `PascalCase`                                   | `using StringList = std::vector<std::string>` |

---

### 🧱 3. Class Naming

* Use **nouns or noun phrases**.
* Use `PascalCase`.
* Example:

  ```cpp
  class DataPacket {
  public:
      void send_message() const;
  private:
      int packet_id_;
  };
  ```

---

### 🔧 4. Function Naming

* Use **verbs or verb phrases**.
* Use `snake_case`.
* Prefer short but descriptive names.
* Example:

  ```cpp
  void send_message();
  int read_file_data(const std::string& path);
  ```

---

### 📄 5. Variable Naming

* Use `snake_case` for all variables.
* Prefix **private class members** with an underscore suffix.
* Prefix **global** variables with `g_`, static locals with `s_`.
* Examples:

  ```cpp
  int retry_count = 0;
  std::string content_;
  static int s_retry_limit = 5;
  std::string g_log_path;
  ```

---

### 🔠 6. Constant and Macro Naming

* Use **`SCREAMING_SNAKE_CASE`**.
* Example:

  ```cpp
  constexpr int MAX_BUFFER_SIZE = 1024;
  #define ENABLE_LOGGING
  ```

---

### 🎛️ 7. Enum and Template Naming

* Enums use `PascalCase`, values in `SCREAMING_SNAKE_CASE`.
* Template parameters: one-letter `T`, `U`, or descriptive like `Key`, `Value`.

```cpp
enum class LogLevel {
    DEBUG,
    INFO,
    ERROR
};

template <typename T>
class Container {};
```

---

### 📁 8. File Naming

* Use lowercase `snake_case` with `.cpp` and `.hpp`.
* Group related headers into subdirectories.
* Example:

  ```
  include/network_utils/data_packet.hpp
  src/data_packet.cpp
  ```

---

### 🧾 9. Type Aliases

* Use `PascalCase` for clarity.
* Prefer `using` over `typedef`.

```cpp
using StringMap = std::map<std::string, std::string>;
```

---

### 📚 10. Project Layout (Suggested)

```
my_project/
├── include/
│   └── module_name/
│       └── class_name.hpp
├── src/
│   └── class_name.cpp
├── main.cpp
├── CMakeLists.txt
```

---

### ✅ Summary

| Feature       | Style            |
| ------------- | ---------------- |
| Readability   | ✅ High           |
| Community Fit | ✅ Excellent      |
| Scalability   | ✅ Easy to extend |

---