#include <stdio.h>

#include "image-loader.cc"
#include "config-system.cc"
#include "lottie-system.cc"
#include "image-sequence-system.cc"
#include "command-system.cc"
#include "render-system.cc"
#include "redis-system.cc"

#include "image-sequence.cc"

#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>

volatile bool interruptReceived = false;

static void InterruptHandler(int _Signo) {
  interruptReceived = true;
}

int main(int argc, char *argv[]) {
    RenderSystem::Initialize();
    ImageLoader::Initialize();
    ConfigSystem::Initialize();
    LottieSystem::Initialize();
    ImageSequenceSystem::Initialize();
    CommandSystem::Initialize();
    RedisSystem::Initialize();

    signal(SIGTERM, InterruptHandler);
    signal(SIGINT, InterruptHandler);

    while(!interruptReceived)
    {
        RedisSystem::Run();
        RenderSystem::Run();
        usleep(1 * 1000);
    }

    printf("Received CTRL-C. Exiting.\n");

    RedisSystem::Finalize();
    CommandSystem::Finalize();
    ImageSequenceSystem::Finalize();
    LottieSystem::Finalize();
    ConfigSystem::Finalize();
    ImageLoader::Finalize();
    RenderSystem::Finalize();

    return 0;
}