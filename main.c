#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspdebug.h>
#include <pspctrl.h>

PSP_MODULE_INFO("World Football Legends", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

#define BUF_WIDTH 512
#define SCR_WIDTH 480
#define SCR_HEIGHT 272
#define FRAME_SIZE (BUF_WIDTH * SCR_HEIGHT * 4)

static unsigned int __attribute__((aligned(16))) display_list[262144];

typedef struct {
    float x, y, z;
} Vertex;

static void draw_pitch(void)
{
    sceGuDisable(GU_TEXTURE_2D);
    sceGuDisable(GU_CULL_FACE);

    sceGuColor(0xFF287A25);

    Vertex *v = sceGuGetMemory(4 * sizeof(Vertex));

    v[0] = (Vertex){-12.0f, 0.0f, -8.0f};
    v[1] = (Vertex){ 12.0f, 0.0f, -8.0f};
    v[2] = (Vertex){-12.0f, 0.0f,  8.0f};
    v[3] = (Vertex){ 12.0f, 0.0f,  8.0f};

    sceGuDrawArray(
        GU_TRIANGLE_STRIP,
        GU_VERTEX_32BITF | GU_TRANSFORM_3D,
        4, NULL, v
    );

    sceGuColor(0xFFFFFFFF);

    Vertex *lines = sceGuGetMemory(10 * 2 * sizeof(Vertex));

    Vertex boundary[] = {
        {-12, 0.02f, -8}, { 12, 0.02f, -8},
        { 12, 0.02f, -8}, { 12, 0.02f,  8},
        { 12, 0.02f,  8}, {-12, 0.02f,  8},
        {-12, 0.02f,  8}, {-12, 0.02f, -8},
        {  0, 0.02f, -8}, {  0, 0.02f,  8}
    };

    for (int i = 0; i < 20; i++)
        ((float *)lines)[i] = ((float *)boundary)[i];

    sceGuDrawArray(
        GU_LINES,
        GU_VERTEX_32BITF | GU_TRANSFORM_3D,
        20, NULL, lines
    );
}

int main(void)
{
    sceCtrlSetSamplingCycle(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

    sceGuInit();

    sceGuStart(GU_DIRECT, display_list);

    sceGuDrawBuffer(
        GU_PSM_8888, (void *)0, BUF_WIDTH
    );

    sceGuDispBuffer(
        SCR_WIDTH, SCR_HEIGHT,
        (void *)(FRAME_SIZE), BUF_WIDTH
    );

    sceGuDepthBuffer(
        (void *)(FRAME_SIZE * 2), BUF_WIDTH
    );

    sceGuOffset(
        2048 - SCR_WIDTH / 2,
        2048 - SCR_HEIGHT / 2
    );

    sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);
    sceGuDepthRange(65535, 0);
    sceGuScissor(0, 0, SCR_WIDTH, SCR_HEIGHT);

    sceGuEnable(GU_SCISSOR_TEST);
    sceGuEnable(GU_DEPTH_TEST);
    sceGuDepthFunc(GU_GEQUAL);

    sceGuFinish();
    sceGuSync(0, 0);

    sceDisplayWaitVblankStart();
    sceGuDisplay(GU_TRUE);

    for (;;) {
        sceGuStart(GU_DIRECT, display_list);

        sceGuClearColor(0xFF173D19);
        sceGuClearDepth(0);
        sceGuClear(
            GU_COLOR_BUFFER_BIT |
            GU_DEPTH_BUFFER_BIT
        );

        sceGumMatrixMode(GU_PROJECTION);
        sceGumLoadIdentity();

        sceGumPerspective(
            60.0f, 16.0f / 9.0f, 0.5f, 100.0f
        );

        sceGumMatrixMode(GU_VIEW);
        sceGumLoadIdentity();

        ScePspFVector3 camera = {0.0f, 14.0f, 20.0f};
        ScePspFVector3 target = {0.0f, 0.0f, 0.0f};
        ScePspFVector3 up = {0.0f, 1.0f, 0.0f};

        sceGumLookAt(&camera, &target, &up);

        sceGumMatrixMode(GU_MODEL);
        sceGumLoadIdentity();

        draw_pitch();

        sceGuFinish();
        sceGuSync(0, 0);

        sceDisplayWaitVblankStart();
        sceGuSwapBuffers();
    }

    sceGuTerm();
    sceKernelSleepThread();

    return 0;
}
