#pragma once
int roll(int min, int max);

#ifdef UNIT_TEST
void setForcedRoll(int value);
void clearForcedRoll();
#endif
