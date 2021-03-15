#ifndef __ximaiter_h
#define __ximaiter_h

#include "ximage.h"
#include "ximadef.h"
#include <algorithm>

class CImageIterator
{
friend class CxImage;
protected:
    int32_t Itx, Ity; // Counters
    int32_t Stepx, Stepy;
    uint8_t* IterImage; // Image pointer
    CxImage* ima;
public:
    // Constructors
    CImageIterator();
    CImageIterator(CxImage* image);
    operator CxImage* ();

    void Upset();
    void SetRow(uint8_t* buf, int32_t n);
    void GetRow(uint8_t* buf, int32_t n);
    uint8_t* GetRow();
    BOOL PrevRow();
};

inline CImageIterator::CImageIterator()
{
    ima = 0;
    IterImage = 0;
    Itx = Ity = 0;
    Stepx = Stepy = 0;
}
inline CImageIterator::CImageIterator(CxImage* imageImpl) : ima(imageImpl)
{
    if (ima) IterImage = ima->GetBits();
    Itx = Ity = 0;
    Stepx = Stepy = 0;
}
inline CImageIterator::operator CxImage* ()
{
    return ima;
}
inline void CImageIterator::Upset()
{
    Itx = 0;
    Ity = ima->GetHeight() - 1;
    IterImage = ima->GetBits() + ima->GetEffWidth() * (ima->GetHeight() - 1);
}
inline void CImageIterator::SetRow(uint8_t* buf, int32_t n)
{
    if (n < 0) n = (int32_t)ima->GetEffWidth();
    else n = (std::min)(n, (int32_t)ima->GetEffWidth());

    if ((IterImage != NULL) && (buf != NULL) && (n > 0)) memcpy(IterImage, buf, n);
}
inline void CImageIterator::GetRow(uint8_t* buf, int32_t n)
{
    if ((IterImage != NULL) && (buf != NULL) && (n > 0))
        memcpy(buf, IterImage, (std::min)(n, (int32_t)ima->GetEffWidth()));
}
inline BOOL CImageIterator::PrevRow()
{
    if (--Ity < 0) return 0;
    IterImage -= ima->GetEffWidth();
    return 1;
}
inline uint8_t* CImageIterator::GetRow()
{
    return IterImage;
}
#endif // __ximaiter_h
