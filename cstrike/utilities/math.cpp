#include "math.h"  
#include "memory.h"  
#include "../sdk/datatypes/qangle.h"
#include <DirectXMath.h>

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


QAngle_t MATH::CalcAngles(Vector_t viewPos, Vector_t aimPos) {
    __m128 view = _mm_set_ps(0.0f, viewPos.z, viewPos.y, viewPos.x);
    __m128 aim = _mm_set_ps(0.0f, aimPos.z, aimPos.y, aimPos.x);
    __m128 delta = _mm_sub_ps(aim, view);

    __m128 squared = _mm_mul_ps(delta, delta);
    __m128 sum = _mm_hadd_ps(squared, squared);
    sum = _mm_hadd_ps(sum, sum);
    __m128 length = _mm_sqrt_ps(sum);

    float deltaZ = _mm_cvtss_f32(_mm_shuffle_ps(delta, delta, _MM_SHUFFLE(2, 2, 2, 2)));
    float deltaY = _mm_cvtss_f32(_mm_shuffle_ps(delta, delta, _MM_SHUFFLE(1, 1, 1, 1)));
    float deltaX = _mm_cvtss_f32(delta);

    QAngle_t angle;
    angle.x = -asinf(deltaZ / _mm_cvtss_f32(length)) * 57.2957795131f;
    angle.y = atan2f(deltaY, deltaX) * 57.2957795131f;
    angle.z = 0.0f;

    return angle;
}
Vector_t MATH::CalculateCameraPosition(Vector_t anchorPos, float distance, QAngle_t viewAngles) {
    float yaw = DirectX::XMConvertToRadians(viewAngles.y);
    float pitch = DirectX::XMConvertToRadians(viewAngles.x);

    float x = anchorPos.x + distance * cosf(yaw) * cosf(pitch);
    float y = anchorPos.y + distance * sinf(yaw) * cosf(pitch);
    float z = anchorPos.z + distance * sinf(pitch);

    return Vector_t{ x, y, z };
}
QAngle_t MATH::NormalizeAngles(QAngle_t angles) {
    while (angles.x > 89.0f)
        angles.x -= 180.0f;
    while (angles.x < -89.0f)
        angles.x += 180.0f;
    while (angles.y > 180.0f)
        angles.y -= 360.0f;
    while (angles.y < -180.0f)
        angles.y += 360.0f;
    angles.z = 0.0f;
    return angles;
}
