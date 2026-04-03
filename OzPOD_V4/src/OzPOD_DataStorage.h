#ifndef _OzPOD_DataStorage_h
#define _OzPOD_DataStorage_h

#include "Particle.h"

#define RUN_TO_PUBLISH      19

struct OZPOD_data
{    
    uint16_t Mics[RUN_TO_PUBLISH];
    uint16_t Mq[RUN_TO_PUBLISH];
    int16_t B4_Worker[RUN_TO_PUBLISH];
    int16_t B4_Auxiliary[RUN_TO_PUBLISH];

    float Temperature[RUN_TO_PUBLISH];
    float Rel_Humidity[RUN_TO_PUBLISH];
};

class OZPOD {
    public:
        OZPOD();

    private:

};

#endif    //BORON_DATA_STORAGE_H