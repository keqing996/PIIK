#include "PIIK/Network/Network.h"
#include "Platform/PlatformApi.h"

namespace Piik
{
    namespace Network
    {
        void Initialize()
        {
            Npi::GlobalInit();
        }
    }
}
