
#include "GTEC.h"

class CSoundPlayer
{

public:

	// ----  Constructors ------	
  CSoundPlayer() { pAudioChunk = NULL; Channel = 0; Volume = 100;}

  // -----  Destructor --------
  ~CSoundPlayer() { if (pAudioChunk != NULL) { Mix_HaltChannel( Channel); Mix_FreeChunk( pAudioChunk); }}

private:
  Mix_Chunk *pAudioChunk;  // pointer to audio data
  int Channel;			   // audio channel
  int Volume;			   // volume
  
  
public:
 
 
  // Load audio from a .wav file
  bool LoadAudio( string filename);

  // Play a certain number of loops
  void Play(int loops);

  // play once
  void Play() 	{ Play(1); }

  // play continuously
  void PlayAlways() { Play(0); }
  
  // to be compatible with GFC
  void Play(string filename, int loops=1);

  // set volume in %
  void SetVolume( int volume);
  
  void Stop() { Mix_HaltChannel( Channel); }

  void Pause();

  void Resume();
  
  void Clear();
	
};
