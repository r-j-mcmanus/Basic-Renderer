#include <vector>
#include <string>
#include <thread>

#include <AL/al.h>
#include <AL/alc.h>
#define DR_WAV_IMPLEMENTATION
#include <dr_libs/dr_wav.h>
#include <glm/glm.hpp>

#include "Errors.h"

#include "Audio.h"


AudioManager::AudioManager() {
	device = AudioManager::getDefaultDevice();
	createCurrentContext(device);

	makeSources();
}


void AudioManager::makeSources() {
	// make a vector of ids and register them all
	std::vector<ALuint> tempIds(maxSources);
	alGenSources(static_cast<ALsizei>(maxSources), tempIds.data());

	//put these ids into our containers
	sources.resize(maxSources);
	for (size_t i = 0; i < maxSources; ++i) {
		sources[i].id = tempIds[i];
	}
}


ALCdevice* AudioManager::getDefaultDevice()
{
	const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
	ALCdevice* defaultDevice = alcOpenDevice(defaultDeviceString);
	if (!defaultDevice) {
		std::cerr << "failed to get the default device for OpenAL" << std::endl;
		return nullptr;
	}
	std::cout << "OpenAL Device: " << alcGetString(defaultDevice, ALC_DEVICE_SPECIFIER) << std::endl;
	return defaultDevice;
}


void AudioManager::createCurrentContext(ALCdevice* device)
{
	context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(context))
	{
		std::cerr << "failed to make the OpenAL context the current context" << std::endl;
	}
}


void AudioManager::createListener(glm::vec3 position, glm::vec3 velocity)
{
	ALCall(alListener3f(AL_POSITION, position.x, position.y, position.z));
	ALCall(alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z));
	ALfloat forwardAndUpVectors[] = {
		/*forward = */ 1.f, 0.f, 0.f,
		/* up = */ 0.f, 1.f, 0.f
	};
	ALCall(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));
}


void AudioManager::bindBufferToMonoSource(std::string bufferName, SourceInfo* source) {
	ALCall(alSource3f(source->id, AL_POSITION, source->position.x, source->position.y, source->position.z));
	// dopler
	ALCall(alSource3f(source->id, AL_VELOCITY, source->velocity.x, source->velocity.y, source->velocity.z));
	ALCall(alSourcef(source->id, AL_PITCH, source->pitch));
	ALCall(alSourcef(source->id, AL_GAIN, source->gain));
	ALCall(alSourcei(source->id, AL_LOOPING, AL_FALSE));
	ALCall(alSourcei(source->id, AL_BUFFER, soundBuffers[bufferName]));
}


void AudioManager::bindBufferToSterioSource(std::string bufferName, SourceInfo* source) {
	ALCall(alSourcef(source->id, AL_PITCH, source->pitch));
	ALCall(alSourcef(source->id, AL_GAIN, source->gain));
	ALCall(alSourcei(source->id, AL_LOOPING, AL_FALSE));
	ALCall(alSourcei(source->id, AL_BUFFER, soundBuffers[bufferName]));
}


void AudioManager::loadWavFile(std::string name, std::string path) {
	drwav wav;
	if (!drwav_init_file(&wav, path.c_str(), nullptr)) {
		std::cerr << "Failed to open WAV file: " << path << std::endl;
		return;
	}


	size_t totalSampleCount = wav.totalPCMFrameCount * wav.channels;
	std::vector<drwav_int16> pcmData(totalSampleCount);
	drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, pcmData.data());


	// Determine OpenAL format
	ALenum format;
	if (wav.channels == 1) format = AL_FORMAT_MONO16;
	else if (wav.channels == 2) format = AL_FORMAT_STEREO16;
	else {
		std::cerr << "Unsupported channel count: " << wav.channels << std::endl;
		drwav_uninit(&wav);
		return;
	}

	// Generate OpenAL buffer
	ALuint soundBuffer;
	alGenBuffers(1, &soundBuffer);
	alBufferData(
		soundBuffer,
		format, 
		pcmData.data(),
		static_cast<ALsizei>(pcmData.size() * sizeof(drwav_int16)),
		wav.sampleRate
	);

	// Clean up
	drwav_uninit(&wav);

	soundBuffers[name] = soundBuffer;
}



void AudioManager::playSource(SourceInfo* source) {
	ALCall(alSourcePlay(source->id));

	// make a thread that tracks the state of the source
	std::thread([source]() {
		// loop untill the source is no longer playing, no need to check often
		do {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			ALCall(alGetSourcei(source->id, AL_SOURCE_STATE, &source->state));
		} while (source->state == AL_PLAYING);
	}).detach(); // detash as we do not need the main thread to wait
}


void AudioManager::cleanUp() {
	for (auto& pair : soundBuffers) {
		ALCall(alDeleteBuffers(1, &pair.second));
	}
	soundBuffers.clear();

	for (auto& source : sources) {
		ALCall(alDeleteSources(1, &source.id));
	}

	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}


SourceInfo* AudioManager::getFreeSource() {
	for (auto& source : sources) {
		if (!source.inUse) {
			ALint state;
			alGetSourcei(source.id, AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING) {
				source.inUse = true;
				return &source;
			}
		}
	}
	return 0; // No free source available
}
