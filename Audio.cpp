#include <vector>
#include <string>

#include <AL/al.h>
#include <AL/alc.h>
#include <AudioFile/AudioFile.h>

#include "Errors.h"


ALCdevice* getDefaultDevices()
{
	const ALCchar* defaultDeviceString = alcGetString(nullptr, defaultDeviceString);
	ALCdevice* device = alcOpenDevice(defaultDeviceString);
	if (!device) {
		std::cerr << "failed to get the default device for OpenAL" << std::endl;
		return nullptr;
	}
	std::cout << "OpenAL Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
	return device;
}

ALCcontext* createCurrentContext(ALCdevice* device)
{
	ALCcontext* context = alcCreateContext(device, nullptr);
	if (!alcMakeContextCurrent(context))
	{
		std::cerr << "failed to make the OpenAL context the current context" << std::endl;
		return nullptr;
	}
	return context;
}

void createListener()
{
	ALCall(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
	ALCall(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f));
	ALfloat forwardAndUpVectors[] = {
		/*forward = */ 1.f, 0.f, 0.f,
		/* up = */ 0.f, 1.f, 0.f
	};
	ALCall(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));
}

void createMonoSource(ALuint monoSoundBuffer)
{
	ALuint monoSource;
	ALCall(alGenSources(1, &monoSource));
	ALCall(alSource3f(monoSource, AL_POSITION, 1.f, 0.f, 0.f));
	ALCall(alSource3f(monoSource, AL_VELOCITY, 0.f, 0.f, 0.f));
	ALCall(alSourcef(monoSource, AL_PITCH, 1.f));
	ALCall(alSourcef(monoSource, AL_GAIN, 1.f));
	ALCall(alSourcei(monoSource, AL_LOOPING, AL_FALSE));
	ALCall(alSourcei(monoSource, AL_BUFFER, monoSoundBuffer));
}

void createSterioSource(ALuint monoSoundBuffer)
{
	//sterio sources are not given positions as they are not point like, eg. background music
	ALuint monoSource;
	ALCall(alGenSources(1, &monoSource));
	ALCall(alSourcef(monoSource, AL_PITCH, 1.f));
	ALCall(alSourcef(monoSource, AL_GAIN, 1.f));
	ALCall(alSourcei(monoSource, AL_LOOPING, AL_FALSE));
	ALCall(alSourcei(monoSource, AL_BUFFER, monoSoundBuffer));
}

void loadWaveFile() {
	AudioFile<float> soundFile;
	if (!soundFile.load("sounds/TestSound_Mono.wav"))
	{
		std::cerr << "failed to load the test mono sound file" << std::endl;
		return -1;
	}
	std::vector<uint8_t> PCMDataBytes;
	soundFile.writePCMToBuffer(PCMDataBytes); //remember, we added this function to the AudioFile library
	//auto convertFileToOpenALFormat = [](const AudioFile<float>& audioFile) {
	//	int bitDepth = audioFile.getBitDepth();
	//	if (bitDepth == 16)
	//		return audioFile.isStereo() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	//	else if (bitDepth == 8)
	//		return audioFile.isStereo() ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	//	else
	//		return -1; // this shouldn't happen!
	//	};

	ALuint soundBuffer;
	ALCall(alGenBuffers(1, &soundBuffer));
	ALCall(alBufferData(soundBuffer, convertFileToOpenALFormat(soundFile), PCMDataBytes.data(), PCMDataBytes.size(), soundFile.getSampleRate()));

}

auto convertFileToOpenALFormat = [](const AudioFile<float>& audioFile) {
	int bitDepth = audioFile.getBitDepth();
	if (bitDepth == 16)
		return audioFile.isStereo() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
	else if (bitDepth == 8)
		return audioFile.isStereo() ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
	else
		return -1; // this shouldn't happen!
	};


void playSource(ALuint source) {
	ALCall(alSourcePlay(source));
	ALint sourceState;
	ALCall(alGetSourcei(source, AL_SOURCE_STATE, &sourceState));
	while (sourceState == AL_PLAYING)
	{
		//basically loop until we're done playing the sound source
		ALCall(alGetSourcei(source, AL_SOURCE_STATE, &sourceState));
	}
}

void cleanup() {
	alec(alDeleteSources(1, &monoSource));
	alec(alDeleteSources(1, &stereoSource));
	alec(alDeleteBuffers(1, &monoSoundBuffer));
	alec(alDeleteBuffers(1, &stereoSoundBuffer));
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
}