#include "AudioManager.h"
#include "Errors.h"

namespace Bengine {

	AudioManager* AudioManager::myInstance = nullptr;
	unsigned int AudioManager::latestPlayID = 0;

	AudioManager*
	AudioManager::getInstance()
	{
		if (myInstance == nullptr)
			myInstance = new AudioManager();

		return myInstance;
	}

	AudioManager::AudioManager()
	{
		latestPlayID = 0;

		// Initialize the environment
		alutInit(0, NULL);

		// Clear all errors in the error buffer.
		alGetError();
	}

	bool
	AudioManager::loadAudioFile(const std::string& path, const std::string& fileID)
	{
		fileBufferMap[path] = alutCreateBufferFromFile(path.c_str());

		if (fileID != "")
			fileBufferMap[fileID] = fileBufferMap[path];

		ALenum error = alutGetError();
		if (error != ALUT_ERROR_NO_ERROR)
		{
			fatalError(alutGetErrorString(error));
			return false;
		}
		return true;
	}

	// FileID refers to the developer given fileID in loadAudioFile.
	// This can also be the file's path.
	unsigned int
	AudioManager::playOnce(const std::string& fileID, float volume)
	{
		AudioThreadParameters p = AudioThreadParameters(fileBufferMap[fileID], fileID, volume);
		return genericPlay(fileID, p);
	}

	unsigned int
	AudioManager::playLooping(const std::string& fileID, float volume)
	{
		AudioThreadParameters p = AudioThreadParameters(fileBufferMap[fileID], fileID, volume, true);
		return genericPlay(fileID, p);
	}

	unsigned int
	AudioManager::playMultipleTimes(const std::string& fileID, unsigned int numTimes, float volume)
	{
		AudioThreadParameters p = AudioThreadParameters(fileBufferMap[fileID], fileID, volume, false, numTimes);
		return genericPlay(fileID, p);
	}

	unsigned int
	AudioManager::genericPlay(const std::string& fileID, AudioThreadParameters p)
	{
		unsigned int uniqueID = latestPlayID;

		// File not loaded yet, attempt to load
		if (fileBufferMap[fileID] == 0 && !loadAudioFile(fileID))
			fatalError("File not previously loaded, and failed to load on attempt.");

		p.buffer = fileBufferMap[fileID];
		AudioThreadParameters *atp = new AudioThreadParameters(p.buffer, p.filename, p.volume, p.isLooping, p.numTimes);
		playIDToThreadParameters[uniqueID] = atp;

		std::thread audio(&AudioManager::playAudioThread, this, uniqueID, atp);
		audio.detach();

		latestPlayID++;
		return uniqueID;
	}

	void
	AudioManager::stop(unsigned int playID)
	{
		playIDIterator it = playIDToThreadParameters.find(playID);

		if (it == playIDToThreadParameters.end())
			return;   // Not found, could have stopped already
		else
			it->second->isPlaying = false;
	}

	void
	AudioManager::playAudioThread(unsigned int playID, AudioThreadParameters *p)
	{
		ALuint source;
		ALint state;

		alGenSources(1, &source);  // Create sound source (use buffer to fill source)
		alSourcei(source, AL_BUFFER, p->buffer); // set current buffer	
		alSourcef(source, AL_GAIN, p->volume); // Volume

		unsigned int count = 0;
		do
		{
			// Play
			alSourcePlay(source);

			// Wait for the file to complete
			do {
				alGetSourcei(source, AL_SOURCE_STATE, &state);
			} while (state == AL_PLAYING && p->isPlaying);

			if (!p->isLooping)
				count++;
		} while (count < p->numTimes && p->isPlaying);

		// Clean up sources and buffers
		alDeleteSources(1, &source);

		removePlayID(playID);
	}

	void
	AudioManager::removePlayID(unsigned int playID)
	{
		playIDIterator it = playIDToThreadParameters.find(playID);

		if (it != playIDToThreadParameters.end())
		{
			it->second->isPlaying = false;
			delete it->second;
			playIDToThreadParameters.erase(it);
		}
	}

	AudioManager::~AudioManager()
	{
		typedef std::map<std::string, ALuint>::iterator it_type;
		for (it_type iterator = fileBufferMap.begin();
		iterator != fileBufferMap.end(); iterator++)
		{
			alDeleteBuffers(1, &iterator->second);
		}

		// Exit everything
		alutExit();
	}

}