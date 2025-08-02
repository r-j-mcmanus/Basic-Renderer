#include <vector>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>
#include <AudioFile/AudioFile.h>
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
	AudioFile<float> soundFile;
	if (!soundFile.load(path))
	{
		std::cerr << "failed to load " << name << " sound file " << path << std::endl;
		return;
	}
	std::vector<uint8_t> PCMDataBytes;
	soundFile.writePCMToBuffer(PCMDataBytes); //remember, we added this function to the AudioFile library

	ALuint soundBuffer;
	ALCall(alGenBuffers(1, &soundBuffer));
	ALCall(alBufferData(soundBuffer, convertFileToOpenALFormat(soundFile), PCMDataBytes.data(), PCMDataBytes.size(), soundFile.getSampleRate()));

	soundBuffers[name] = soundBuffer;
}


bool AudioManager::convertFileToOpenALFormat(AudioFile<float>& audioFile) {
	int bitDepth = audioFile.getBitDepth();
	if (bitDepth == 16)
		return audioFile.isStereo() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	else if (bitDepth == 8)
		return audioFile.isStereo() ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	else
		return false; // this shouldn't happen!
};


void AudioManager::playSource(ALuint source) {
	ALCall(alSourcePlay(source));
	ALint sourceState;
	ALCall(alGetSourcei(source, AL_SOURCE_STATE, &sourceState));
	while (sourceState == AL_PLAYING)
	{
		//basically loop until we're done playing the sound source
		ALCall(alGetSourcei(source, AL_SOURCE_STATE, &sourceState));
	}
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
