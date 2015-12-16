#pragma once
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alut.h"
#include <string>
#include <vector>
#include <map>
#include <Thread>

namespace Bengine {

	// Note: All functions return an unsigned int to get a PlayID.
	// This can be stored for later use if it is required to
	// stop a file playing at a very specific moment. If all
	// files with the same name can be stopped at the same time 
	// during usage, this ID can be ignored and stop(string)
	// can be called.
	class AudioManager
	{
	public:
		static AudioManager* getInstance();

		// Return false on fail, true on success. FileID is a developer given name to the file, so
		// they do not have to refer to it using the full file path any time the file is referenced. This
		// should only be used when the unique fileID will be used later on.
		bool loadAudioFile(const std::string& path, const std::string& fileID = "");

		// FileID refers to the developer given fileID in loadAudioFile.
		// This can also be the file's path. Can be called on
		// a file that was not previously loaded as well, but will always
		// need to be referred to by file path.
		unsigned int playOnce(const std::string& fileID, float volume = 1.0f);

		unsigned int playLooping(const std::string& fileID, float volume = 1.0f);

		unsigned int playMultipleTimes(const std::string& fileID, unsigned int numTimes, float volume = 1.0f);

		// If the unique playID sound is playing, it stops it immediately. 
		void stop(unsigned int playID);

	private:
		struct AudioThreadParameters
		{
			AudioThreadParameters(ALuint buf, std::string _filename, float _volume = 1.0f, bool looping = false,
				unsigned int _numTimes = 1) : buffer(buf), filename(_filename),
				volume(_volume), isLooping(looping), numTimes(_numTimes), isPlaying(true) {};
			ALuint buffer;
			std::string filename;
			bool isLooping;
			unsigned int numTimes;
			float volume;
			bool isPlaying;
		};

		static AudioManager* myInstance;
		std::map<std::string, ALuint> fileBufferMap; // Buffer values used in OpenAL for buffer id's associated to a filename.

													 // Play ID is generated once a play function is called. If stop is called using the given id,
													 // the correct thread will be stopped and memory properly cleared.
													 // If already stopped or successfully stopped, returns true.
													 // If not found, returns false.
		std::map<unsigned int, AudioThreadParameters*> playIDToThreadParameters;
		typedef std::map<unsigned int, AudioThreadParameters*>::iterator playIDIterator;

		void removePlayID(unsigned int playID);

		unsigned int genericPlay(const std::string& fileID, AudioThreadParameters p);

		// isLooping takes priority over numTimes. If isLooping is set to true, 
		// numTimes is irrelevant. 
		void playAudioThread(unsigned int playID, AudioThreadParameters *p);

		static unsigned int latestPlayID;

		AudioManager();
		~AudioManager();
	};

}