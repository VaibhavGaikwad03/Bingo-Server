Certainly! Here’s a **step-by-step guide** that worked for you to install and set up **uWebSockets** on Linux, specifically for building and running the example WebSocket server.

---

### **Steps to Install and Run uWebSockets on Linux**

---

### **1. Install Dependencies**

Before starting, make sure the essential packages are installed:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake zlib1g-dev
```

---

### **2. Clone uWebSockets Repository**

Clone the **uWebSockets** repository from GitHub:

```bash
git clone https://github.com/uNetworking/uWebSockets.git
cd uWebSockets
```

---

### **3. Build uSockets Library**

Navigate to the `uSockets` directory and build the static library `uSockets.a`:

1. Clean any previous builds:

   ```bash
   make clean
   ```

2. Build the static library:

   ```bash
   make
   ```

   After this, you should see `uSockets.a` in the `uSockets` directory.

---

### **4. Install the uSockets Library**

Manually copy the built static library and header files to appropriate system locations:

1. Copy the `uSockets.a` library to `/usr/local/lib/`:

   ```bash
   sudo cp uSockets.a /usr/local/lib/
   ```

2. Copy the header files from the `src` folder to `/usr/local/include/`:

   ```bash
   sudo cp -r src/*.h /usr/local/include/
   ```

3. Update the linker cache:

   ```bash
   sudo ldconfig
   ```

---

### **5. Install zlib (For Compression)**

To resolve missing references for compression functions (`deflateEnd`, `inflateEnd`), install the **zlib** development libraries:

```bash
sudo apt-get install zlib1g-dev
```

---

### **6. Compile the Example (`HelloWorld.cpp`)**

Now that everything is set up, go to the `examples` folder and compile the example `HelloWorld.cpp`:

1. Navigate to the `examples` directory:

   ```bash
   cd ~/Desktop/uWebSockets/examples
   ```

2. Compile the example using `g++`, linking the required libraries:

   ```bash
   g++ -o hello_world HelloWorld.cpp -I/usr/local/include -L/usr/local/lib /usr/local/lib/uSockets.a -lz
   ```

    * `-I/usr/local/include`: Includes the headers from `/usr/local/include`.
    * `-L/usr/local/lib`: Specifies the library path for the linker.
    * `/usr/local/lib/uSockets.a`: Specifies the full path to the `uSockets.a` library.
    * `-lz`: Links against **zlib** for compression.

---

### **7. Run the WebSocket Server**

After successful compilation, run the `hello_world` WebSocket server:

```bash
./hello_world
```

The output should show:

```bash
Listening on port 3000
```

---

### **8. Test the WebSocket Server**

You can now connect to your WebSocket server at `ws://localhost:3000` using a WebSocket client, browser, or testing tools.

---

### **Summary of Key Steps:**

* Install dependencies: `build-essential`, `cmake`, `zlib1g-dev`
* Clone and build the `uWebSockets` repository.
* Install the library by copying the built files (`uSockets.a` and headers) to `/usr/local/lib/` and `/usr/local/include/`.
* Install zlib for compression support.
* Compile and link the example WebSocket server (`HelloWorld.cpp`) with `g++`.
* Run the WebSocket server on port 3000.

---

### Notes:

* If you encounter the **"undefined reference to `deflateEnd`"** or **"inflateEnd"** errors, make sure **zlib** is installed and link it with `-lz`.
* If the linker can’t find `uWebSockets`, provide the full path to `uSockets.a`.

---

This should serve as a nice reference for setting up **uWebSockets** in the future. If you need further refinements, feel free to ask!
