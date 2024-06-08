#pragma once
#include <memory>
#include <queue>
#include <mutex>
namespace diji 
{
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() noexcept = default;
		virtual void AddSoundRequest(const std::string& audio, bool isMusic, int volume = -1) = 0;
	};

	class NullSoundSystem final : public ISoundSystem
	{
	public:
		void AddSoundRequest(const std::string& audio, bool isMusic, int volume = -1) override;
	};

	class ServiceLocator final
	{
	public:
		static ISoundSystem& GetSoundSystem() { return *_ss_instance; };
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& ss) 
		{ 
			_ss_instance = ss == nullptr ? std::make_unique<NullSoundSystem>() : std::move(ss);
		};

	private:
		static std::unique_ptr<ISoundSystem> _ss_instance;
	};

	class SDLISoundSystem final : public ISoundSystem
	{		
	public:
		SDLISoundSystem();
		~SDLISoundSystem() noexcept;
		void AddSoundRequest(const std::string& audio, bool isMusic, int volume = -1) override;
		
	private:
		void PlaySound(const std::string& audio, bool isMusic, const int volume) const;
		std::pair<std::pair<bool, int>, std::string>  GetNextSoundRequest();
	
		void ProcessSounds();

		std::jthread m_SoundThread;
		std::queue<std::pair<std::pair<bool, int>, std::string>> m_SoundQueue;
		std::mutex soundMutex_;
		bool m_IsRunning = false;
		// Allows threads to synchronize their execution based on certain conditions. 
		// It provides a way for one thread to notify other threads when a particular condition becomes true, allowing them to proceed with their execution
		std::condition_variable condition_;
	};

	class LoggingSoundSystem final : public ISoundSystem
	{
	public:
		explicit LoggingSoundSystem(std::unique_ptr<ISoundSystem>&& ss) : _real_ss{ std::move(ss) } {};
		virtual ~LoggingSoundSystem() noexcept override = default;

		void AddSoundRequest(const std::string& audio, bool isMusic, int volume = -1) override;
	private:
		std::unique_ptr<ISoundSystem> _real_ss;
	};
}