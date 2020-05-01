//
//  Types.h
//  CPUID
//
//  Created by Артем Шарапов on 01.05.2020.
//  Copyright © 2020 Hooba-booba. All rights reserved.
//

#pragma once

typedef struct bit {
    unsigned char value: 1;
} bit;

typedef struct _zero {
    int max_number_of_func;
    char message[3][4]; //message[0] = EBX; message[1] = EDX; message[2] = ECX;
} _zero;

typedef struct _one {
    //EAX
    unsigned stepping_id: 4;
    unsigned model_number: 4;
    unsigned family_code: 4;
    unsigned processor_type: 2;
    unsigned extended_model: 4;
    unsigned extended_family: 8;
    
    //EBX
    unsigned brand_id: 8;
    unsigned chunks: 8;
    unsigned apic_id: 8;
    
    //EDX
    bit characteristics[32];
}_one;

typedef struct _two {
    unsigned char cache_descriptors[15];
} _two;
