#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GLOBAL_MODES 2048

typedef struct {
    char deviceName[32];
    DEVMODE dm;
} GlobalMode;

GlobalMode g_modes[MAX_GLOBAL_MODES];
DWORD g_totalModes = 0;
BOOL g_silent = FALSE;

// Greatest Common Divisor helper
long gcd(long a, long b) {
    while (b != 0) {
        long t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Calculate and format the aspect ratio string
void GetAspectRatio(long width, long height, char* buf, size_t bufSize) {
    // Explicit resolution overrides
    if (width == 1366 && height == 768) {
        sprintf_s(buf, bufSize, "16:9");
        return;
    }
    if (width == 1360 && height == 768) {
        sprintf_s(buf, bufSize, "16:9");
        return;
    }
    if (width == 1280 && height == 600) {
        sprintf_s(buf, bufSize, "16:7.5");
        return;
    }

    long g = gcd(width, height);
    if (g <= 0) {
        sprintf_s(buf, bufSize, "Unknown");
        return;
    }
    long w = width / g;
    long h = height / g;

    // Translate mathematical ratios to popular industry aspect ratios
    if (w == 8 && h == 5) {
        sprintf_s(buf, bufSize, "16:10");
    } else if (w == 64 && h == 27) {
        sprintf_s(buf, bufSize, "21:9");
    } else if (w == 43 && h == 18) {
        sprintf_s(buf, bufSize, "21:9");
    } else if (w == 12 && h == 5) {
        sprintf_s(buf, bufSize, "21:9");
    } else {
        sprintf_s(buf, bufSize, "%ld:%ld", w, h);
    }
}

// Fixed output mode string helper
const char* GetFixedOutputString(DWORD mode) {
    switch (mode) {
        case DMDFO_DEFAULT: return "Default";
        case DMDFO_CENTER:  return "Center (No Stretch)";
        case DMDFO_STRETCH: return "Stretch to Fill";
        default:            return "Unknown";
    }
}

// Callback function for EnumDisplayMonitors
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
    (void)hdcMonitor;
    (void)lprcMonitor;
    (void)dwData;

    MONITORINFOEX mi;
    mi.cbSize = sizeof(MONITORINFOEX);
    
    if (!GetMonitorInfo(hMonitor, (LPMONITORINFO)&mi)) {
        if (!g_silent) {
            printf("Failed to get monitor info.\n\n");
        }
        return TRUE;
    }

    if (!g_silent) {
        printf("======================================================================\n");
        printf(" Monitor: %s %s\n", mi.szDevice, (mi.dwFlags & MONITORINFOF_PRIMARY) ? "[Primary]" : "");
        printf("======================================================================\n");
        printf("  Virtual Bounds: Left=%ld, Top=%ld, Right=%ld, Bottom=%ld\n", 
               mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right, mi.rcMonitor.bottom);
        printf("  Work Area:      Left=%ld, Top=%ld, Right=%ld, Bottom=%ld\n\n", 
               mi.rcWork.left, mi.rcWork.top, mi.rcWork.right, mi.rcWork.bottom);

        // Get current settings
        DEVMODE dmCurrent;
        ZeroMemory(&dmCurrent, sizeof(dmCurrent));
        dmCurrent.dmSize = sizeof(dmCurrent);
        
        if (EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &dmCurrent)) {
            char aspectStr[32];
            GetAspectRatio(dmCurrent.dmPelsWidth, dmCurrent.dmPelsHeight, aspectStr, sizeof(aspectStr));

            printf("  Current Active Mode:\n");
            printf("    Resolution:   %ld x %ld (%s)\n", dmCurrent.dmPelsWidth, dmCurrent.dmPelsHeight, aspectStr);
            printf("    Color Depth:  %ld bits per pixel\n", dmCurrent.dmBitsPerPel);
            printf("    Refresh Rate: %ld Hz\n", dmCurrent.dmDisplayFrequency);
            if (dmCurrent.dmFields & DM_DISPLAYFIXEDOUTPUT) {
                printf("    LCD Stretch:  %s\n", GetFixedOutputString(dmCurrent.dmDisplayFixedOutput));
            } else {
                printf("    LCD Stretch:  Not Supported/Specified\n");
            }
        }
        printf("\n");

        // List headers
        printf("  Supported Graphic Modes:\n");
        printf("  -------------------------------------------------------------------------------------------------\n");
        printf("  %-6s %-26s %-12s %-14s %-25s\n", "Index", "Resolution", "Color Depth", "Refresh Rate", "LCD Stretch (Fixed Output)");
        printf("  -------------------------------------------------------------------------------------------------\n");
    }

    DEVMODE dm;
    ZeroMemory(&dm, sizeof(dm));
    dm.dmSize = sizeof(dm);
    DWORD localModeNum = 0;

    while (EnumDisplaySettingsEx(mi.szDevice, localModeNum, &dm, 0)) {
        // Store in global cache
        if (g_totalModes < MAX_GLOBAL_MODES) {
            strcpy_s(g_modes[g_totalModes].deviceName, sizeof(g_modes[g_totalModes].deviceName), mi.szDevice);
            g_modes[g_totalModes].dm = dm;
        }

        if (!g_silent) {
            char aspectStr[32];
            GetAspectRatio(dm.dmPelsWidth, dm.dmPelsHeight, aspectStr, sizeof(aspectStr));

            char resStr[48];
            sprintf_s(resStr, sizeof(resStr), "%ld x %ld (%s)", dm.dmPelsWidth, dm.dmPelsHeight, aspectStr);

            char bppStr[16];
            sprintf_s(bppStr, sizeof(bppStr), "%ld bpp", dm.dmBitsPerPel);

            char freqStr[16];
            sprintf_s(freqStr, sizeof(freqStr), "%ld Hz", dm.dmDisplayFrequency);

            const char* stretchStr = "N/A";
            if (dm.dmFields & DM_DISPLAYFIXEDOUTPUT) {
                stretchStr = GetFixedOutputString(dm.dmDisplayFixedOutput);
            }

            printf("  %-6lu %-26s %-12s %-14s %-25s\n", 
                   g_totalModes, resStr, bppStr, freqStr, stretchStr);
        }

        g_totalModes++;
        localModeNum++;
    }

    if (!g_silent) {
        printf("  -------------------------------------------------------------------------------------------------\n");
        printf("  Total modes for this monitor: %lu (Global index limit: %lu)\n\n", localModeNum, g_totalModes - 1);
    }

    return TRUE;
}

