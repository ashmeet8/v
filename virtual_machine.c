#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

uint64_t push_value(uint64_t *S, uint64_t *SP, uint64_t *V)
{
    S[*SP] = *V;
    *SP = *SP + 1;
    printf("Push: %lx\n", *V);
    return *SP;
}

uint64_t pop_value(uint64_t *S, uint64_t *SP, uint64_t *V)
{
    *V = S[(*SP) - 1];
    *SP = *SP - 1;
    printf("Pop:  %lx\n", *V);
    return *SP;
}

void read_value(void *BVP, uint64_t I, uint64_t W, uint64_t *V)
{
    uint8_t *B;
    B = (uint8_t *)BVP;

    if (W >= 8)
    {
        *V = ((uint64_t)(B[I]));
    }
    if (W >= 16)
    {
        *V = *V | (((uint64_t)(B[I + 1])) << 8);
    }
    if (W >= 32)
    {
        *V = *V | (((uint64_t)(B[I + 2])) << 16) | (((uint64_t)(B[I + 3])) << 24);
    }
    if (W >= 64)
    {
        *V = *V | (((uint64_t)(B[I + 4])) << 32) | (((uint64_t)(B[I + 5])) << 40) | (((uint64_t)(B[I + 6])) << 48) | (((uint64_t)(B[I + 7])) << 56);
    }
}

void write_value(void *BVP, uint64_t I, uint64_t W, uint64_t *V)
{
    uint8_t *B;
    B = (uint8_t *)BVP;

    if (W >= 8)
    {
        B[I] = ((uint8_t)((*V) & 0xff));
    }
    if (W >= 16)
    {
        B[I + 1] = ((uint8_t)(((*V) & 0xff00) >> 8));
    }
    if (W >= 24)
    {
        B[I + 2] = ((uint8_t)(((*V) & 0xff0000) >> 16));
    }
    if (W >= 32)
    {
        B[I + 3] = ((uint8_t)(((*V) & 0xff000000) >> 24));
    }
    if (W >= 40)
    {
        B[I + 4] = ((uint8_t)(((*V) & 0xff00000000) >> 32));
    }
    if (W >= 48)
    {
        B[I + 5] = ((uint8_t)(((*V) & 0xff0000000000) >> 40));
    }
    if (W >= 56)
    {
        B[I + 6] = ((uint8_t)(((*V) & 0xff000000000000) >> 48));
    }
    if (W >= 64)
    {
        B[I + 7] = ((uint8_t)(((*V) & 0xff00000000000000) >> 56));
    }
}

int main(int argc, char *argv[])
{
    uint8_t *M; // Memory
    uint8_t B; // Current byte
    uint64_t C; // Counter

    uint64_t *S; // Stack
    uint64_t SP; // Stack Pointer
    uint64_t V1;
    uint64_t V2;
    uint64_t V3;
    uint64_t V4;
    uint64_t V5;

    M = (uint8_t *)malloc(0x100000);
    S = (uint64_t *)malloc(0x10000 * 8);

    C = 0x10000;
    SP = 0;

    fread((void *)M, 1, 0x100000, fopen("memory", "rb"));

    while (1) {
        B = M[C];
        V1 = 0;
        V2 = 0;
        V3 = 0;

        if (B == 0x01)
        {
            read_value((void *)M, C + 1, 8, &V1);
            push_value(S, &SP, &V1);
            C = C + 2;
        }
        else if (B == 0x11)
        {
            read_value((void *)M, C + 1, 16, &V1);
            push_value(S, &SP, &V1);
            C = C + 3;
        }
        else if (B == 0x21)
        {
            read_value((void *)M, C + 1, 32, &V1);
            push_value(S, &SP, &V1);
            C = C + 5;
        }
        else if (B == 0x31)
        {
            read_value((void *)M, C + 1, 64, &V1);
            push_value(S, &SP, &V1);
            C = C + 9;
        }
        else if (B == 0x03)
        {
            pop_value(S, &SP, &V1);
            read_value((void *)M, V1, 8, &V2);
            push_value(S, &SP, &V2);
            C = C + 1;
        }
        else if (B == 0x13)
        {
            pop_value(S, &SP, &V1);
            read_value((void *)M, V1, 16, &V2);
            push_value(S, &SP, &V2);
            C = C + 1;
        }
        else if (B == 0x23)
        {
            pop_value(S, &SP, &V1);
            read_value((void *)M, V1, 32, &V2);
            push_value(S, &SP, &V2);
            C = C + 1;
        }
        else if (B == 0x33)
        {
            pop_value(S, &SP, &V1);
            read_value((void *)M, V1, 64, &V2);
            push_value(S, &SP, &V2);
            C = C + 1;
        }
        else if (B == 0x05)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            write_value((void *)M, V1, 8, &V2);
            C = C + 1;
        }
        else if (B == 0x15)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            write_value((void *)M, V1, 16, &V2);
            C = C + 1;
        }
        else if (B == 0x25)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            write_value((void *)M, V1, 32, &V2);
            C = C + 1;
        }
        else if (B == 0x35)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            write_value((void *)M, V1, 64, &V2);
            C = C + 1;
        }
        else if (B == 0x07)
        {
            push_value(S, &SP, &C);
            C = C + 1;
        }
        else if (B == 0x09)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            V3 = V1 + V2;
            push_value(S, &SP, &V3);
            C = C + 1;
        }
        else if (B == 0x89)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            V3 = V1 + V2;
            push_value(S, &SP, &V3);
            C = C + 1;
        }
        else if (B == 0x09)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            V3 = V1 + V2;
            push_value(S, &SP, &V3);
            C = C + 1;
        }
        else if (B == 0x89)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            V3 = V2 - V1;
            push_value(S, &SP, &V3);
            C = C + 1;
        }
        else if (B == 0x49)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            V3 = V2 ^ V1;
            push_value(S, &SP, &V3);
            C = C + 1;
        }
        else if (B == 0x69)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            V3 = V2 | V1;
            push_value(S, &SP, &V3);
            C = C + 1;
        }
        else if (B == 0x79)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            V3 = V2 & V1;
            push_value(S, &SP, &V3);
            C = C + 1;
        }
        else if (B == 0x19)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            V3 = V2 << (V1 & 0x3f);
            push_value(S, &SP, &V3);
            C = C + 1;
        }
        else if (B == 0x39)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            V3 = V2 >> (V1 & 0x3f);
            push_value(S, &SP, &V3);
            C = C + 1;
        }
        else if (B == 0x0b)
        {
            pop_value(S, &SP, &V1);
            C = V1;
        }
        else if (B == 0x0d)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            pop_value(S, &SP, &V3);
            if (V1 == V2) { C = V3;}
        }
        else if (B == 0x1d)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            pop_value(S, &SP, &V3);
            if (V1 == V2) { C = V3; }
        }
        else if (B == 0x2d)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            pop_value(S, &SP, &V3);
            if (V2 < V1) { C = V3; }
        }
        else if (B == 0x0d)
        {
            pop_value(S, &SP, &V1);
            pop_value(S, &SP, &V2);
            pop_value(S, &SP, &V3);
            if (V2 >= V1) { C = V3; }
        }
        else if (B == 0x0f)
        {
            break;
        }

        else {  break; }
    }

    fwrite(M, 1, 0x100000, fopen("memory_new", "wb"));
    fwrite(S, 1, 0x10000, fopen("stack_new", "wb"));

    return 0;
}

