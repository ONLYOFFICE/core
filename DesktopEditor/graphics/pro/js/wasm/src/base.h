class CxImage;

extern "C"
{
    void* Spellchecker_Malloc(unsigned int size);
    void Spellchecker_Free(void* p);

    CxImage* CxImage_Create();
    void CxImage_Destroy(CxImage* p);

    bool CxImage_Decode(CxImage* p, unsigned char * buffer, unsigned int size, unsigned int imagetype);
}