// Print Usage Instructions in English
void PrintUsage(const char* progName) {
    fprintf(stderr, "\n");
    fprintf(stderr, "Windows Display Resolution Manager\n");
    fprintf(stderr, "==================================\n");
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s\n", progName);
    fprintf(stderr, "      Lists all active monitors and their supported display modes with a unique global index.\n\n");
    fprintf(stderr, "  %s <mode_index>\n", progName);
    fprintf(stderr, "      Changes the display resolution to the mode specified by <mode_index>.\n\n");
    fprintf(stderr, "Examples:\n");
    fprintf(stderr, "  %s      (Show current resolutions and indexes)\n", progName);
    fprintf(stderr, "  %s 135  (Apply the mode at global index 135)\n\n", progName);
}

int main(int argc, char* argv[]) {
    // Set console code page to UTF-8 for better character rendering
    SetConsoleOutputCP(CP_UTF8);

    // Get the base program name from argv[0]
    char progName[MAX_PATH] = "list_resolution.exe";
    if (argc > 0 && argv[0] != NULL) {
        char ext[MAX_PATH];
        _splitpath_s(argv[0], NULL, 0, NULL, 0, progName, sizeof(progName), ext, sizeof(ext));
        strcat_s(progName, sizeof(progName), ext);
    }

    if (argc == 1) {
        g_silent = FALSE;
        printf("======================================================================\n");
        printf("                 Windows Display Resolution Enumerator                 \n");
        printf("======================================================================\n\n");

        if (!EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0)) {
            fprintf(stderr, "Error: Failed to enumerate display monitors.\n");
            return 1;
        }
        return 0;
    } else if (argc == 2) {
        // Parse the index
        char* endptr;
        long targetIndex = strtol(argv[1], &endptr, 10);
        if (*endptr != '\0' || targetIndex < 0) {
            fprintf(stderr, "Error: Invalid index parameter '%s'. Must be a non-negative integer.\n", argv[1]);
            PrintUsage(progName);
            return 1;
        }

        // Silently enumerate to build the global index map
        g_silent = TRUE;
        if (!EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, 0)) {
            fprintf(stderr, "Error: Failed to enumerate display monitors.\n");
            return 1;
        }

        if (targetIndex >= (long)g_totalModes) {
            fprintf(stderr, "Error: Index %ld is out of range. Total modes available: %lu.\n", targetIndex, g_totalModes);
            PrintUsage(progName);
            return 1;
        }

        // Apply settings
        GlobalMode* mode = &g_modes[targetIndex];
        char aspectStr[32];
        GetAspectRatio(mode->dm.dmPelsWidth, mode->dm.dmPelsHeight, aspectStr, sizeof(aspectStr));

        printf("Applying mode %ld:\n", targetIndex);
        printf("  Monitor:    %s\n", mode->deviceName);
        printf("  Resolution: %ld x %ld (%s)\n", mode->dm.dmPelsWidth, mode->dm.dmPelsHeight, aspectStr);
        printf("  Color:      %ld bits\n", mode->dm.dmBitsPerPel);
        printf("  Frequency:  %ld Hz\n", mode->dm.dmDisplayFrequency);
        if (mode->dm.dmFields & DM_DISPLAYFIXEDOUTPUT) {
            printf("  Stretch:    %s\n", GetFixedOutputString(mode->dm.dmDisplayFixedOutput));
        }

        LONG result = ChangeDisplaySettingsEx(
            mode->deviceName,
            &mode->dm,
            NULL,
            CDS_UPDATEREGISTRY,
            NULL
        );

        if (result == DISP_CHANGE_SUCCESSFUL) {
            printf("\nSuccess: Resolution changed successfully.\n");
            return 0;
        } else {
            fprintf(stderr, "\nError: Failed to change resolution (Exit code: %ld).\n", result);
            switch (result) {
                case DISP_CHANGE_RESTART:
                    fprintf(stderr, "  Reason: A computer restart is required for the change to take effect.\n");
                    break;
                case DISP_CHANGE_BADMODE:
                    fprintf(stderr, "  Reason: The graphics mode is not supported by the display adapter.\n");
                    break;
                case DISP_CHANGE_FAILED:
                    fprintf(stderr, "  Reason: The display driver failed the specified graphics mode.\n");
                    break;
                default:
                    fprintf(stderr, "  Reason: An unspecified error occurred.\n");
                    break;
            }
            return 1;
        }
    } else {
        fprintf(stderr, "Error: Too many parameters.\n");
        PrintUsage(progName);
        return 1;
    }
}
