#pragma once

#include <string>
#include <unordered_map>

#include <AL/al.h>
#include <AL/alc.h>
#include <AudioFile/AudioFile.h>
#include <glm/glm.hpp>


struct SourceInfo {
	ALuint id;
	bool inUse = false; // used by audio component to indicate it has releced it
	bool playing = false; // used by audio manager to say the sound has ended
	bool dirty = false; // used by audio component to indicate position needs updating in openAL
	glm::vec3 position = glm::vec3(0,0,0); // the position of the source
	glm::vec3 velocity = glm::vec3(0,0,0); // the position of the source
	float pitch;
	float gain;

	void setPosition(glm::vec3 position) {
		this->position = position;
		dirty = true;
	}

	void setVelocity(glm::vec3 position) {
		this->position = position;
		dirty = true;
	}

	void setDynamics(glm::vec3 position, glm::vec3 velocity) {
		this->position = position;
		this->velocity = velocity;
		dirty = true;
	}
};


class AudioManager {

public:
	AudioManager();

	void createListener(glm::vec3 position, glm::vec3 velocity);

	void bindBufferToMonoSource(std::string bufferName, SourceInfo* source);
	void bindBufferToSterioSource(std::string bufferName, SourceInfo* source);

	void loadWavFile(std::string name, std::string path);
	void streamWavFile(std::string name, std::string path) {};

	void playSource(ALuint source);
	SourceInfo* getFreeSource();

private:

	ALCdevice* getDefaultDevice();
	void createCurrentContext(ALCdevice* device);
	bool convertFileToOpenALFormat(AudioFile<float>& audioFile);
	void makeSources();
	void cleanUp();

private:
	const int maxSources = 8;
	ALCcontext* context;
	ALCdevice* device;
	std::unordered_map<std::string, ALuint> soundBuffers;
	std::vector<SourceInfo> sources;
};
