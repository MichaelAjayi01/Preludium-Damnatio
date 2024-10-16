// audio_manager.cpp
#include "audio_manager.h"
#include <iostream>

// Constructor
AudioManager::AudioManager() {
    // Initialize SDL audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    volume = 128; // Default volume level (max)
}

// Destructor
AudioManager::~AudioManager() {
    SDL_CloseAudioDevice(deviceId);
    SDL_Quit();
}

// Load audio file
bool AudioManager::LoadAudio(const std::string& filename) {
    if (SDL_LoadWAV(filename.c_str(), &wavSpec, &wavBuffer, &wavLength) == nullptr) {
        std::cerr << "Failed to load WAV file: " << filename << " SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);
    return true;
}

// Play loaded audio
void AudioManager::PlayAudio(const std::string& filename) {
    if (!LoadAudio(filename)) {
        return; // Exit if loading fails
    }

    SDL_QueueAudio(deviceId, wavBuffer, wavLength); // Queue the audio to play
    SDL_PauseAudioDevice(deviceId, 0); // Start playback
}

// Play sound effect
void AudioManager::PlaySoundEffect(const std::string& filename) {
    // Existing code for sound effect handling
}

// Set volume (0 to 128)
void AudioManager::SetVolume(int newVolume) {
    // Existing volume handling code
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
