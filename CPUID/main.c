//
//  main.c
//  CPUID
//
//  Created by Артем Шарапов on 01.05.2020.
//  Copyright © 2020 Hooba-booba. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Types.h"

_zero cpuid_zero(void) {
    unsigned mask = 0xFF000000;
    unsigned char shift = 24;
    _zero data;
    uint32_t eax_buf,  ebx_buf, edx_buf, ecx_buf;
    eax_buf = ebx_buf = edx_buf = ecx_buf = 0;
    asm {
        mov eax, 0
        cpuid
        mov eax_buf, eax
        mov ebx_buf, ebx
        mov edx_buf, edx
        mov ecx_buf, ecx
    }
    data.max_number_of_func = eax_buf;
    for (int i = 0; i < 4; ++i) {
        data.message[0][i] = (ebx_buf & mask) >> shift;
        mask = mask >> 8;
        shift-=8;
    }
    mask = 0xFF000000;
    shift = 24;
    for (int i = 0; i < 4; ++i) {
        data.message[1][i] = (edx_buf & mask) >> shift;
        mask = mask >> 8;
        shift-=8;
    }
    mask = 0xFF000000;
    shift = 24;
    for (int i = 0; i < 4; ++i) {
        data.message[2][i] = (ecx_buf & mask) >> shift;
        mask = mask >> 8;
        shift-=8;
    }
    return data;
}

_one cpuid_one(void) {
    _one data;
    uint32_t eax_buf,  ebx_buf, edx_buf;
    eax_buf = ebx_buf = edx_buf = 0;
    asm {
        mov eax, 1
        cpuid
        mov eax_buf, eax
        mov ebx_buf, ebx
        mov edx_buf, edx
    }
    data.stepping_id = eax_buf & 0xF;
    data.model_number = (eax_buf & 0xF0) >> 4;
    data.family_code = (eax_buf & 0xF00) >> 8;
    data.processor_type = (eax_buf & 0x3000) >> 12;
    data.extended_model = (eax_buf & 0xF0000) >> 16;
    data.extended_family = (eax_buf & 0xFF00000) >> 20;
    
    data.brand_id = ebx_buf & 0xFF;
    data.chunks = (ebx_buf & 0xFF00) >> 8;
    data.apic_id = (ebx_buf & 0xFF000000) >> 24;
    
    for (int i = 0; i < 32; ++i)
        data.characteristics[i].value = (edx_buf >> i) & 1;
    
    return data;
}

_two cpuid_two(void) {
    _two data;
    uint32_t eax_buf,  ebx_buf, edx_buf, ecx_buf;
    eax_buf = ebx_buf = edx_buf = ecx_buf = 0;
    asm {
        mov eax, 2
        cpuid
        mov eax_buf, eax
        mov ebx_buf, ebx
        mov edx_buf, edx
        mov ecx_buf, ecx
    }
    unsigned mask = 0xFF000000;
    unsigned char shift = 24;
    int i = 0;
    for (i = 0; i < 3; i+=1) {
        data.cache_descriptors[i] = (eax_buf & mask) >> shift;
        mask = mask >> 8;
        shift-=8;
    }
    mask = 0xFF000000;
    shift = 24;
    for (i+=1; i < 7; i+=1) {
        data.cache_descriptors[i] = (ebx_buf & mask) >> shift;
        mask = mask >> 8;
        shift-=8;
    }
    mask = 0xFF000000;
    shift = 24;
    for (i+=1; i < 11; i+=1) {
        data.cache_descriptors[i] = (edx_buf & mask) >> shift;
        mask = mask >> 8;
        shift-=8;
    }
    mask = 0xFF000000;
    shift = 24;
    for (i+=1; i < 15; i+=1) {
        data.cache_descriptors[i] = (ecx_buf & mask) >> shift;
        mask = mask >> 8;
        shift-=8;
    }
    return data;
}

void print_cpuid_zero(void) {
    _zero data = cpuid_zero();
    printf("Максимальный разрешенный номер функции базового набора: %d\n", data.max_number_of_func);
    printf("Производитель: ");
    for (int i = 0;i < 3; ++i) {
        for (int j = 3; j >= 0; --j)
            printf("%c",data.message[i][j]);
    }
    printf("\n");
}

