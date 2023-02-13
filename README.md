## Introduction:  
  * This is the final project of B09902005 and B09901133.
  * This project simulates a better parking lot for intelligence vehicles, and it can run on MacOS and Linux.
  * To see the details of this project, you can check https://github.com/B09902005/parking/blob/master/Intelligent_Vehicle_Final_Report.pdf for more information.

## Demo:  
  You can check https://youtu.be/PcYCgYxBmFA and https://www.youtube.com/watch?v=Z2qO128PWHM for demo.

## How to run the simulation:  
1. First, you need to download the environment:
    - Linux : 
    ```
    sudo apt-get install make g++
    sudo add-apt-repository ppa:allegro/5.2
    sudo apt-get install liballegro*5.2 liballegro*5-dev 
    ```
    - MacOS : 
    ```
    brew install allegro
    brew install pkg-config
    ```
2. Second, we can change directory into the the folder "parking" and compile everything:
   ```
   make
   ```
3. Finally, run the simulation.
    ```
    ./bin/main
    ```

## References:  
  https://github.com/liballeg/allegro_wiki/wiki/Quickstart
