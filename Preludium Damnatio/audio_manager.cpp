#include "audio_manager.h"
#include <iostream>
#include <thread>

// Constructor
// Constructor
AudioManager::AudioManager()
    : wavBuffer(nullptr), wavLength(0), deviceId(0) // Initialize member variables
{
    // Initialize wavSpec to default values
    wavSpec.freq = 44100;               // Default frequency
    wavSpec.format = AUDIO_S16SYS;      // Default audio format
    wavSpec.channels = 2;                // Default number of channels (stereo)
    wavSpec.samples = 4096;             // Default buffer size
    wavSpec.callback = nullptr;          // No callback function
    wavSpec.userdata = nullptr;          // No user data

    // Initialize SDL audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    volume = 10;
}


// Destructor
AudioManager::~AudioManager() {
    SDL_CloseAudioDevice(deviceId);
    SDL_Quit();
}

// Load audio file
bool AudioManager::LoadAudio(const std::string& filename) {
    if (SDL_LoadWAV(filename.c_str(), &wavSpec, &wavBuffer, &wavLength) == nullptr) {
        return false;
    }
    deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);
    return true;
}

// Play loaded audio once
void AudioManager::PlayAudio(const std::string& filename) {
    if (!LoadAudio(filename)) {
        return; // Exit if loading fails
    }

    SDL_QueueAudio(deviceId, wavBuffer, wavLength); // Queue the audio to play
    SDL_PauseAudioDevice(deviceId, 0); // Start playback
}

// Play audio in a loop
void AudioManager::PlayAudioLoop(const std::string& filename) {
    if (!LoadAudio(filename)) {
        return; // Exit if loading fails
    }

    std::thread([this]() {
        SDL_PauseAudioDevice(deviceId, 0); // Start playback
        while (true) {
            SDL_QueueAudio(deviceId, wavBuffer, wavLength); // Queue audio for looping
            SDL_PauseAudioDevice(deviceId, 0); // Ensure playback starts again

            // Wait until the audio has finished playing
            SDL_Delay(wavLength / wavSpec.freq * 1000); // Convert length from samples to milliseconds
        }
        }).detach(); // Detach the thread to run independently
}

// Play sound effect
void AudioManager::PlaySoundEffect(const std::string& filename) {
    // Existing code for sound effect handling
}

// Set volume (0 to 128)
void AudioManager::SetVolume(int newVolume) {
    // Ensure the new volume is within the valid range
    if (newVolume < 0) newVolume = 0;
    if (newVolume > 128) newVolume = 128;

    volume = newVolume; // Update the internal volume level

    // Set the volume for the audio device
    SDL_MixAudioFormat(nullptr, nullptr, wavSpec.format, 0, volume); // This line sets the volume level
}


// Stop playing audio
void AudioManager::StopAudio() {
    SDL_ClearQueuedAudio(deviceId); // Clear any queued audio
}

// Pause audio
void AudioManager::PauseAudio() {
    SDL_PauseAudioDevice(deviceId, 1); // Pause playback
}

// Resume audio
void AudioManager::ResumeAudio() {
    SDL_PauseAudioDevice(deviceId, 0); // Resume playback
}
