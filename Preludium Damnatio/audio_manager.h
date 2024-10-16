// audio_manager.h

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <string>
#include <SDL.h>

class AudioManager {
public:
    // Constructor
    AudioManager();

    // Destructor
    ~AudioManager();

    // Load audio file
    bool LoadAudio(const std::string& filename);

    // Play loaded audio
    void PlayAudio(const std::string& filename);  // Updated to accept a filename parameter

    // Play sound effect
    void PlaySoundEffect(const std::string& filename);

    // Set volume (0 to 128)
    void SetVolume(int volume);

    // Stop playing audio
    void StopAudio();

    // Pause audio
    void PauseAudio();

    // Resume audio
    void ResumeAudio();

private:
    SDL_AudioSpec wavSpec;      // Structure for audio specification
    Uint8* wavBuffer;           // Pointer to audio buffer
    Uint32 wavLength;           // Length of the audio buffer
    SDL_AudioDeviceID deviceId; // Audio device ID for playback
    int volume;                 // Volume level (0-128)
};

#endif // AUDIO_MANAGER_H
