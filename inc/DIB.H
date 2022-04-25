// dib.h : header file
//
// CDIB class
//

#ifndef __DIB__
#define __DIB__

class CDIB : public CObject
{
    DECLARE_SERIAL(CDIB)
public:
    CDIB();
    ~CDIB();

    BITMAPINFO* GetBitmapInfoAddress()
        {return m_pBMI;}                        // Pointer to bitmap info
    void* GetBitsAddress()
        {return m_pBits;}                       // Pointer to the bits
    RGBQUAD* GetClrTabAddress()
        {return (LPRGBQUAD)(((BYTE*)(m_pBMI)) 
            + sizeof(BITMAPINFOHEADER));}       // Pointer to color table
    int GetNumClrEntries();                     // Number of color table entries
    BOOL Create(int width, int height);         // Create a new DIB
    BOOL Create(BITMAPINFO* pBMI, BYTE *pBits); // Create from existing mem,
    void* GetPixelAddress(int x, int y);
    virtual BOOL Load(CFile* fp);               // Load from file
    virtual BOOL Load(char* pszFileName = NULL);// Load DIB from disk file
    virtual void Serialize(CArchive& ar);
    virtual void Draw(CDC* pDC, int x, int y);
    virtual void DrawZoom(CDC* pDC, int x, int y, int srcx, int srcy);
    virtual int GetWidth() {return DibWidth();}   // Image width
    virtual int GetHeight() {return DibHeight();} // Image height
    virtual int MapColorsToPalette(CPalette *pPal);
    virtual void GetRect(CRect* pRect);
    virtual void CopyBits(CDIB* pDIB, 
                          int xd, int yd,
                          int w,  int h,
                          int xs, int ys,
                          COLORREF clrTrans = 0xFFFFFFFF);
	void SetScale(float scale) { m_scale = scale; }

protected:
    BITMAPINFO* m_pBMI;         // Pointer to BITMAPINFO struct
    BYTE* m_pBits;              // Pointer to the bits
    BOOL  m_bMyBits;            // TRUE if DIB owns Bits memory
	float m_scale;
private:
    int DibWidth()
        {return m_pBMI->bmiHeader.biWidth;}
    int DibHeight() 
        {return m_pBMI->bmiHeader.biHeight;}
    int StorageWidth()
        {return (m_pBMI->bmiHeader.biWidth + 3) & ~3;}
};

#endif // __DIB__
