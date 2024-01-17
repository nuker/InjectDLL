#include "inc/HQ.h"

HQ H;

int main() {

    // Change to target process
    H.GetPID(L"Hooks.exe");

    if (H.Load() == true) {
        printf("Loaded DLL.\n");
    }

    return 0;
}

