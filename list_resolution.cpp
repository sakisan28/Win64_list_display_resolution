#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        // List available resolutions
        DEVMODE dm;
        ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);

        std::cout << "Enumerating available display resolutions..." << std::endl;
        std::cout << "Enum\t Width x Height \t Refresh Rate \t Color Depth" << std::endl;
        std::cout << "---------------------------------------------------" << std::endl;

        for (int i = 0; EnumDisplaySettings(NULL, i, &dm) != 0; i++) {
            std::cout << i << "\t " << dm.dmPelsWidth << " x " << dm.dmPelsHeight
                      << "\t " << dm.dmDisplayFrequency << " Hz"
                      << "\t " << dm.dmBitsPerPel << " bit" << std::endl;
        }

        std::cout << "---------------------------------------------------" << std::endl;
        std::cout << "Enumeration complete. Provide an 'Enum' index as an argument to change resolution." << std::endl;
    }
    else if (argc == 2) {
        // Set display resolution
        int index = std::stoi(argv[1]);
        DEVMODE dm;
        ZeroMemory(&dm, sizeof(dm));
        dm.dmSize = sizeof(dm);

        if (EnumDisplaySettings(NULL, index, &dm)) {
            std::cout << "Attempting to change resolution to: " 
                      << dm.dmPelsWidth << "x" << dm.dmPelsHeight << "..." << std::endl;
            
            LONG result = ChangeDisplaySettings(&dm, 0);

            switch (result) {
                case DISP_CHANGE_SUCCESSFUL:
                    std::cout << "Display settings changed successfully." << std::endl;
                    break;
                case DISP_CHANGE_RESTART:
                    std::cout << "A restart is required for the changes to take effect." << std::endl;
                    break;
                case DISP_CHANGE_BADMODE:
                    std::cerr << "Error: The graphics mode is not supported." << std::endl;
                    break;
                default:
                    std::cerr << "Error: Failed to change display settings. Code: " << result << std::endl;
                    break;
            }
        } else {
            std::cerr << "Error: Invalid index '" << index << "'. Use 0 arguments to list valid indices." << std::endl;
            return 1;
        }
    }

    return 0;
}
