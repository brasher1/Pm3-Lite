// sprite.h : header file
//
// CSprite class
//
//

class CSprite : public CDIB
{
    DECLARE_SERIAL(CSprite)
public:
    CSprite();
    ~CSprite();

    virtual int GetX() {return m_x;}   // Get x.
    virtual int GetY() {return m_y;}   // Get y.
    virtual int GetZ() {return m_z;}   // Get z-order.

    virtual void Serialize(CArchive& ar);
    virtual void Render(CDIB* pDIB, CRect* pClipRect = NULL);
    virtual BOOL Load(CFile* fp);               // Load from file
    virtual BOOL Load(char* pszFileName = NULL);// Load DIB from disk file
    virtual BOOL MapColorsToPalette(CPalette* pPal);
    virtual void GetRect(CRect* pRect); 
    virtual BOOL HitTest(CPoint point);
    virtual void SetPosition(int x,
                             int y);
    virtual void SetZ(int z);
    virtual void SetNotificationObject(CSpriteNotifyObj* pNO)
        {m_pNotifyObj = pNO;}


protected:
    int m_x;                        // X Coordinate of top-left corner
    int m_y;                        // Y Coordinate of top-left corner
    int m_z;                        // Z-order for sprite
    BYTE m_bTransIndex;             // Transparency index value
    CSpriteNotifyObj* m_pNotifyObj; // Pointer to a notification object

private:
    void Initialize();              // Set initial state.
};
