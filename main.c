#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

PSP_MODULE_INFO("World Football Legends", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

int main(void)
{
    pspDebugScreenInit();

    pspDebugScreenSetTextColor(0xFF00FF00);
    pspDebugScreenPrintf("WORLD FOOTBALL LEGENDS\n\n");

    pspDebugScreenSetTextColor(0xFFFFFFFF);
    pspDebugScreenPrintf("================================\n");
    pspDebugScreenPrintf("       FOOTBALL GAME DEMO       \n");
    pspDebugScreenPrintf("================================\n\n");

    pspDebugScreenPrintf("Welcome, Manager!\n\n");
    pspDebugScreenPrintf("Game Status: Initial Prototype\n\n");
    pspDebugScreenPrintf("Upcoming Features:\n");
    pspDebugScreenPrintf("- Club Teams\n");
    pspDebugScreenPrintf("- National Teams\n");
    pspDebugScreenPrintf("- League Mode\n");
    pspDebugScreenPrintf("- Tournament Mode\n");
    pspDebugScreenPrintf("- 3D Football Gameplay\n\n");

    pspDebugScreenPrintf("First Goal: Build our game!\n");

    sceKernelSleepThread();

    return 0;
}
