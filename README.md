# HAL lib (WIP)
Header only hal lib for stm32f411 mcu.

✨ Features:
 - Include only
 - Designed with c++ and type-safety in mind without sacrifising performance (tested and compared)
 - Templated implementation

📦 Requirements:
 - arm-none-eabi-gcc, arm-none-eabi-g++ compiler
 - openocd or stlink tools for flashing, debugging
 - still requires CMSIS library which you can obtain from example folder or from official website

🚀 Starting point:
 - open example folder
 - create "build" directory in your project folder
 - open "build" folder
 - run: ```cmake .. -DCMAKE_C_COMPILER=arm-none-eabi-gcc -DCMAKE_CXX_COMPILER=arm-none-eabi-g++ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY ```
 - run: ```cmake --build . ```

📌 Roadmap:
 - [x] Implement base for building Registers and Register masks
 - [x] Add initial peripheral implementation
 - [x] Add blink example
 - [ ] Implement fully all Peripherals
 - [ ] Add benchmarking examples
 - [ ] Add more examples

