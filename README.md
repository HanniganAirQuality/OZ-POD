# Oz-POD
Oz-POD is a ozone sensing pod contracted by NASA Ames and the Bay Area Environmental Research Institute (BAERI) for the purpose of in situ verification of satellite measured ozone data as well as Bay Area community STEM outreach. This project was led by Spencer Hoehl (BS '26) and advised by Percy Smith (BS '24, PhD Candidate) for use by Kristen Okorn (NASA Ames) and Emma Yates (BAERI). 
 
# Using this Firmware
USE OzPOD_V3 version

This github repository is updated with the most recent version of the Oz-POD firmware as well as all iterations from the beginning of the development. When pulling code from this repository for the purpose of further development of code or uploading to a particle microcontroller, please follow these steps:

1. Ensure that you have VS code IDE with the particle workbench extension downloaded on your local machine.
2. Copy SSH key and open a new window in VS code.  Select the option in startup menu that says "Clone Git Repository". Paste the repository's SSH key into the VS Code search bar when       
   prompted. Choose the location on your local machine where you plan to store the repository. The repository should open automatically in your VS Code IDE.
4. Navigate to the folder of the most recent version of the firmware within your IDE.  Locate and open the <firmware version>.cpp file within the SRC folder.  Ensure that included library       dependencies are recognized in this folder.  IF THEY ARE NOT RECOGNIZED, follow subsequent steps.  IF THEY ARE RECOGNIZED AND CODE COMPILES, you have a working version of code.
5. Open the particle workbench extension within VS code.  There should be a startup menu. Select the option on the startup menu that says "Create New Project".  When prompted about what         location on your local machine you wish to store this "New Project", select the folder that is your entire github repository that you just cloned.
6. You will then be prompted to select a project name by VS Code, type in the exact name of the folder that is the firmware version you wish to work with (case sensitive). After doing this,     particle workbench should connect to the project and you should be able to develop/compile/upload code as normal.

Important Notes: The <firmware version>.cpp file within the SRC folder is the file that must be compiled and uploaded to the particle microcontroller.  The OzPOD_node.h file is the file where you will configure the board how you desire. You must recompile the .cpp file everytime you update the OzPOD_node.h file. 


# Update Tracker
Thanks to Izzy for this suggestion! Here we will be tracking each version of the firmware.
| Version       | "Named" Ver.       | Pilot         | Date               | Description & Purpose                  | 
| ------------- | ------------------ | ------------- | -------------      | -------------------------------------- |
| V1            | Initial Changes    | Spencer       | 05/21/2025         | Initial adaptation of LPOD --> OzPOD code   |
| V2            | Compile Fix        | Percy         | 05/21/2025         | Fixes incompatibility issues with git & workbench        |
| V3            | File naming Scheme Change       | Spencer        | 05/22/2025         | Fixed File Names to align with OzPOD project naming scheme. Also added software change to allow alphasense to read negative numbers    |
| V4            | Alphasense Troubleshooting       | Percy        | 04/06/2026         | Swapped worker & aux to reflect accurate channels & added setting for troubleshooting alphasense  |

V4 - tested with Boron OS 6.3.3
