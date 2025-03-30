#include "math.h"  
#include "memory.h"  
#include "../sdk/datatypes/qangle.h"

bool MATH::Setup()  
{  
   bool bSuccess = true;  

   const void* hTier0Lib = MEM::GetModuleBaseHandle(TIER0_DLL);  
   if (hTier0Lib == nullptr)  
       return false;  

   fnRandomSeed = reinterpret_cast<decltype(fnRandomSeed)>(MEM::GetExportAddress(hTier0Lib, CS_XOR("RandomSeed")));  
   bSuccess &= (fnRandomSeed != nullptr);  

   fnRandomFloat = reinterpret_cast<decltype(fnRandomFloat)>(MEM::GetExportAddress(hTier0Lib, CS_XOR("RandomFloat")));  
   bSuccess &= (fnRandomFloat != nullptr);  

   fnRandomFloatExp = reinterpret_cast<decltype(fnRandomFloatExp)>(MEM::GetExportAddress(hTier0Lib, CS_XOR("RandomFloatExp")));  
   bSuccess &= (fnRandomFloatExp != nullptr);  

   fnRandomInt = reinterpret_cast<decltype(fnRandomInt)>(MEM::GetExportAddress(hTier0Lib, CS_XOR("RandomInt")));  
   bSuccess &= (fnRandomInt != nullptr);  

   fnRandomGaussianFloat = reinterpret_cast<decltype(fnRandomGaussianFloat)>(MEM::GetExportAddress(hTier0Lib, CS_XOR("RandomGaussianFloat")));  
   bSuccess &= (fnRandomGaussianFloat != nullptr);  

   return bSuccess;  
}  

void MATH::AngleVectors(const QAngle_t& angles, Vector_t* forward, Vector_t* right, Vector_t* up) {  
   float sp, sy, sr, cp, cy, cr;  

   MATH::SinCos(angles.x * 0.017453292f, &sp, &cp);  
   MATH::SinCos(angles.y * 0.017453292f, &sy, &cy);  
   MATH::SinCos(angles.z * 0.017453292f, &sr, &cr);  

   if (forward) {  
       forward->x = cp * cy;  
       forward->y = cp * sy;  
       forward->z = -sp;  
   }  

   if (right) {  
       right->x = sr * sp * cy - cr * sy;  
       right->y = cr * cy + sr * sp * sy;  
       right->z = sr * cp;  
   }  

   if (up) {  
       up->x = sr * sy + cr * sp * cy;  
       up->y = cr * sp * sy - sr * cy;  
       up->z = cr * cp;  
   }  
}