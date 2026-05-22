#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

int main() {
    // DEVMODE structure contains information about the device capabilities 
    // and settings of a printer or a display device.
    DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);

    std::cout << "Enumerating available display resolutions..." << std::endl;
    std::cout << "Enum\t Width x Height \t Refresh Rate \t Color Depth" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;

    // Use EnumDisplaySettings to iterate through all supported modes.
    // ENUM_CURRENT_SETTINGS or ENUM_REGISTRY_SETTINGS can be used for specific modes,
    // but passing an index (starting at 0) retrieves all supported modes.
    for (int i = 0; EnumDisplaySettings(NULL, i, &dm) != 0; i++) {
        std::cout << i << "\t " << dm.dmPelsWidth << " x " << dm.dmPelsHeight
                  << "\t " << dm.dmDisplayFrequency << " Hz"
                  << "\t " << dm.dmBitsPerPel << " bit" << std::endl;
    }

    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "Enumeration complete." << std::endl;

    return 0;
}