void print_cpuid_one(void) {
    char *charachteristics_text[] = {"Есть встроенный сопроцессор","Поддерживается VME","Поддерживается DE","Поддерживается PSE","Поддерживается TSC (команда RDTSC)","Поддерживается MSR (команды RDMSR и WRMSR)","Поддерживается РАE","Поддерживается MСE","Поддерживается команда СМРХСНG","Есть встроенный APIC"," ","Поддерживаются команды SYSENTER и SYSEXIT","Поддерживается MTRR","Поддерживается PGE","Поддерживается MGE_CAP (machine check global capability) MSR","Поддержка команд CMOV cc (mov and compare), cc-условие","Поддерживается РАТ (Page Attribute Table)","Поддерживается PSE-36","Процессор имеет сериальный номер","Поддерживается команда CLFLUSH"," ","Поддерживается возможность записи отладочной информации и «истории переходов» в созданный в памяти резидентный буфер","Поддерживается контроль температу-ры и регулировка производительности процессора под управлением ПО","Поддерживается ММХ","Поддерживаются команды  FXRSTOR и FXRSAVE","Поддерживается набор команд SSE","Поддерживается набор команд SSE2","Самослежение процессора за собственными кэшами (Self-Snoop)"," ","Встроенный аппаратный контроль температуры."," "," "};
    _one data = cpuid_one();
    printf("Семейство: ");
    switch (data.family_code) {
        case 4:
            printf("МП 486\n");
            break;
        case 5:
            printf("Pentium\n");
            break;
        case 6:
            printf("Core I\n");
            break;
        case 0x0F:
            printf("Pentium IV\n");
            break;
        default:
            break;
    }
    printf("Номер модели внутри семейства: %ui\nНомер варианта внутри модели: %ui\n", data.model_number, data.stepping_id);
    printf("Тип процессора: ");
    switch (data.processor_type) {
        case 0:
            printf("Original OEM processor\n");
            break;
        case 1:
            printf("OverDrive processor\n");
            break;
        case 2:
            printf("Dual processor\n");
            break;
        case 3:
            printf("Intel reserved\n");
            break;
        default:
            break;
    }
    printf("Размер строки кэша: %d\nУточнение модели: ", data.chunks * 8);
    switch (data.brand_id) {
        case 0:
            printf("Не поддерживается\n");
            break;
        case 1:
            printf("Intel Celeron\n");
            break;
        case 2:
            printf("Intel Pentium III\n");
            break;
        case 3:
            printf("Intel Pentium III Xeon\n");
            break;
        case 4:
            printf("Intel Pentium III\n");
            break;
        case 8:
            printf("Intel Pentium 4\n");
            break;
        case 9:
            printf("Intel Xeon\n");
            break;
        default:
            break;
    }
    for (int i = 0 ; i < 32; ++i) {
        if (data.characteristics[i].value == 1)
            printf("%s\n", charachteristics_text[i]);
    }
}

void print_cpuid_two() {
    _two data = cpuid_two();
    for (int i = 0; i < 15; ++i) {
        switch (data.cache_descriptors[i]) {
            case 0x0:
                printf("Пустой\n");
                break;
            case 0x01:
                printf("TLB команд 4 Kбайт; 4 w.s., 32 строки\n");
                break;
            case 0x02:
                printf("TLB команд 4 Мбайт; fully., 2 строки\n");
                break;
            case 0x03:
                printf("TLB данных 4 Kбайт; 4 w.s., 64 строки\n");
                break;
            case 0x04:
                printf("TLB данных 4 Мбайт; 4 w.s.,  8 строк\n");
                break;
            case 0x06:
                printf("Кэш команд, 8 Кбайт; 4ws, строка-32байта\n");
                break;
            case 0x08:
                printf("Кэш команд, 16 Кбайт; 4ws, строка-32байта");
                break;
            case 0x0A:
                printf("Кэш данных, 8 Кбайт;2ws, строка-32байта\n");
                break;
            case 0x0C:
                printf("Кэш данных, 16 Кбайт; 4ws, строка-32байта\n");
                break;
            case 0x40:
                printf("Нет L2 кэша для P6 или L3 для Pentium IV\n");
                break;
            case 0x41:
                printf("Кэш L2, 128 Кбайт; 4ws, строка-32байта\n");
                break;
            case 0x42:
                printf("Кэш L2, 256 Кбайт; 4ws, строка-32байта\n");
                break;
            case 0x43:
                printf("Кэш L2, 512 Кбайт; 4ws, строка-32байта\n");
                break;
            case 0x44:
                printf("Кэш L2, 1 Mбайт; 4ws, строка-32байта\n");
                break;
            case 0x45:
                printf("Кэш L2, 2 Mбайт; 4ws, строка-32байта\n");
                break;
            case 0x50:
                printf("Общий TLB команд, fully, 64 строки\n");
                break;
            case 0x51:
                printf("Общий TLB команд, fully, 128 строк\n");
                break;
            case 0x52:
                printf("Общий TLB команд, fully, 256 строк\n");
                break;
            case 0x5B:
                printf("Общий TLB данных, fully, 64 строки\n");
                break;
            case 0x5C:
                printf("Общий TLB данных, fully, 128 строк\n");
                break;
            case 0x5D:
                printf("Общий TLB данных, fully, 256 строк\n");
                break;
            case 0x66:
                printf("Кэш данных, 8 Кбайт ;4ws, строка 64 байта\n");
                break;
            case 0x67:
                printf("Кэш данных, 16 Кбайт ;4ws, строка 64 байта\n");
                break;
            case 0x68:
                printf("Кэш данных, 32 Кбайт ;4ws, строка 64 байта\n");
                break;
            case 0x70:
                printf("Trace-кэш команд 8ws, 12K mops \n");
                break;
            case 0x71:
                printf("Trace-кэш команд 8ws, 16K mops \n");
                break;
            case 0x72:
                printf("Trace-кэш команд 8ws, 32K mops \n");
                break;
            case 0x79:
                printf("Кэш L2, 128 Кбайт ;8ws, строка 64байта\n");
                break;
            case 0x7A:
                printf("Кэш L2, 256 Кбайт ;8ws, строка 64байта\n");
                break;
            case 0x7B:
                printf("Кэш L2, 512 Кбайт ;8ws, строка 64байта\n");
                break;
            case 0x7C:
                printf("Кэш L2, 1 Mбайт ;8ws, строка 64байта\n");
                break;
            case 0x82:
                printf("Кэш L2, 256 Kайт ;8ws, строка 32байта\n");
                break;
            case 0x83:
                printf("Кэш L2, 512 Kбайт ;8ws, строка 32байта\n");
                break;
            case 0x84:
                printf("Кэш L2, 1 Mбайт ;8ws, строка 32байта\n");
                break;
            case 0x85:
                printf("Кэш L2, 2 Mбайт ;8ws, строка 32байта\n");
                break;
            default:
                break;
        }
    }
}

int main(int argc, const char * argv[]) {
    print_cpuid_zero();
    print_cpuid_one();
    print_cpuid_two();
    return 0;
}
