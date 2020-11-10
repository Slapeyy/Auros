#pragma once

class CColor  
{


public:
	float R,G,B,A; // Red, Green, Blue, Alpha

public:
	// ----  Constructors ------
	CColor() { Set(128, 128, 128); } // default colour is gray
	CColor(Uint8 r, Uint8 g, Uint8 b)	{ Set(r, g, b); }

	// -----  Destructor --------
	virtual ~CColor()				{ }

	void Set(Uint8 r, Uint8 g, Uint8 b)	{ R = (float)r/255; G = (float)g/255; B = (float)b/255; A = 1.0; }
	void Set(float r, float g, float b, float a)	{ R = r; G = g; B = b; A = a; }
    void Set( const CColor& color) { R = color.R; G = color.G; B = color.B; A = color.A; }
	void SetAlpha( float a)  { A = a; }

    // -----  Assignment operator -----
	CColor &operator=(const CColor &Color)	{ Set(Color); return *this; }
	
	// --- pre-defined common colours -----------
	static CColor Red()				{ return(CColor(255,0,0)); }
	static CColor DarkRed()				{ return(CColor(128,0,0)); }
	static CColor Green()				{ return(CColor(0,255,0)); }
	static CColor DarkGreen()				{ return(CColor(0,128,0)); }
	static CColor Blue()				{ return(CColor(0,0,255)); }
	static CColor DarkBlue()				{ return(CColor(0,0,128)); }

	static CColor Yellow()				{ return(CColor(255,255,0)); }
	static CColor Cyan()				{ return(CColor(0,255,255)); }
	static CColor Magenta()			{ return(CColor(255, 0,255)); }

	static CColor White()				{ return(CColor(255,255,255)); }
	static CColor LightGray()			{ return(CColor(170,170,170)); }
	static CColor Gray()				{ return(CColor(128,128,128)); }
	static CColor Black()				{ return(CColor(0,0,0)); }
	static CColor None()				{ CColor c; c.Set(0,0,0,-1); return c;}

	
};

